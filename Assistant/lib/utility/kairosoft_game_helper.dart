import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/vdf_helper.dart';
import '/utility/byte_stream_view.dart';
import '/utility/external_tool_helper.dart';
import 'dart:ui';
import 'dart:math';
import 'dart:async';
import 'dart:typed_data';
import 'dart:ffi' as lib;
import 'package:ffi/ffi.dart' as lib;
import 'package:collection/collection.dart';
import 'package:archive/archive_io.dart' as lib;
import 'package:win32/win32.dart' as lib;

// ----------------

enum GamePlatform {
  windowsIntel32,
  androidArm32,
  androidArm64,
}

enum GameProgramState {
  none,
  original,
  modified,
}

enum GameRecordState {
  none,
  original,
  decrypted,
  invalid,
}

enum GamePackageType {
  flat,
  zip,
  apk,
  apks,
}

class GameInformation {
  String           path;
  String?          library;
  String?          identifier;
  String?          version;
  String           name;
  Image?           icon;
  String?          key;
  GameProgramState program;
  GameRecordState  record;
  GameInformation({
    this.path = '',
    this.library = null,
    this.identifier = null,
    this.version = null,
    this.name = '',
    this.icon = null,
    this.key = null,
    this.program = .none,
    this.record = .none,
  });
}

enum GameFunctionType {
  modifyProgram,
  encryptRecord,
  importRecord,
  exportRecord,
}

class GameProgramHelper {

  // #region common

  static String _getProgramFilePath(
    GamePlatform platform,
  ) {
    return switch (platform) {
      .windowsIntel32 => 'GameAssembly.dll',
      .androidArm32   => 'lib/armeabi-v7a/libil2cpp.so',
      .androidArm64   => 'lib/arm64-v8a/libil2cpp.so',
    };
  }

  static String _getMetadataFilePath(
    GamePlatform platform,
  ) {
    return switch (platform) {
      .windowsIntel32 => 'KairoGames_Data/il2cpp_data/Metadata/global-metadata.dat',
      .androidArm32   => 'assets/bin/Data/Managed/Metadata/global-metadata.dat',
      .androidArm64   => 'assets/bin/Data/Managed/Metadata/global-metadata.dat',
    };
  }

  // ----------------

  static Future<List<GamePlatform>> detectPlatform(
    String gameDirectory,
  ) async {
    assertTest(await StorageHelper.existDirectory(gameDirectory));
    var result = <GamePlatform>[];
    for (var platform in GamePlatform.values) {
      if (!await StorageHelper.existFile('${gameDirectory}/${_getProgramFilePath(platform)}')) {
        continue;
      }
      if (!await StorageHelper.existFile('${gameDirectory}/${_getMetadataFilePath(platform)}')) {
        continue;
      }
      result.add(platform);
    }
    return result;
  }

  // #endregion

  // #region asm

  static const Integer _instructionCodeNopIntel32 = 0x90;

  static const Integer _instructionCodeNopArm32 = 0xE320F000;

  static const Integer _instructionCodeNopArm64 = 0xD503201F;

  // ----------------

