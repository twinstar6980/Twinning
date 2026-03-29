import '/common.dart';
import '/setting.dart';
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

  // #region path

  static String regularize(
    String path,
  ) {
    return path.replaceAll('\\', '/');
  }

  static String nativize(
    String path,
  ) {
    var result = path;
    if (SystemChecker.isWindows) {
      result = path.replaceAll('/', '\\');
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
      result = path.replaceAll('\\', '/');
    }
    return result;
  }

  // ----------------

  static String parent(
    String path,
  ) {
    return StorageHelper.regularize(lib.dirname(path));
  }

  static String name(
    String path,
  ) {
    return StorageHelper.regularize(lib.basename(path));
  }

  // ----------------

  static Future<String> temporary(
  ) async {
    var parent = await StorageHelper.queryApplicationCacheDirectory();
    var name = DateTime.now().millisecondsSinceEpoch.toString();
    var result = '${parent}/${name}';
    var suffix = 0;
    while (await exist(result)) {
      suffix += 1;
      result = '${parent}/${name}.${suffix}';
    }
    return result;
  }

  // #endregion

  // #region basic

  static Future<Boolean> exist(
    String target,
  ) async {
    if (target.isEmpty) {
      return false;
    }
    return await FileSystemEntity.type(target, followLinks: false) != .notFound;
  }

  static Future<Void> copy(
    String  target,
    String  placement,
    Boolean followLink,
  ) async {
    assertTest(await StorageHelper.exist(target));
    assertTest(!await StorageHelper.exist(placement));
    if (!await StorageHelper.existDirectory(StorageHelper.parent(placement))) {
      await StorageHelper.createDirectory(StorageHelper.parent(placement));
    }
    var type = await FileSystemEntity.type(target, followLinks: false);
    if (followLink && type == .link) {
      var referentType = await FileSystemEntity.type(target, followLinks: true);
      if (referentType != .notFound) {
        type = referentType;
      }
    }
    if (type == .link) {
      var referent = await StorageHelper.resolveLink(target);
      var isDirectory = false;
      if (SystemChecker.isWindows) {
        var hTarget = target.toNativeUtf16();
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
      await for (var item in Directory(target).list(recursive: false, followLinks: false)) {
        var itemName = StorageHelper.name(item.path);
        await StorageHelper.copy(item.path, '${placement}/${itemName}', followLink);
      }
    }
    else {
      throw UnsupportedException();
    }
    return;
  }

  static Future<Void> rename(
    String target,
    String placement,
  ) async {
    assertTest(await StorageHelper.exist(target));
    assertTest(!await StorageHelper.exist(placement));
    if (!await StorageHelper.existDirectory(StorageHelper.parent(placement))) {
      await StorageHelper.createDirectory(StorageHelper.parent(placement));
    }
    var type = await FileSystemEntity.type(target, followLinks: false);
    if (type == .link) {
      await Link(target).rename(placement);
    }
    else if (type == .file) {
      await File(target).rename(placement);
    }
    else if (type == .directory) {
      await Directory(target).rename(placement);
    }
    else {
      throw UnsupportedException();
    }
    return;
  }

  static Future<Void> remove(
    String target,
  ) async {
    assertTest(await StorageHelper.exist(target));
    var type = await FileSystemEntity.type(target, followLinks: false);
    if (type == .link) {
      await Link(target).delete();
    }
    else if (type == .file) {
      await File(target).delete();
    }
    else if (type == .directory) {
      await Directory(target).delete(recursive: true);
    }
    else {
      throw UnsupportedException();
    }
    return;
  }

  // #endregion

  // #region link

  static Future<Boolean> existLink(
    String target,
  ) async {
    if (target.isEmpty) {
      return false;
    }
    return await FileSystemEntity.type(target, followLinks: false) == .link;
  }

  static Future<Void> createLink(
    String  target,
    String  referent,
    Boolean isDirectory,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    if (!await StorageHelper.existDirectory(StorageHelper.parent(target))) {
      await StorageHelper.createDirectory(StorageHelper.parent(target));
    }
    if (SystemChecker.isWindows) {
      var hTarget = target.toNativeUtf16();
      var hReferent = StorageHelper.nativize(referent).toNativeUtf16();
      var result = lib.CreateSymbolicLink(hTarget, hReferent, lib.SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE | (!isDirectory ? 0 : lib.SYMBOLIC_LINK_FLAG_DIRECTORY));
      lib.calloc.free(hTarget);
      lib.calloc.free(hReferent);
      assertTest(result != 0);
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
      await Link(target).create(StorageHelper.nativize(referent), recursive: false);
    }
    return;
  }

  // ----------------

  static Future<String> resolveLink(
    String target,
  ) async {
    assertTest(await StorageHelper.existLink(target));
    return StorageHelper.regularize(await Link(target).target());
  }

  // #endregion

  // #region file

  static Future<Boolean> existFile(
    String target,
  ) async {
    if (target.isEmpty) {
      return false;
    }
    return await FileSystemEntity.type(target, followLinks: true) == .file;
  }

  // TODO: remove
  static Boolean existFileSync(
    String target,
  ) {
    return FileSystemEntity.typeSync(target, followLinks: true) == .file;
  }

  static Future<Void> createFile(
    String target,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    if (!await StorageHelper.existDirectory(StorageHelper.parent(target))) {
      await StorageHelper.createDirectory(StorageHelper.parent(target));
    }
    await File(target).create(recursive: false);
    return;
  }

  // ----------------

  static Future<Integer> sizeFile(
    String target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    return await File(target).length();
  }

  // ----------------

  static Future<Uint8List> readFile(
    String target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    return await File(target).readAsBytes();
  }

  static Future<Void> writeFile(
    String    target,
    Uint8List data,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    await File(target).writeAsBytes(data, mode: .writeOnly, flush: true);
    return;
  }

  // ----------------

  static Future<String> readFileText(
    String target,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    return await File(target).readAsString();
  }

  static Future<Void> writeFileText(
    String target,
    String text,
  ) async {
    assertTest(await StorageHelper.existFile(target));
    await File(target).writeAsString(text, mode: .writeOnly, flush: true);
    return;
  }

  // #endregion

  // #region directory

  static Future<Boolean> existDirectory(
    String target,
  ) async {
    if (target.isEmpty) {
      return false;
    }
    return await FileSystemEntity.type(target, followLinks: true) == .directory;
  }

  // TODO: remove
  static Boolean existDirectorySync(
    String target,
  ) {
    return FileSystemEntity.typeSync(target, followLinks: true) == .directory;
  }

  static Future<Void> createDirectory(
    String target,
  ) async {
    assertTest(!await StorageHelper.exist(target));
    await Directory(target).create(recursive: true);
  }

  // ----------------

  static Future<List<String>> listDirectory(
    String   target,
    Integer? depth,
    Boolean  followLink,
    Boolean  allowLink,
    Boolean  allowFile,
    Boolean  allowDirectory,
  ) async {
    assertTest(await StorageHelper.existDirectory(target));
    var result = <String>[];
    Future<Void> iterate(
      String  currentTarget,
      String  currentItem,
      Integer currentDepth,
    ) async {
      if (depth == null || currentDepth < depth) {
        await for (var item in Directory(currentTarget).list(recursive: false, followLinks: false)) {
          var itemType = await FileSystemEntity.type(item.path, followLinks: false);
          var itemName = StorageHelper.name(item.path);
          var itemPath = '${currentItem.isEmpty ? '' : '${currentItem}/'}${itemName}';
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
            await iterate('${currentTarget}/${itemName}', itemPath, currentDepth + 1);
          }
        }
      }
      return;
    }
    await iterate(target, '', 0);
    return result;
  }

  // #endregion

  // #region shell

  static Future<Void> reveal(
    String target,
  ) async {
    assertTest(await StorageHelper.exist(target));
    var revealed = false;
    if (SystemChecker.isWindows || SystemChecker.isMacintosh || SystemChecker.isLinux) {
      revealed = await lib.launchUrl(.file(target), mode: .externalApplication);
    }
    if (SystemChecker.isAndroid) {
      throw UnsupportedException();
    }
    if (SystemChecker.isIphone) {
      revealed = await lib.launchUrl(.file(target).replace(scheme: 'shareddocuments'), mode: .externalApplication);
    }
    assertTest(revealed);
    return;
  }

  // ----------------

  static Future<String?> pick(
    String       type,
    BuildContext context,
    String?      location,
    String?      name,
  ) async {
    assertTest(type == 'load_file' || type == 'load_directory' || type == 'save_file');
    var target = null as String?;
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var locationTag = location == null ? null : !location.startsWith('@') ? null : location.substring(1);
    var locationPath = location;
    if (locationTag != null) {
      locationPath = setting.data.storagePickerHistoryLocation[locationTag];
    }
    if (locationPath != null && !await StorageHelper.existDirectory(locationPath)) {
      locationPath = null;
    }
    name ??= '';
    if (SystemChecker.isWindows) {
      locationPath ??= 'C:/';
      if (type == 'load_file') {
        target = (await lib.openFile(initialDirectory: StorageHelper.nativize(locationPath)))?.path;
      }
      if (type == 'load_directory') {
        target = await lib.getDirectoryPath(initialDirectory: StorageHelper.nativize(locationPath));
      }
      if (type == 'save_file') {
        target = (await lib.getSaveLocation(initialDirectory: StorageHelper.nativize(locationPath), suggestedName: name))?.path;
      }
      if (target != null) {
        target = StorageHelper.regularize(target);
      }
      if (target == '') {
        target = null;
      }
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh) {
      locationPath ??= '/';
      if (type == 'load_file') {
        target = (await lib.openFile(initialDirectory: locationPath))?.path;
      }
      if (type == 'load_directory') {
        target = await lib.getDirectoryPath(initialDirectory: locationPath);
      }
      if (type == 'save_file') {
        target = (await lib.getSaveLocation(initialDirectory: locationPath, suggestedName: name))?.path;
      }
    }
    if (SystemChecker.isAndroid) {
      locationPath ??= (await ApplicationPlatformMethod.instance.queryExternalStoragePath()).path;
      if (type == 'load_file') {
        target = (await ApplicationPlatformMethod.instance.pickStorageItem('load_file', locationPath, name)).target;
      }
      if (type == 'load_directory') {
        target = (await ApplicationPlatformMethod.instance.pickStorageItem('load_directory', locationPath, name)).target;
      }
      if (type == 'save_file') {
        target = (await ApplicationPlatformMethod.instance.pickStorageItem('save_file', locationPath, name)).target;
      }
      if (target != null) {
        target = await StorageHelper.parseAndroidContentUri(context, Uri.parse(target), true);
      }
    }
    if (SystemChecker.isIphone) {
      locationPath ??= await StorageHelper.queryApplicationSharedDirectory();
      if (type == 'load_file') {
        target = (await ApplicationPlatformMethod.instance.pickStorageItem('load_file', locationPath, name)).target;
      }
      if (type == 'load_directory') {
        target = (await ApplicationPlatformMethod.instance.pickStorageItem('load_directory', locationPath, name)).target;
      }
      if (type == 'save_file') {
        throw UnsupportedException();
      }
    }
    if (locationTag != null && target != null) {
      setting.data.storagePickerHistoryLocation[locationTag] = switch (type) {
        'load_file'      => StorageHelper.parent(target),
        'load_directory' => target,
        'save_file'      => StorageHelper.parent(target),
        _                => throw UnreachableException(),
      };
      await setting.save(apply: false);
    }
    return target;
  }

  static Future<String?> pickLoadFile(
    BuildContext context,
    String?      location,
  ) async {
    return StorageHelper.pick('load_file', context, location, null);
  }

  static Future<String?> pickLoadDirectory(
    BuildContext context,
    String?      location,
  ) async {
    return StorageHelper.pick('load_directory', context, location, null);
  }

  static Future<String?> pickSaveFile(
    BuildContext context,
    String?      location,
    String?      name,
  ) async {
    return StorageHelper.pick('save_file', context, location, name);
  }

  // #endregion

  // #region miscellaneous

  static Future<String> queryApplicationSharedDirectory(
  ) async {
    var result = null as String?;
    if (SystemChecker.isWindows) {
      result = (await lib.getApplicationSupportDirectory()).path;
      result = StorageHelper.regularize(result);
    }
    if (SystemChecker.isLinux) {
      result = (await lib.getApplicationSupportDirectory()).path;
    }
    if (SystemChecker.isMacintosh) {
      result = (await lib.getApplicationSupportDirectory()).path;
    }
    if (SystemChecker.isAndroid) {
      result = (await lib.getExternalStorageDirectory())!.path;
    }
    if (SystemChecker.isIphone) {
      result = (await lib.getApplicationDocumentsDirectory()).path;
    }
    return result!;
  }

  static Future<String> queryApplicationCacheDirectory(
  ) async {
    var result = null as String?;
    if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isIphone) {
      result = (await StorageHelper.queryApplicationSharedDirectory()) + '/cache';
    }
    if (SystemChecker.isAndroid) {
      result = (await lib.getApplicationCacheDirectory()).path;
    }
    return result!;
  }

  // ----------------

  static Future<String?> parseAndroidContentUri(
    BuildContext context,
    Uri          uri,
    Boolean      copyable,
  ) async {
    var result = null as String?;
    assertTest(uri.scheme == 'content');
    var provider = uri.authority;
    var path = Uri.decodeComponent(uri.path);
    switch (provider) {
      // AOSP DocumentsUI
      case 'com.android.externalstorage.documents': {
        // /document/primary:<path-relative-external-storage>
        if (path.startsWith('/document/primary:')) {
          result = path.substring('/document/primary:'.length);
          result = '${(await ApplicationPlatformMethod.instance.queryExternalStoragePath()).path}${result.isEmpty ? '' : '/'}${result}';
        }
        // /tree/primary:<path-relative-external-storage>
        if (path.startsWith('/tree/primary:')) {
          result = path.substring('/tree/primary:'.length);
          result = '${(await ApplicationPlatformMethod.instance.queryExternalStoragePath()).path}${result.isEmpty ? '' : '/'}${result}';
        }
        break;
      }
      // Material Files
      case 'me.zhanghai.android.files.file_provider': {
        path = Uri.decodeComponent(path);
        // /file://<path-absolute>
        if (path.startsWith('/file://')) {
          result = Uri.decodeComponent(Uri.parse(path.substring('/'.length)).path);
        }
        break;
      }
      // Root Explorer
      case 'com.speedsoftware.rootexplorer.fileprovider': {
        // /root/<path-relative-root>
        if (path.startsWith('/root/')) {
          result = path.substring('/root'.length);
        }
        break;
      }
      // Solid Explorer
      case 'pl.solidexplorer2.files': {
        result = path;
        break;
      }
      // MT Manager
      case 'bin.mt.plus.fp': {
        result = path;
        break;
      }
      // NMM
      case 'in.mfile.files': {
        result = path;
        break;
      }
      default: {
        if (path.startsWith('/') && await StorageHelper.exist(path)) {
          result = path;
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
        result = (await ApplicationPlatformMethod.instance.copyStorageFile(uri.toString(), setting.data.storagePickerFallbackDirectory)).destination;
      }
    }
    return result;
  }

  // #endregion

}
