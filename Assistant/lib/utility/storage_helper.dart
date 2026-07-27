import '/common.dart';
import '/utility/finalizer.dart';
import '/utility/storage_path.dart';
import '/utility/convert_helper.dart';
import '/utility/platform_integration_manager.dart';
import 'dart:io' as lib;
import 'dart:typed_data' as lib;
import 'dart:convert' as lib;
import 'package:ffi/ffi.dart' as lib;
import 'package:win32/win32.dart' as lib;

// ----------------

enum StorageQueryType {
  userHome,
  applicationShared,
  applicationPersistent,
  applicationTemporary,
  applicationCache,
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
    return await lib.FileSystemEntity.type(targetString, followLinks: false) != .notFound;
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
    var type = await lib.FileSystemEntity.type(targetString, followLinks: false);
    if (followLink && type == .link) {
      var referentType = await lib.FileSystemEntity.type(targetString, followLinks: true);
      if (referentType != .notFound) {
        type = referentType;
      }
    }
    if (type == .link) {
      var referent = await StorageHelper.resolveLink(target);
      var isDirectory = false;
      if (SystemChecker.isWindows) {
        var hTarget = targetString.toPcwstr();
        var attribute = lib.GetFileAttributes(hTarget);
        lib.calloc.free(hTarget);
        isDirectory = attribute.value != -1 && (attribute.value & lib.FILE_ATTRIBUTE_DIRECTORY) != 0;
      }
      if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
        isDirectory = false;
      }
      await StorageHelper.createLink(placement, referent, isDirectory);
    }
    else if (type == .file) {
      var data = await StorageHelper.readFileData(target);
      await StorageHelper.createFile(placement);
      await StorageHelper.writeFileData(placement, data);
    }
    else if (type == .directory) {
      await StorageHelper.createDirectory(placement);
      await for (var item in lib.Directory(targetString).list(recursive: false, followLinks: false)) {
        var itemName = item.path.substring(item.path.lastIndexOf(RegExp(r'[/\\]')) + 1);
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
    var type = await lib.FileSystemEntity.type(targetString, followLinks: false);
    if (type == .link) {
      await lib.Link(targetString).rename(placementString);
    }
    else if (type == .file) {
      await lib.File(targetString).rename(placementString);
    }
    else if (type == .directory) {
      await lib.Directory(targetString).rename(placementString);
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
    var type = await lib.FileSystemEntity.type(targetString, followLinks: false);
    if (type == .link) {
      await lib.Link(targetString).delete();
    }
    else if (type == .file) {
      await lib.File(targetString).delete();
    }
    else if (type == .directory) {
      await lib.Directory(targetString).delete(recursive: true);
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
    return await lib.FileSystemEntity.type(targetString, followLinks: false) == .link;
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
      var hTarget = targetString.toPcwstr();
      var hReferent = referentString.toPcwstr();
      var result = lib.CreateSymbolicLink(hTarget, hReferent, lib.SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE | (!isDirectory ? 0 : lib.SYMBOLIC_LINK_FLAG_DIRECTORY));
      lib.calloc.free(hTarget);
      lib.calloc.free(hReferent);
      assertTest(result.value);
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
      await lib.Link(targetString).create(referentString, recursive: false);
    }
    return;
  }

  // ----------------

  static Future<StoragePath> resolveLink(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.existLink(target));
    var targetString = target.emitNative();
    return .of(await lib.Link(targetString).target());
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
    return await lib.FileSystemEntity.type(targetString, followLinks: true) == .file;
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
    await lib.File(targetString).create(recursive: false);
    return;
  }

  // ----------------

  static Future<Integer> sizeFile(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    var targetString = target.emitNative();
    return await lib.File(targetString).length();
  }

  static Future<Void> resizeFile(
    StoragePath target,
    Integer     size,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    assertTest(size >= 0);
    var targetString = target.emitNative();
    var handler = await lib.File(targetString).open(mode: .writeOnlyAppend);
    try {
      await handler.truncate(size);
    }
    finally {
      await handler.close();
    }
    return;
  }

  // ----------------

  static Future<Void> readFile(
    StoragePath   target,
    Integer       offset,
    lib.Uint8List data,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    assertTest(offset >= 0);
    var targetString = target.emitNative();
    var handler = await lib.File(targetString).open(mode: .read);
    try {
      assertTest(await handler.length() >= offset + data.length);
      await handler.setPosition(offset);
      var count = await handler.readInto(data);
      assertTest(count == data.length);
    }
    finally {
      await handler.close();
    }
    return;
  }

  static Future<Void> writeFile(
    StoragePath   target,
    Integer       offset,
    lib.Uint8List data,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    assertTest(offset >= 0);
    var targetString = target.emitNative();
    var handler = await lib.File(targetString).open(mode: .writeOnlyAppend);
    try {
      assertTest(await handler.length() >= offset + data.length);
      await handler.setPosition(offset);
      await handler.writeFrom(data);
    }
    finally {
      await handler.close();
    }
    return;
  }

  // ----------------

  static Future<lib.Uint8List> readFileData(
    StoragePath target,
  ) async {
    var size = await StorageHelper.sizeFile(target);
    var data = lib.Uint8List(size);
    await StorageHelper.readFile(target, 0, data);
    return data;
  }

  static Future<Void> writeFileData(
    StoragePath   target,
    lib.Uint8List data,
  ) async {
    await StorageHelper.resizeFile(target, data.length);
    await StorageHelper.writeFile(target, 0, data);
    return;
  }

  static Future<String> readFileText(
    StoragePath target,
  ) async {
    var data = await StorageHelper.readFileData(target);
    var text = lib.utf8.decode(data);
    return text;
  }

  static Future<Void> writeFileText(
    StoragePath target,
    String      text,
  ) async {
    var data = lib.utf8.encode(text);
    await StorageHelper.writeFileData(target, data);
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
    return await lib.FileSystemEntity.type(targetString, followLinks: true) == .directory;
  }

  static Future<Void> createDirectory(
    StoragePath target,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    var targetString = target.emitNative();
    await lib.Directory(targetString).create(recursive: true);
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
        await for (var item in lib.Directory(currentTargetString).list(recursive: false, followLinks: false)) {
          var itemType = await lib.FileSystemEntity.type(item.path, followLinks: false);
          var itemName = item.path.substring(item.path.lastIndexOf(RegExp(r'[/\\]')) + 1);
          var itemPath = currentItem.join(itemName);
          if (followLink && itemType == .link) {
            var referentType = await lib.FileSystemEntity.type(item.path, followLinks: true);
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
    return (await PlatformIntegrationManager.instance.invokeQueryStorageItem(ConvertHelper.makeEnumerationToStringOfSnakeCase(type))).target;
  }

  static Future<Void> reveal(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.exist(target));
    await PlatformIntegrationManager.instance.invokeRevealStorageItem(target);
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
    var target = (await PlatformIntegrationManager.instance.invokePickStorageItem(ConvertHelper.makeEnumerationToStringOfSnakeCase(type), multiply, location, name)).target;
    return target;
  }

  // ----------------

  static Future<(StoragePath, Finalizer)> temporary({
    Boolean useCache = false,
  }) async {
    var parent = await StorageHelper.query(!useCache ? .applicationTemporary : .applicationCache);
    var name = ConvertHelper.makeDateTimeToString(DateTime.now());
    var target = parent.join(name);
    var suffix = 0;
    while (await exist(target)) {
      suffix += 1;
      target = parent.join('${name}.${suffix}');
    }
    {
      await StorageHelper.createDirectory(target);
    }
    var targetFinalizer = Finalizer(() async {
      await StorageHelper.remove(target);
    });
    return (target, targetFinalizer);
  }

  // #endregion

}