  static Boolean _findCallInstruction(
    ByteStreamView data,
    Integer        limit,
    List<Integer>  address,
    Boolean        overwrite,
    GamePlatform   platform,
  ) {
    var state = false;
    var dataEnd = min(data.size, data.position + limit);
    if (platform == .windowsIntel32) {
      while (data.position < dataEnd) {
        var instructionCode = data.getIntegerU8();
        // call #X = E8 XX XX XX XX
        if (instructionCode != 0xE8) {
          continue;
        }
        var jumpOffset = data.getInteger32();
        var jumpAddress = data.position + jumpOffset;
        if (!address.contains(jumpAddress)) {
          data.position -= 4;
          continue;
        }
        if (overwrite) {
          data.position -= 5;
          data.setIntegerU8(_instructionCodeNopIntel32);
          data.setIntegerU8(_instructionCodeNopIntel32);
          data.setIntegerU8(_instructionCodeNopIntel32);
          data.setIntegerU8(_instructionCodeNopIntel32);
          data.setIntegerU8(_instructionCodeNopIntel32);
        }
        state = true;
        break;
      }
    }
    if (platform == .androidArm32) {
      while (data.position < dataEnd) {
        var instructionCode = data.getIntegerU32();
        // bl #X = EB XX XX XX
        if ((instructionCode & 0xFF000000) != 0xEB000000) {
          continue;
        }
        var jumpOffset = instructionCode & 0x00FFFFFF;
        if ((jumpOffset & 0x800000) == 0x800000) {
          jumpOffset = -(0x1000000 - jumpOffset);
        }
        var jumpAddress = data.position - 4 + 8 + jumpOffset * 4;
        if (!address.contains(jumpAddress)) {
          continue;
        }
        if (overwrite) {
          data.position -= 4;
          data.setIntegerU32(_instructionCodeNopArm32);
        }
        state = true;
        break;
      }
    }
    if (platform == .androidArm64) {
      while (data.position < dataEnd) {
        var instructionCode = data.getIntegerU32();
        // bl #X = 97 XX XX XX
        if ((instructionCode & 0xFF000000) != 0x97000000) {
          continue;
        }
        var jumpOffset = instructionCode & 0x00FFFFFF;
        if ((jumpOffset & 0x800000) == 0x800000) {
          jumpOffset = -(0x1000000 - jumpOffset);
        }
        var jumpAddress = data.position - 4 + jumpOffset * 4;
        if (!address.contains(jumpAddress)) {
          continue;
        }
        if (overwrite) {
          data.position -= 4;
          data.setIntegerU32(_instructionCodeNopArm64);
        }
        state = true;
        break;
      }
    }
    return state;
  }

  // #endregion

  // #region modify

