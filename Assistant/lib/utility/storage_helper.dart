import '/common.dart';
import '/utility/storage_path.dart';
import '/utility/convert_helper.dart';
import '/utility/application_platform_method.dart';
import 'dart:io';
import 'dart:typed_data';
import 'package:ffi/ffi.dart' as lib;
import 'package:win32/win32.dart' as lib;

// ----------------

enum StorageQueryType {
  userHome,
  applicationShared,
  applicationTemporary,
}

enum StoragePickType {
  loadFile,
  loadDirectory,
  saveFile,
}

// ----------------

class StorageHelper {

  // #region basic

  static Future<Boolean> exist(
    StoragePath target,
  ) async {
    if (target.type() != .absolute) {
      return false;
    }
    var targetString = target.emitNative();
    return await FileSystemEntity.type(targetString, followLinks: false) != .notFound;
  }

  static Future<Void> copy(
    StoragePath target,
    StoragePath placement,
    Boolean     followLink,
  ) async {
    assertTest(await StorageHelper.exist(target));
    assertTest(!await StorageHelper.exist(placement));
    var placementParent = placement.parent();
    if (placementParent != null && !await StorageHelper.existDirectory(placementParent)) {
      await StorageHelper.createDirectory(placementParent);
    }
    var targetString = target.emitNative();
    var type = await FileSystemEntity.type(targetString, followLinks: false);
    if (followLink && type == .link) {
      var referentType = await FileSystemEntity.type(targetString, followLinks: true);
      if (referentType != .notFound) {
        type = referentType;
      }
    }
    if (type == .link) {
      var referent = await StorageHelper.resolveLink(target);
      var isDirectory = false;
      if (SystemChecker.isWindows) {
        var hTarget = targetString.toNativeUtf16();
        var attribute = lib.GetFileAttributes(hTarget);
        lib.calloc.free(hTarget);
        isDirectory = attribute != -1 && (attribute & lib.FILE_ATTRIBUTE_DIRECTORY) != 0;
      }
      if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
        isDirectory = false;
      }
      await StorageHelper.createLink(placement, referent, isDirectory);
    }
    else if (type == .file) {
      var data = await StorageHelper.readFile(target);
      await StorageHelper.createFile(placement);
      await StorageHelper.writeFile(placement, data);
    }
    else if (type == .directory) {
      await StorageHelper.createDirectory(placement);
      await for (var item in Directory(targetString).list(recursive: false, followLinks: false)) {
        var itemName = item.path.substring(item.path.lastIndexOf(r'[\/]'));
        await StorageHelper.copy(target.join(itemName), placement.join(itemName), followLink);
      }
    }
    else {
      throw UnsupportedException();
    }
    return;
  }

  static Future<Void> rename(
    StoragePath target,
    StoragePath placement,
  ) async {
    assertTest(await StorageHelper.exist(target));
    assertTest(!await StorageHelper.exist(placement));
    var placementParent = placement.parent();
    if (placementParent != null && !await StorageHelper.existDirectory(placementParent)) {
      await StorageHelper.createDirectory(placementParent);
    }
    var targetString = target.emitNative();
    var placementString = placement.emitNative();
    var type = await FileSystemEntity.type(targetString, followLinks: false);
    if (type == .link) {
      await Link(targetString).rename(placementString);
    }
    else if (type == .file) {
      await File(targetString).rename(placementString);
    }
    else if (type == .directory) {
      await Directory(targetString).rename(placementString);
    }
    else {
      throw UnsupportedException();
    }
    return;
  }

  static Future<Void> remove(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.exist(target));
    var targetString = target.emitNative();
    var type = await FileSystemEntity.type(targetString, followLinks: false);
    if (type == .link) {
      await Link(targetString).delete();
    }
    else if (type == .file) {
      await File(targetString).delete();
    }
    else if (type == .directory) {
      await Directory(targetString).delete(recursive: true);
    }
    else {
      throw UnsupportedException();
    }
    return;
  }

  // #endregion

  // #region link

  static Future<Boolean> existLink(
    StoragePath target,
  ) async {
    if (target.type() != .absolute) {
      return false;
    }
    var targetString = target.emitNative();
    return await FileSystemEntity.type(targetString, followLinks: false) == .link;
  }

  static Future<Void> createLink(
    StoragePath target,
    StoragePath referent,
    Boolean     isDirectory,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    var targetParent = target.parent();
    if (targetParent != null && !await StorageHelper.existDirectory(targetParent)) {
      await StorageHelper.createDirectory(targetParent);
    }
    var targetString = target.emitNative();
    var referentString = referent.emitNative();
    if (SystemChecker.isWindows) {
      var hTarget = targetString.toNativeUtf16();
      var hReferent = referentString.toNativeUtf16();
      var result = lib.CreateSymbolicLink(hTarget, hReferent, lib.SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE | (!isDirectory ? 0 : lib.SYMBOLIC_LINK_FLAG_DIRECTORY));
      lib.calloc.free(hTarget);
      lib.calloc.free(hReferent);
      assertTest(result != 0);
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
      await Link(targetString).create(referentString, recursive: false);
    }
    return;
  }

  // ----------------

  static Future<StoragePath> resolveLink(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.existLink(target));
    var targetString = target.emitNative();
    return .of(await Link(targetString).target());
  }

  // #endregion

  // #region file

  static Future<Boolean> existFile(
    StoragePath target,
  ) async {
    if (target.type() != .absolute) {
      return false;
    }
    var targetString = target.emitNative();
    return await FileSystemEntity.type(targetString, followLinks: true) == .file;
  }

  // TODO: remove?
  static Boolean existFileSync(
    StoragePath target,
  ) {
    if (target.type() != .absolute) {
      return false;
    }
    var targetString = target.emitNative();
    return FileSystemEntity.typeSync(targetString, followLinks: true) == .file;
  }

  static Future<Void> createFile(
    StoragePath target,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    var targetParent = target.parent();
    if (targetParent != null && !await StorageHelper.existDirectory(targetParent)) {
      await StorageHelper.createDirectory(targetParent);
    }
    var targetString = target.emitNative();
    await File(targetString).create(recursive: false);
    return;
  }

  // ----------------

  static Future<Integer> sizeFile(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    var targetString = target.emitNative();
    return await File(targetString).length();
  }

  // ----------------

  static Future<Uint8List> readFile(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    var targetString = target.emitNative();
    return await File(targetString).readAsBytes();
  }

  static Future<Void> writeFile(
    StoragePath target,
    Uint8List   data,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    var targetString = target.emitNative();
    await File(targetString).writeAsBytes(data, mode: .writeOnly, flush: true);
    return;
  }

  // ----------------

  static Future<String> readFileText(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    var targetString = target.emitNative();
    return await File(targetString).readAsString();
  }

  static Future<Void> writeFileText(
    StoragePath target,
    String      text,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    var targetString = target.emitNative();
    await File(targetString).writeAsString(text, mode: .writeOnly, flush: true);
    return;
  }

  // #endregion

  // #region directory

  static Future<Boolean> existDirectory(
    StoragePath target,
  ) async {
    if (target.type() != .absolute) {
      return false;
    }
    var targetString = target.emitNative();
    return await FileSystemEntity.type(targetString, followLinks: true) == .directory;
  }

  // TODO: remove?
  static Boolean existDirectorySync(
    StoragePath target,
  ) {
    if (target.type() != .absolute) {
      return false;
    }
    var targetString = target.emitNative();
    return FileSystemEntity.typeSync(targetString, followLinks: true) == .directory;
  }

  static Future<Void> createDirectory(
    StoragePath target,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    var targetString = target.emitNative();
    await Directory(targetString).create(recursive: true);
  }

  // ----------------

  static Future<List<StoragePath>> listDirectory(
    StoragePath target,
    Integer?    depth,
    Boolean     followLink,
    Boolean     allowLink,
    Boolean     allowFile,
    Boolean     allowDirectory,
  ) async {
    assertTest(await StorageHelper.existDirectory(target));
    var result = <StoragePath>[];
    var iterate = (
      Object      self,
      StoragePath currentTarget,
      StoragePath currentItem,
      Integer     currentDepth,
    ) async {
      if (depth == null || currentDepth < depth) {
        var currentTargetString = currentTarget.emitNative();
        await for (var item in Directory(currentTargetString).list(recursive: false, followLinks: false)) {
          var itemType = await FileSystemEntity.type(item.path, followLinks: false);
          var itemName = item.path.substring(item.path.lastIndexOf(RegExp(r'[/\\]')));
          var itemPath = currentItem.join(itemName);
          if (followLink && itemType == .link) {
            var referentType = await FileSystemEntity.type(item.path, followLinks: true);
            if (referentType != .notFound) {
              itemType = referentType;
            }
          }
          if (allowLink && itemType == .link) {
            result.add(itemPath);
          }
          if (allowFile && itemType == .file) {
            result.add(itemPath);
          }
          if (allowDirectory && itemType == .directory) {
            result.add(itemPath);
          }
          if (itemType == .directory) {
            await self.as<Future<Void> Function(Object, StoragePath, StoragePath, Integer)>()(self, currentTarget.join(itemName), itemPath, currentDepth + 1);
          }
        }
      }
      return null as Void;
    };
    await iterate(iterate, target, .by(.detached), 0);
    return result;
  }

  // #endregion

  // #region shell

  static Future<StoragePath> query(
    StorageQueryType type,
  ) async {
    return .of((await ApplicationPlatformMethod.instance.queryStorageItem(ConvertHelper.makeEnumerationToStringOfSnakeCase(type))).target);
  }

  static Future<Void> reveal(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.exist(target));
    var targetString = target.emitNative();
    await ApplicationPlatformMethod.instance.revealStorageItem(targetString);
    return;
  }

  static Future<List<StoragePath>> pick(
    StoragePickType type,
    Boolean         multiply,
    StoragePath?    location,
    String?         name,
  ) async {
    if (location == null || !await StorageHelper.existDirectory(location)) {
      location = await StorageHelper.query(.userHome);
    }
    if (name == null) {
      name = '';
    }
    var targetString = (await ApplicationPlatformMethod.instance.pickStorageItem(ConvertHelper.makeEnumerationToStringOfSnakeCase(type), multiply, location.emitNative(), name)).target;
    var target = targetString.map((it) => StoragePath.of(it)).toList();
    return target;
  }

  // ----------------

  static Future<StoragePath> temporary(
  ) async {
    var parent = await StorageHelper.query(.applicationTemporary);
    var name = DateTime.now().millisecondsSinceEpoch.toString();
    var target = parent.join(name);
    var suffix = 0;
    while (await exist(target)) {
      suffix += 1;
      target = parent.join('${name}.${suffix}');
    }
    return target;
  }

  // #endregion

}
