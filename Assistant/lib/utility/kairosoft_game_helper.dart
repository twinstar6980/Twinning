import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/vdf_helper.dart';
import 'dart:ui';
import 'dart:async';
import 'dart:typed_data';
import 'dart:ffi' as lib;
import 'package:ffi/ffi.dart' as lib;
import 'package:collection/collection.dart';
import 'package:archive/archive_io.dart' as lib;
import 'package:win32/win32.dart' as lib;

// ----------------

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

class GameInformation {
  StoragePath      path;
  StoragePath?     library;
  String?          identifier;
  String?          version;
  String           name;
  Image?           icon;
  String?          key;
  GameProgramState program;
  GameRecordState  record;
  GameInformation(
  ) :
    this.path = .new(),
    this.library = null,
    this.identifier = null,
    this.version = null,
    this.name = '',
    this.icon = null,
    this.key = null,
    this.program = .none,
    this.record = .none;
}

class GameRecordHelper {

  // #region common

  static Future<List<StoragePath>> listFile(
    StoragePath recordDirectory,
  ) async {
    return (await StorageHelper.listDirectory(recordDirectory, 1, true, false, true, false))
      .where((it) => RegExp(r'^\d{4,4}(_backup)?$').hasMatch(it.name()!))
      .toList();
  }