  static Future<Void> modify(
    GamePlatform          platform,
    String                programFile,
    String                metadataFile,
    Boolean               disableRecordEncryption,
    Boolean               enableDebugMode,
    dynamic               externalToolSetting,
    Void Function(String) onNotify,
  ) async {
    onNotify('Phase: dump program information.');
    var dumpData = await ExternalToolHelper.runIl2cppdumper(programFile, metadataFile);
    onNotify('Phase: parse symbol address.');
    var symbolAddress = <String, List<Integer>>{};
    {
      symbolAddress['CRC64.GetValue'] = ExternalToolHelper.doIl2cppdumperSearchMethodFromDumpData(dumpData, 'CRC64', 'GetValue')
        .selfAlso((it) => assertTest(it.length == 1))
        .map((it) => it.$1)
        .toList();
      symbolAddress['Encrypter.Encode'] = ExternalToolHelper.doIl2cppdumperSearchMethodFromDumpData(dumpData, 'Encrypter', 'Encode')
        .selfAlso((it) => assertTest(it.length == 3))
        .map((it) => it.$1)
        .toList();
      symbolAddress['Encrypter.Decode'] = ExternalToolHelper.doIl2cppdumperSearchMethodFromDumpData(dumpData, 'Encrypter', 'Decode')
        .selfAlso((it) => assertTest(it.length == 3))
        .map((it) => it.$1)
        .toList();
      symbolAddress['RecordStore.ReadRecord'] = ExternalToolHelper.doIl2cppdumperSearchMethodFromDumpData(dumpData, 'RecordStore', 'ReadRecord')
        .where((it) => !it.$3 && it.$5 == 'int rcId')
        .selfAlso((it) => assertTest(it.length == 1))
        .map((it) => it.$1)
        .toList();
      symbolAddress['RecordStore.WriteRecord'] = ExternalToolHelper.doIl2cppdumperSearchMethodFromDumpData(dumpData, 'RecordStore', 'WriteRecord')
        .where((it) => !it.$3 && it.$5 == 'int rcId, byte[][] data')
        .selfAlso((it) => assertTest(it.length == 1))
        .map((it) => it.$1)
        .toList();
      symbolAddress['MyConfig..cctor'] = ExternalToolHelper.doIl2cppdumperSearchMethodFromDumpData(dumpData, 'MyConfig', '.cctor')
        .selfAlso((it) => assertTest(it.length == 1))
        .map((it) => it.$1)
        .toList();
      symbolAddress['MyConfig.DEBUG'] = ExternalToolHelper.doIl2cppdumperSearchFieldFromDumpData(dumpData, 'MyConfig', 'DEBUG')
        .selfAlso((it) => assertTest(it.length == 1))
        .map((it) => it.$1)
        .toList();
      for (var symbolName in symbolAddress.keys) {
        onNotify('Tip: the symbol \'${symbolName}\' at ${symbolAddress[symbolName]!.map((value) => value.toRadixString(16).padLeft(8, '0')).join(',')}.');
      }
    }
    onNotify('Phase: load original program.');
    var programData = await StorageHelper.readFile(programFile);
    var programStream = ByteStreamView(programData.buffer.asByteData());
    if (disableRecordEncryption) {
      onNotify('Phase: modify method \'RecordStore.ReadRecord\'.');
      programStream.position = symbolAddress['RecordStore.ReadRecord']!.first;
      assertTest(_findCallInstruction(programStream, 0x1000, symbolAddress['Encrypter.Decode']!, true, platform));
      assertTest(_findCallInstruction(programStream, 0x1000, symbolAddress['Encrypter.Decode']!, true, platform));
      assertTest(_findCallInstruction(programStream, 0x1000, symbolAddress['CRC64.GetValue']!, false, platform));
      if (platform == .windowsIntel32) {
        // add esp, .. = 83 C4 XX
        assertTest(programStream.getIntegerU8() == 0x83);
        assertTest(programStream.getIntegerU8() == 0xC4);
        programStream.position += 1;
        // cmp eax, .. = 3B XX XX
        assertTest(programStream.getIntegerU8() == 0x3B);
        programStream.setIntegerU8(_instructionCodeNopIntel32);
        programStream.setIntegerU8(_instructionCodeNopIntel32);
        programStream.setIntegerU8(_instructionCodeNopIntel32);
        // jnz .. = 75 XX
        assertTest(programStream.getIntegerU8() == 0x75);
        programStream.setIntegerU8(_instructionCodeNopIntel32);
        programStream.setIntegerU8(_instructionCodeNopIntel32);
      }
      if (platform == .androidArm32) {
        // eor; eor; orrs
        programStream.position += 12;
        // bne #X = 1A XX XX XX
        assertTest((programStream.getIntegerU32() & 0xFF000000) == 0x1A000000);
        programStream.position -= 4;
        programStream.setIntegerU32(_instructionCodeNopArm32);
      }
      if (platform == .androidArm64) {
        // cmp; mov
        programStream.position += 8;
        // bne #X = 54 XX XX XX
        assertTest((programStream.getIntegerU32() & 0xFF000000) == 0x54000000);
        programStream.position -= 4;
        programStream.setIntegerU32(_instructionCodeNopArm64);
      }
    }
    if (disableRecordEncryption) {
      onNotify('Phase: modify method \'RecordStore.WriteRecord\'.');
      programStream.position = symbolAddress['RecordStore.WriteRecord']!.first;
      assertTest(_findCallInstruction(programStream, 0x1000, symbolAddress['Encrypter.Encode']!, true, platform));
      assertTest(_findCallInstruction(programStream, 0x1000, symbolAddress['Encrypter.Encode']!, true, platform));
    }
    if (enableDebugMode) {
      onNotify('Phase: modify method \'MyConfig..cctor\'.');
      programStream.position = symbolAddress['MyConfig..cctor']!.first;
      var programStreamEnd = programStream.position + 0x200;
      if (platform == .windowsIntel32) {
        while (programStream.position < programStreamEnd) {
          // mov byte ptr [eax+X], 0 = C6 40 XX 00
          var instructionCode = 0;
          instructionCode = programStream.getIntegerU8();
          if (instructionCode != 0xC6) {
            continue;
          }
         instructionCode = programStream.getIntegerU8();
          if (instructionCode != 0x40) {
            programStream.position -= 1;
            continue;
          }
          instructionCode = programStream.getIntegerU8();
          if (instructionCode != symbolAddress['MyConfig.DEBUG']!.first) {
            programStream.position -= 1;
            continue;
          }
          instructionCode = programStream.getIntegerU8();
          if (instructionCode != 0x00) {
            programStream.position -= 1;
            continue;
          }
          programStream.position -= 1;
          programStream.setIntegerU8(0x01);
          break;
        }
      }
      if (platform == .androidArm32) {
        while (programStream.position < programStreamEnd) {
          // strb rX, [rY, #Z] = 111001011100 YYYY XXXX ZZZZZZZZZZZZ
          var instructionCode = programStream.getIntegerU32();
          if ((instructionCode & 0xFFF00000) != 0xE5C00000) {
            continue;
          }
          if ((instructionCode & 0x00000FFF) >> 0 != symbolAddress['MyConfig.DEBUG']!.first + 4) {
            continue;
          }
          programStream.position -= 4;
          programStream.setIntegerU32((instructionCode & 0xFFFF0000) | (14 << 12) | (symbolAddress['MyConfig.DEBUG']!.first << 0));
          break;
        }
      }
      if (platform == .androidArm64) {
        while (programStream.position < programStreamEnd) {
          // strb wX, [xY, #Z] = 0011100100 ZZZZZZZZZZZZ YYYYY XXXXX
          var instructionCode = programStream.getIntegerU32();
          if ((instructionCode & 0xFFC00000) != 0x39000000) {
            continue;
          }
          if ((instructionCode & 0x003FFC00) >> 10 != symbolAddress['MyConfig.DEBUG']!.first + 4) {
            continue;
          }
          programStream.position -= 4;
          programStream.setIntegerU32((instructionCode & 0xFFC003E0) | (symbolAddress['MyConfig.DEBUG']!.first << 10) | (30 << 0));
          break;
        }
      }
      assertTest(programStream.position != programStreamEnd);
      onNotify('Warning: the STR instruction for \'MyConfig.DEBUG\'+4 was found at ${(programStream.position - 4).toRadixString(16).padLeft(8, '0')}, but this modification may cause error.');
    }
    onNotify('Phase: save modified program.');
    await StorageHelper.writeFile(programFile, programStream.view.buffer.asUint8List());
    return;
  }

