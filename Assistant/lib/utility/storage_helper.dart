import '/common.dart';
import '/setting.dart';
import '/utility/storage_path.dart';
import '/utility/application_platform_method.dart';
import '/widget/export.dart';
import 'dart:io';
import 'dart:typed_data';
import 'package:ffi/ffi.dart' as lib;
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';
import 'package:path/path.dart' as lib;
import 'package:path_provider/path_provider.dart' as lib;
import 'package:file_selector/file_selector.dart' as lib;
import 'package:url_launcher/url_launcher.dart' as lib;
import 'package:win32/win32.dart' as lib;

// ----------------

class StorageHelper {

  // #region basic

  static Future<Boolean> exist(
    StoragePath target,
  ) async {
    if (target.type() == .nothing) {
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
        var itemName = lib.basename(item.path);
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
    if (target.type() == .nothing) {
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
    if (target.type() == .nothing) {
      return false;
    }
    var targetString = target.emitNative();
    return await FileSystemEntity.type(targetString, followLinks: true) == .file;
  }

  // TODO: remove
  static Boolean existFileSync(
    StoragePath target,
  ) {
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
    if (target.type() == .nothing) {
      return false;
    }
    var targetString = target.emitNative();
    return await FileSystemEntity.type(targetString, followLinks: true) == .directory;
  }

  // TODO: remove
  static Boolean existDirectorySync(
    StoragePath target,
  ) {
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
          var itemName = lib.basename(item.path);
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
    await iterate(iterate, target, .relative(), 0);
    return result;
  }

  // #endregion

  // #region shell

  static Future<StoragePath> temporary(
  ) async {
    var parent = await StorageHelper.queryApplicationCacheDirectory();
    var name = DateTime.now().millisecondsSinceEpoch.toString();
    var result = parent.join(name);
    var suffix = 0;
    while (await exist(result)) {
      suffix += 1;
      result = parent.join('${name}.${suffix}');
    }
    return result;
  }

  // ----------------

  static Future<Void> reveal(
    StoragePath target,
  ) async {
    assertTest(await StorageHelper.exist(target));
    var revealed = false;
    var targetString = target.emitNative();
    if (SystemChecker.isWindows || SystemChecker.isMacintosh || SystemChecker.isLinux) {
      revealed = await lib.launchUrl(.file(targetString), mode: .externalApplication);
    }
    if (SystemChecker.isAndroid) {
      throw UnsupportedException();
    }
    if (SystemChecker.isIphone) {
      revealed = await lib.launchUrl(.file(targetString).replace(scheme: 'shareddocuments'), mode: .externalApplication);
    }
    assertTest(revealed);
    return;
  }

  // ----------------

  static Future<StoragePath?> pick(
    String       type,
    BuildContext context,
    String?      location,
    String?      name,
  ) async {
    assertTest(type == 'load_file' || type == 'load_directory' || type == 'save_file');
    var targetString = null as String?;
    var target = null as StoragePath?;
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var locationTag = location == null ? null : !location.startsWith('@') ? null : location.substring(1);
    var locationPath = null as StoragePath?;
    if (locationTag != null) {
      locationPath = setting.data.storagePickerHistoryLocation[locationTag];
    }
    else {
      locationPath = location == null ? null : .of(location);
    }
    if (locationPath != null && !await StorageHelper.existDirectory(locationPath)) {
      locationPath = null;
    }
    name ??= '';
    if (SystemChecker.isWindows) {
      locationPath ??= .of('C:/');
      if (type == 'load_file') {
        targetString = (await lib.openFile(initialDirectory: locationPath.emitNative()))?.path;
      }
      if (type == 'load_directory') {
        targetString = (await lib.getDirectoryPath(initialDirectory: locationPath.emitNative()));
      }
      if (type == 'save_file') {
        targetString = (await lib.getSaveLocation(initialDirectory: locationPath.emitNative(), suggestedName: name))?.path;
      }
      target = (targetString == null || targetString == '') ? null : .of(targetString);
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh) {
      locationPath ??= .of('/');
      if (type == 'load_file') {
        targetString = (await lib.openFile(initialDirectory: locationPath.emitNative()))?.path;
      }
      if (type == 'load_directory') {
        targetString = (await lib.getDirectoryPath(initialDirectory: locationPath.emitNative()));
      }
      if (type == 'save_file') {
        targetString = (await lib.getSaveLocation(initialDirectory: locationPath.emitNative(), suggestedName: name))?.path;
      }
      target = (targetString == null || targetString == '') ? null : .of(targetString);
    }
    if (SystemChecker.isAndroid) {
      locationPath ??= .of((await ApplicationPlatformMethod.instance.queryExternalStoragePath()).path);
      if (type == 'load_file') {
        targetString = (await ApplicationPlatformMethod.instance.pickStorageItem('load_file', locationPath.emitNative(), name)).target;
      }
      if (type == 'load_directory') {
        targetString = (await ApplicationPlatformMethod.instance.pickStorageItem('load_directory', locationPath.emitNative(), name)).target;
      }
      if (type == 'save_file') {
        targetString = (await ApplicationPlatformMethod.instance.pickStorageItem('save_file', locationPath.emitNative(), name)).target;
      }
      target = (targetString == null || targetString == '') ? null : await StorageHelper.parseAndroidContentUri(context, Uri.parse(targetString), true);
    }
    if (SystemChecker.isIphone) {
      locationPath ??= await StorageHelper.queryApplicationSharedDirectory();
      if (type == 'load_file') {
        targetString = (await ApplicationPlatformMethod.instance.pickStorageItem('load_file', locationPath.emitNative(), name)).target;
      }
      if (type == 'load_directory') {
        targetString = (await ApplicationPlatformMethod.instance.pickStorageItem('load_directory', locationPath.emitNative(), name)).target;
      }
      if (type == 'save_file') {
        throw UnsupportedException();
      }
    }
    if (locationTag != null && target != null) {
      setting.data.storagePickerHistoryLocation[locationTag] = switch (type) {
        'load_file'      => target.parent()!,
        'load_directory' => target,
        'save_file'      => target.parent()!,
        _                => throw UnreachableException(),
      };
      await setting.save(apply: false);
    }
    return target;
  }

  static Future<StoragePath?> pickLoadFile(
    BuildContext context,
    String?      location,
  ) async {
    return StorageHelper.pick('load_file', context, location, null);
  }

  static Future<StoragePath?> pickLoadDirectory(
    BuildContext context,
    String?      location,
  ) async {
    return StorageHelper.pick('load_directory', context, location, null);
  }

  static Future<StoragePath?> pickSaveFile(
    BuildContext context,
    String?      location,
    String?      name,
  ) async {
    return StorageHelper.pick('save_file', context, location, name);
  }

  // #endregion

  // #region miscellaneous

  static Future<StoragePath> queryApplicationSharedDirectory(
  ) async {
    var result = null as StoragePath?;
    if (SystemChecker.isWindows) {
      result = .of((await lib.getApplicationSupportDirectory()).path);
    }
    if (SystemChecker.isLinux) {
      result = .of((await lib.getApplicationSupportDirectory()).path);
    }
    if (SystemChecker.isMacintosh) {
      result = .of((await lib.getApplicationSupportDirectory()).path);
    }
    if (SystemChecker.isAndroid) {
      result = .of((await lib.getExternalStorageDirectory())!.path);
    }
    if (SystemChecker.isIphone) {
      result = .of((await lib.getApplicationDocumentsDirectory()).path);
    }
    return result!;
  }

  static Future<StoragePath> queryApplicationCacheDirectory(
  ) async {
    var result = null as StoragePath?;
    if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isIphone) {
      result = (await StorageHelper.queryApplicationSharedDirectory()).join('cache');
    }
    if (SystemChecker.isAndroid) {
      result = .of((await lib.getApplicationCacheDirectory()).path);
    }
    return result!;
  }

  // ----------------

  // ----------------

  static Future<StoragePath?> parseAndroidContentUri(
    BuildContext context,
    Uri          uri,
    Boolean      copyable,
  ) async {
    var result = null as StoragePath?;
    assertTest(uri.scheme == 'content');
    var provider = uri.authority;
    var path = Uri.decodeComponent(uri.path);
    switch (provider) {
      // AOSP DocumentsUI
      case 'com.android.externalstorage.documents': {
        // /document/primary:<path-relative-external-storage>
        if (path.startsWith('/document/primary:')) {
          result = .of((await ApplicationPlatformMethod.instance.queryExternalStoragePath()).path).join(path.substring('/document/primary:'.length));
        }
        // /tree/primary:<path-relative-external-storage>
        if (path.startsWith('/tree/primary:')) {
          result = .of((await ApplicationPlatformMethod.instance.queryExternalStoragePath()).path).join(path.substring('/tree/primary:'.length));
        }
        break;
      }
      // Material Files
      case 'me.zhanghai.android.files.file_provider': {
        path = Uri.decodeComponent(path);
        // /file://<path-absolute>
        if (path.startsWith('/file://')) {
          result = .of(Uri.decodeComponent(Uri.parse(path.substring('/'.length)).path));
        }
        break;
      }
      // Root Explorer
      case 'com.speedsoftware.rootexplorer.fileprovider': {
        // /root/<path-relative-root>
        if (path.startsWith('/root/')) {
          result = .of(path.substring('/root'.length));
        }
        break;
      }
      // Solid Explorer
      case 'pl.solidexplorer2.files': {
        result = .of(path);
        break;
      }
      // MT Manager
      case 'bin.mt.plus.fp': {
        result = .of(path);
        break;
      }
      // NMM
      case 'in.mfile.files': {
        result = .of(path);
        break;
      }
      default: {
        if (path.startsWith('/') && await StorageHelper.exist(.of(path))) {
          result = .of(path);
        }
        break;
      }
    }
    if (result == null) {
      var canDuplicate = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
        title: 'Unknown Content Uri',
        contentBuilder: (context, setStateForPanel) => [
          FlexContainer.horizontal([
            StyledText.custom(
              uri.toString(),
              overflow: .clip,
            ).withSelectableArea(
            ).withFlexExpanded(),
          ]),
        ],
        actionBuilder: (context) => [
          StyledButton.text(
            content: StyledText.inherit('Ignore'),
            onPressed: (context) => Navigator.pop(context, false),
          ),
          StyledButton.text(
            enabled: copyable,
            content: StyledText.inherit('Duplicate'),
            onPressed: (context) => Navigator.pop(context, true),
          ),
        ],
      )) ?? false;
      if (canDuplicate) {
        var setting = Provider.of<SettingProvider>(context, listen: false);
        result = .of((await ApplicationPlatformMethod.instance.copyStorageFile(uri.toString(), setting.data.storagePickerFallbackDirectory.emitNative())).destination);
      }
    }
    return result;
  }

  // #endregion

}