  static Future<GameRecordState> detectState(
    StoragePath recordDirectory,
    Uint8List?  key,
  ) async {
    var state = GameRecordState.invalid;
    var itemList = await GameRecordHelper.listFile(recordDirectory);
    if (itemList.isEmpty) {
      state = .none;
    }
    else if (itemList.firstWhereOrNull((it) => it.name()! == '0000') != null) {
      var itemFile = recordDirectory.join('0000');
      var itemData = await StorageHelper.readFileLimited(itemFile, 8);
      if (itemData.length == 8) {
        if (itemData.buffer.asUint32List().first == 0x00000000) {
          state = .decrypted;
        }
        else if (key != null) {
          GameRecordHelper.encryptData(itemData, key);
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
      assertTest(!key.isEmpty);
      for (var index = 0; index < data.length; index++) {
        data[index] ^= key[index % key.length];
      }
    }
    return;
  }

  static Future<Void> encryptFile(
    StoragePath sourceFile,
    StoragePath destinationFile,
    Uint8List?  key,
  ) async {
    var data = await StorageHelper.readFile(sourceFile);
    GameRecordHelper.encryptData(data, key);
    if (!await StorageHelper.existFile(destinationFile)) {
      await StorageHelper.createFile(destinationFile);
    }
    await StorageHelper.writeFile(destinationFile, data);
    return;
  }

  static Future<Void> encrypt(
    StoragePath recordDirectory,
    Uint8List?  key,
  ) async {
    var itemList = await GameRecordHelper.listFile(recordDirectory);
    for (var item in itemList) {
      await GameRecordHelper.encryptFile(recordDirectory.push(item), recordDirectory.push(item), key);
    }
    return;
  }

  // #endregion

  // #region archive

  static Future<Void> export(
    StoragePath targetDirectory,
    StoragePath archiveFile,
    Uint8List?  key,
  ) async {
    var archiveDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(archiveDirectory);
    // TODO
    await StorageHelper.createFile(archiveDirectory.join('configuration'));
    await StorageHelper.writeFileText(archiveDirectory.join('configuration'), '');
    await StorageHelper.createDirectory(archiveDirectory.join('data'));
    for (var dataFile in await GameRecordHelper.listFile(targetDirectory)) {
      await GameRecordHelper.encryptFile(targetDirectory.push(dataFile), archiveDirectory.join('data').push(dataFile), key);
    }
    if (await StorageHelper.exist(archiveFile)) {
      await StorageHelper.remove(archiveFile);
    }
    var archive = lib.createArchiveFromDirectory(.new(archiveDirectory.emit()));
    var archiveData = lib.ZipEncoder().encodeBytes(archive, level: lib.DeflateLevel.bestCompression);
    if (!await StorageHelper.existFile(archiveFile)) {
      await StorageHelper.createFile(archiveFile);
    }
    await StorageHelper.writeFile(archiveFile, archiveData);
    await StorageHelper.remove(archiveDirectory);
    return;
  }

  static Future<Void> import(
    StoragePath targetDirectory,
    StoragePath archiveFile,
    Uint8List?  key,
  ) async {
    var archiveDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(archiveDirectory);
    var archiveData = await StorageHelper.readFile(archiveFile);
    var archive = lib.ZipDecoder().decodeBytes(archiveData);
    await lib.extractArchiveToDisk(archive, archiveDirectory.emit());
    if (await StorageHelper.exist(targetDirectory)) {
      await StorageHelper.remove(targetDirectory);
    }
    // TODO
    var configuration = StorageHelper.readFileText(archiveDirectory.join('configuration'));
    await StorageHelper.createDirectory(targetDirectory);
    for (var dataFile in await GameRecordHelper.listFile(archiveDirectory.join('data'))) {
      await GameRecordHelper.encryptFile(archiveDirectory.join('configuration').push(dataFile), targetDirectory.push(dataFile), key);
    }
    await StorageHelper.remove(archiveDirectory);
    return;
  }

  // #endregion

}

class GameRepositoryHelper {

  // #region common

  static Future<Image?> extractProgramIcon(
    StoragePath programFile,
  ) async {
    var result = null as Image?;
    var hIconPath = programFile.emitNative().toNativeUtf16();
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

  static Future<({GameProgramState program, GameRecordState record})> checkGameState(
    StoragePath gameDirectory,
    String?     version,
    String      user,
  ) async {
    var programState = GameProgramState.none;
    var recordState = GameRecordState.none;
    if (await StorageHelper.existFile(gameDirectory.join('KairoGames.exe'))) {
      programState = !await StorageHelper.existFile(gameDirectory.join('KairoGames.exe.${version ?? '0'}.bak'))
        ? .original
        : .modified;
    }
    if (await StorageHelper.existDirectory(gameDirectory.join('saves').join('${user}'))) {
      recordState = await GameRecordHelper.detectState(gameDirectory.join('saves').join('${user}'), GameRepositoryHelper.makeKeyFromSteamUser(user));
    }
    return (program: programState, record: recordState);
  }

  // #endregion

  // #region custom

  static Future<GameInformation?> loadCustomGame(
    StoragePath gameDirectory,
  ) async {
    if (!await StorageHelper.existFile(gameDirectory.join('KairoGames.exe'))) {
      return null;
    }
    var information = GameInformation();
    information.path = gameDirectory;
    information.library = null;
    information.identifier = null;
    information.version = null;
    information.name = gameDirectory.name()!;
    information.icon = await GameRepositoryHelper.extractProgramIcon(gameDirectory.join('KairoGames.exe'));
    information.key = '0';
    if (!await StorageHelper.existDirectory(gameDirectory.join('saves'))) {
      information.key = (await StorageHelper.listDirectory(gameDirectory.join('saves'), 1, true, false, false, true)).firstWhereOrNull((it) => RegExp(r'^\d+$').hasMatch(it.name()!))?.name()! ?? '0';
    }
    var gameState = await GameRepositoryHelper.checkGameState(gameDirectory, information.version, information.key!);
    information.program = gameState.program;
    information.record = gameState.record;
    return information;
  }

  static Future<Boolean> checkCustomRepository(
    StoragePath repositoryDirectory,
  ) async {
    return await StorageHelper.existDirectory(repositoryDirectory);
  }

  static Future<List<GameInformation>> loadCustomRepository(
    StoragePath repositoryDirectory,
  ) async {
    var libraryList = await StorageHelper.listDirectory(repositoryDirectory, 1, true, false, false, true);
    var result = <GameInformation>[];
    for (var library in libraryList) {
      var libraryDirectory = repositoryDirectory.push(library);
      var gameConfiguration = await GameRepositoryHelper.loadCustomGame(libraryDirectory);
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
    StoragePath libraryDirectory,
    String      gameIdentifier,
  ) async {
    var gameManifestFile = libraryDirectory.join('steamapps').join('appmanifest_${gameIdentifier}.acf');
    if (!await StorageHelper.existFile(gameManifestFile)) {
      return null;
    }
    var gameManifest = await VdfHelper.decodeFile(gameManifestFile);
    assertTest(gameManifest.length == 1);
    assertTest(gameManifest.entries.first.key == 'AppState');
    assertTest(gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['appid']!.as<Integer>().toString() == gameIdentifier);
    var gameDirectory = libraryDirectory.join('steamapps').join('common').join(gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['installdir']!.as<String>());
    if (!await StorageHelper.existFile(gameDirectory.join('KairoGames.exe'))) {
      return null;
    }
    var information = GameInformation();
    information.path = gameDirectory;
    information.library = libraryDirectory;
    information.identifier = gameIdentifier;
    information.version = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['buildid']!.as<Integer>().toString();
    information.name = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['name']!.as<String>();
    information.icon = await GameRepositoryHelper.extractProgramIcon(gameDirectory.join('KairoGames.exe'));
    information.key = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['LastOwner']!.as<Integer>().toString();
    var gameState = await GameRepositoryHelper.checkGameState(gameDirectory, information.version, information.key!);
    information.program = gameState.program;
    information.record = gameState.record;
    return information;
  }

  static Future<Boolean> checkSteamRepository(
    StoragePath repositoryDirectory,
  ) async {
    return await StorageHelper.existFile(repositoryDirectory.join('steam.exe'));
  }

  static Future<List<GameInformation>> loadSteamRepository(
    StoragePath repositoryDirectory,
  ) async {
    var libraryList = await VdfHelper.decodeFile(repositoryDirectory.join('steamapps').join('libraryfolders.vdf'));
    assertTest(libraryList.length == 1);
    assertTest(libraryList.entries.first.key == 'libraryfolders');
    var result = <GameInformation>[];
    for (var library in libraryList['libraryfolders']!.as<Map<Object?, Object?>>().entries) {
      var libraryDirectory = StoragePath.of(library.value!.as<Map<Object?, Object?>>()['path']!.as<String>().replaceAll('\\\\', '/'));
      for (var game in library.value!.as<Map<Object?, Object?>>()['apps']!.as<Map<Object?, Object?>>().entries) {
        var gameIdentifier = game.key!.as<String>();
        var gameConfiguration = await GameRepositoryHelper.loadSteamGame(libraryDirectory, gameIdentifier);
        if (gameConfiguration != null) {
          result.add(gameConfiguration);
        }
      }
    }
    return result;
  }

  // #endregion

}