  static Future<Void> modifyBundle(
    String                target,
    Boolean               disableRecordEncryption,
    Boolean               enableDebugMode,
    dynamic               externalToolSetting,
    Void Function(String) onNotify,
  ) async {
    var temporaryDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(temporaryDirectory);
    onNotify('Phase: detect package type.');
    var packageType = null as GamePackageType?;
    if (await StorageHelper.existDirectory(target)) {
      packageType = .flat;
    }
    if (await StorageHelper.existFile(target)) {
      if (target.toLowerCase().endsWith('.zip')) {
        packageType = .zip;
      }
      if (target.toLowerCase().endsWith('.apk')) {
        packageType = .apk;
      }
      if (target.toLowerCase().endsWith('.apks')) {
        packageType = .apks;
      }
    }
    assertTest(packageType != null);
    onNotify('Tip: the package type is \'${packageType!.name}\'.');
    var packageBundle = null as lib.Archive?;
    var packagePartList = null as List<(String, lib.Archive)>?;
    if (packageType != .flat) {
      onNotify('Phase: load package file.');
      packageBundle = lib.ZipDecoder().decodeBytes(await StorageHelper.readFile(target));
      packagePartList = switch (packageType) {
        .zip  => [('main.zip', packageBundle)],
        .apk  => [('main.apk', packageBundle)],
        .apks => packageBundle.files.where((it) => it.isFile && it.name.toLowerCase().endsWith('.apk')).map((it) => (it.name, lib.ZipDecoder().decodeBytes(it.content))).toList(),
        _     => throw UnreachableException(),
      };
    }
    onNotify('Phase: extract necessary file.');
    var targetDirectory = '${temporaryDirectory}/flat';
    var necessaryFileNameList = GamePlatform.values.map((it) => [_getProgramFilePath(it), _getMetadataFilePath(it)]).flattenedToSet;
    if (packageType == .flat) {
      for (var necessaryFileName in necessaryFileNameList) {
        if (await StorageHelper.existFile('${target}/${necessaryFileName}')) {
          await StorageHelper.copy('${target}/${necessaryFileName}', '${targetDirectory}/${necessaryFileName}');
        }
      }
    }
    else {
      packagePartList!;
      for (var packagePart in packagePartList) {
        for (var necessaryFileName in necessaryFileNameList) {
          var necessaryFile = packagePart.$2.find(necessaryFileName);
          if (necessaryFile == null) {
            continue;
          }
          await StorageHelper.writeFile('${targetDirectory}/${necessaryFileName}', necessaryFile.content);
          await necessaryFile.close();
        }
      }
    }
    onNotify('Phase: detect platform.');
    var platformList = await detectPlatform(targetDirectory);
    assertTest(platformList.isNotEmpty);
    onNotify('Tip: the platform is \'${platformList.map((it) => it.name).join('|')}\'.');
    for (var platform in platformList) {
      onNotify('Phase: modify program of \'${platform.name}\'.');
      await modify(
        platform,
        '${targetDirectory}/${_getProgramFilePath(platform)}',
        '${targetDirectory}/${_getMetadataFilePath(platform)}',
        disableRecordEncryption,
        enableDebugMode,
        externalToolSetting,
        onNotify,
      );
    }
    if (packageType != .flat) {
      onNotify('Phase: repack package file.');
      packagePartList!;
      var replaceTaskList = <(lib.Archive, GamePlatform)>[];
      for (var platform in platformList) {
        if (packageType == .zip || packageType == .apk) {
          replaceTaskList.add((packagePartList.first.$2, platform));
        }
        if (packageType == .apks) {
          var architectureName = switch (platform) {
            .androidArm32 => 'armeabi_v7a',
            .androidArm64 => 'arm64_v8a',
            _             => throw UnreachableException(),
          };
          var packagePartName = 'split_config.${architectureName}.apk';
          replaceTaskList.add((packagePartList.firstWhere((it) => it.$1 == packagePartName).$2, platform));
        }
      }
      for (var replaceTask in replaceTaskList) {
        var packagePart = replaceTask.$1;
        var platform = replaceTask.$2;
        var fileName = _getProgramFilePath(platform);
        packagePart.removeFile(packagePart.find(fileName)!);
        packagePart.add(.bytes(fileName, await StorageHelper.readFile('${targetDirectory}/${fileName}')));
      }
      for (var packagePart in packagePartList) {
        await StorageHelper.writeFile('${temporaryDirectory}/package/${packagePart.$1}', lib.ZipEncoder().encodeBytes(packagePart.$2));
      }
    }
    if (packageType == .apk || packageType == .apks) {
      onNotify('Phase: post-processing apk file.');
      packagePartList!;
      var enableAlign = false;
      var enableSign = false;
      // TODO
      for (var packagePart in packagePartList) {
        var packagePartFile = '${temporaryDirectory}/package/${packagePart.$1}';
        if (enableAlign) {
          await ExternalToolHelper.runZipalign(packagePartFile);
        }
        if (enableSign) {
          await ExternalToolHelper.runApksigner(packagePartFile, '', '');
        }
      }
    }
    onNotify('Phase: generate result.');
    if (packageType == .flat) {
      for (var platform in platformList) {
        await StorageHelper.copy('${targetDirectory}/${_getProgramFilePath(platform)}', '${target}/${_getProgramFilePath(platform)}');
      }
    }
    if (packageType == .zip || packageType == .apk) {
      packagePartList!;
      await StorageHelper.copy('${temporaryDirectory}/package/${packagePartList.first.$1}', target);
    }
    if (packageType == .apks) {
      packageBundle!;
      packagePartList!;
      for (var packagePart in packagePartList) {
        packageBundle.removeFile(packageBundle.find(packagePart.$1)!);
        packageBundle.add(.bytes(packagePart.$1, await StorageHelper.readFile('${temporaryDirectory}/package/${packagePart.$1}')));
      }
      await StorageHelper.writeFile('${temporaryDirectory}/package/bundle.apks', lib.ZipEncoder().encodeBytes(packageBundle));
      await StorageHelper.copy('${temporaryDirectory}/package/bundle.apks', target);
    }
    await StorageHelper.remove(temporaryDirectory);
    onNotify('Phase: done.');
    return;
  }

  // #endregion

}

class GameRecordHelper {

  // #region common

  static Future<List<String>> listFile(
    String recordDirectory,
  ) async {
    return (await StorageHelper.listDirectory(recordDirectory, 1, true, false))
      .where(RegExp(r'^\d{4,4}(_backup)?$').hasMatch)
      .toList();
  }

  static Future<GameRecordState> detectState(
    String     recordDirectory,
    Uint8List? key,
  ) async {
    var state = GameRecordState.invalid;
    var itemList = await listFile(recordDirectory);
    if (itemList.length == 0) {
      state = .none;
    }
    else if (itemList.contains('0000')) {
      var itemFile = '${recordDirectory}/0000';
      var itemData = await StorageHelper.readFile(itemFile); // TODO 8
      if (itemData.length == 8) {
        if (itemData.buffer.asUint32List().first == 0x00000000) {
          state = .decrypted;
        }
        else if (key != null) {
          encryptData(itemData, key);
          if (itemData.buffer.asUint32List().first == 0x00000000) {
            state = .original;
          }
        }
      }
    }
    return state;
  }

  // #endregion

  // #region encrypt

  static Void encryptData(
    Uint8List  data,
    Uint8List? key,
  ) {
    if (key != null) {
      assertTest(key.isNotEmpty);
      for (var index = 0; index < data.length; index++) {
        data[index] ^= key[index % key.length];
      }
    }
    return;
  }

  static Future<Void> encryptFile(
    String     sourceFile,
    String     destinationFile,
    Uint8List? key,
  ) async {
    var data = await StorageHelper.readFile(sourceFile);
    encryptData(data, key);
    await StorageHelper.writeFile(destinationFile, data);
    return;
  }

  static Future<Void> encrypt(
    String     recordDirectory,
    Uint8List? key,
  ) async {
    var itemList = await listFile(recordDirectory);
    for (var item in itemList) {
      await encryptFile('${recordDirectory}/${item}', '${recordDirectory}/${item}', key);
    }
    return;
  }

  // #endregion

  // #region archive

  static Future<Void> export(
    String     targetDirectory,
    String     archiveFile,
    Uint8List? key,
  ) async {
    var archiveDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(archiveDirectory);
    // TODO
    await StorageHelper.writeFileText('${archiveDirectory}/configuration', '');
    await StorageHelper.createDirectory('${archiveDirectory}/data');
    for (var dataFile in await listFile(targetDirectory)) {
      await encryptFile('${targetDirectory}/${dataFile}', '${targetDirectory}/data/${dataFile}', key);
    }
    if (await StorageHelper.exist(archiveFile)) {
      await StorageHelper.remove(archiveFile);
    }
    var archive = lib.createArchiveFromDirectory(.new(archiveDirectory));
    var archiveData = lib.ZipEncoder().encodeBytes(archive, level: lib.DeflateLevel.bestCompression);
    await StorageHelper.writeFile(archiveFile, archiveData);
    await StorageHelper.remove(archiveDirectory);
    return;
  }

  static Future<Void> import(
    String     targetDirectory,
    String     archiveFile,
    Uint8List? key,
  ) async {
    var archiveDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(archiveDirectory);
    var archiveData = await StorageHelper.readFile(archiveFile);
    var archive = lib.ZipDecoder().decodeBytes(archiveData);
    await lib.extractArchiveToDisk(archive, archiveDirectory);
    if (await StorageHelper.exist(targetDirectory)) {
      await StorageHelper.remove(targetDirectory);
    }
    // TODO
    var configuration = StorageHelper.readFileText('${archiveDirectory}/configuration');
    await StorageHelper.createDirectory(targetDirectory);
    for (var dataFile in await listFile('${archiveDirectory}/data')) {
      await encryptFile('${archiveDirectory}/data/${dataFile}', '${targetDirectory}/${dataFile}', key);
    }
    await StorageHelper.remove(archiveDirectory);
    return;
  }

  // #endregion

}

class GameRepositoryHelper {

  // #region common

  static Future<Image?> extractProgramIcon(
    String programFile,
  ) async {
    var result = null as Image?;
    var hIconPath = programFile.toNativeUtf16();
    var hIconIndex = lib.calloc.call<lib.Uint16>();
    var hIcon = lib.ExtractAssociatedIcon(lib.NULL, hIconPath, hIconIndex);
    lib.calloc.free(hIconPath);
    lib.calloc.free(hIconIndex);
    var hIconInfo = lib.calloc.call<lib.ICONINFO>();
    if (lib.GetIconInfo(hIcon, hIconInfo) == lib.TRUE) {
      var hBitmap = lib.calloc.call<lib.BITMAP>();
      if (lib.GetObject(hIconInfo.ref.hbmColor, lib.sizeOf<lib.BITMAP>(), hBitmap) != 0) {
        var hBitmapInfo = lib.calloc.call<lib.BITMAPINFO>();
        hBitmapInfo.ref.bmiHeader.biSize = lib.sizeOf<lib.BITMAPINFOHEADER>();
        hBitmapInfo.ref.bmiHeader.biWidth = hBitmap.ref.bmWidth;
        hBitmapInfo.ref.bmiHeader.biHeight = -hBitmap.ref.bmHeight;
        hBitmapInfo.ref.bmiHeader.biPlanes = 1;
        hBitmapInfo.ref.bmiHeader.biBitCount = 32;
        hBitmapInfo.ref.bmiHeader.biCompression = lib.BI_RGB;
        var hDeviceContext = lib.GetDC(lib.NULL);
        if (hDeviceContext != lib.NULL) {
          var hDataSize = hBitmap.ref.bmWidth * hBitmap.ref.bmHeight * 4;
          var hData = lib.calloc.call<lib.Uint8>(hDataSize);
          if (lib.GetDIBits(hDeviceContext, hIconInfo.ref.hbmColor, 0, hBitmap.ref.bmHeight, hData, hBitmapInfo, lib.DIB_RGB_COLORS) != 0) {
            result = await ConvertHelper.parseImageFromData(hData.asTypedList(hDataSize), width: hBitmap.ref.bmWidth, height: hBitmap.ref.bmHeight, isRawBgra: true);
          }
          lib.calloc.free(hData);
        }
        assertTest(lib.ReleaseDC(lib.NULL, hDeviceContext) != 0);
        lib.calloc.free(hBitmapInfo);
        assertTest(lib.DeleteObject(hIconInfo.ref.hbmColor) != 0);
        assertTest(lib.DeleteObject(hIconInfo.ref.hbmMask) != 0);
      }
      lib.calloc.free(hBitmap);
    }
    lib.calloc.free(hIconInfo);
    return result;
  }

  static Future<(GameProgramState, GameRecordState)> checkGameState(
    String  gameDirectory,
    String? version,
    String  user,
  ) async {
    var programState = GameProgramState.none;
    var recordState = GameRecordState.none;
    if (await StorageHelper.existFile('${gameDirectory}/KairoGames.exe')) {
      programState = !await StorageHelper.existFile('${gameDirectory}/KairoGames.exe.${version ?? '0'}.bak')
        ? .original
        : .modified;
    }
    if (await StorageHelper.existDirectory('${gameDirectory}/saves/${user}')) {
      recordState = await GameRecordHelper.detectState('${gameDirectory}/saves/${user}', makeKeyFromSteamUser(user));
    }
    return (programState, recordState);
  }

  // #endregion

  // #region custom

  static Future<GameInformation?> loadCustomGame(
    String gameDirectory,
  ) async {
    if (!await StorageHelper.existFile('${gameDirectory}/KairoGames.exe')) {
      return null;
    }
    var information = GameInformation();
    information.path = gameDirectory;
    information.library = null;
    information.identifier = null;
    information.version = null;
    information.name = StorageHelper.name(gameDirectory);
    information.icon = await extractProgramIcon('${gameDirectory}/KairoGames.exe');
    information.key = '0';
    if (!await StorageHelper.existDirectory('${gameDirectory}/saves')) {
      information.key = (await StorageHelper.listDirectory('${gameDirectory}/saves', 1, false, true)).firstWhereOrNull((it) => RegExp(r'^\d+$').hasMatch(it)) ?? '0';
    }
    var gameState = await checkGameState(gameDirectory, information.version, information.key!);
    information.program = gameState.$1;
    information.record = gameState.$2;
    return information;
  }

  static Future<Boolean> checkCustomRepository(
    String repositoryDirectory,
  ) async {
    return await StorageHelper.existDirectory('${repositoryDirectory}');
  }

  static Future<List<GameInformation>> loadCustomRepository(
    String repositoryDirectory,
  ) async {
    var libraryList = await StorageHelper.listDirectory(repositoryDirectory, 1, false, true);
    var result = <GameInformation>[];
    for (var library in libraryList) {
      var libraryDirectory = '${repositoryDirectory}/${library}';
      var gameConfiguration = await loadCustomGame(libraryDirectory);
      if (gameConfiguration != null) {
        result.add(gameConfiguration);
      }
    }
    return result;
  }

  // #endregion

  // #region steam

  static Uint8List makeKeyFromSteamUser(
    String user,
  ) {
    var keyValue = Integer.parse(user);
    var key = Uint64List.fromList([keyValue]).buffer.asUint8List();
    return key;
  }

  static Future<GameInformation?> loadSteamGame(
    String libraryDirectory,
    String gameIdentifier,
  ) async {
    var gameManifestFile = '${libraryDirectory}/steamapps/appmanifest_${gameIdentifier}.acf';
    if (!await StorageHelper.existFile(gameManifestFile)) {
      return null;
    }
    var gameManifest = await VdfHelper.deserializeFile(gameManifestFile);
    assertTest(gameManifest.length == 1);
    assertTest(gameManifest.entries.first.key == 'AppState');
    assertTest(gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['appid']!.as<Integer>().toString() == gameIdentifier);
    var gameDirectory = '${libraryDirectory}/steamapps/common/${gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['installdir']!.as<String>()}';
    if (!await StorageHelper.existFile('${gameDirectory}/KairoGames.exe')) {
      return null;
    }
    var information = GameInformation();
    information.path = gameDirectory;
    information.library = libraryDirectory;
    information.identifier = gameIdentifier;
    information.version = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['buildid']!.as<Integer>().toString();
    information.name = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['name']!.as<String>();
    information.icon = await extractProgramIcon('${gameDirectory}/KairoGames.exe');
    information.key = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['LastOwner']!.as<Integer>().toString();
    var gameState = await checkGameState(gameDirectory, information.version, information.key!);
    information.program = gameState.$1;
    information.record = gameState.$2;
    return information;
  }

  static Future<Boolean> checkSteamRepository(
    String repositoryDirectory,
  ) async {
    return await StorageHelper.existFile('${repositoryDirectory}/steam.exe');
  }

  static Future<List<GameInformation>> loadSteamRepository(
    String repositoryDirectory,
  ) async {
    var libraryList = await VdfHelper.deserializeFile('${repositoryDirectory}/steamapps/libraryfolders.vdf');
    assertTest(libraryList.length == 1);
    assertTest(libraryList.entries.first.key == 'libraryfolders');
    var result = <GameInformation>[];
    for (var library in libraryList['libraryfolders']!.as<Map<Object?, Object?>>().entries) {
      var libraryDirectory = library.value!.as<Map<Object?, Object?>>()['path']!.as<String>().replaceAll('\\\\', '/');
      for (var game in library.value!.as<Map<Object?, Object?>>()['apps']!.as<Map<Object?, Object?>>().entries) {
        var gameIdentifier = game.key!.as<String>();
        var gameConfiguration = await loadSteamGame(libraryDirectory, gameIdentifier);
        if (gameConfiguration != null) {
          result.add(gameConfiguration);
        }
      }
    }
    return result;
  }

  // #endregion

}
