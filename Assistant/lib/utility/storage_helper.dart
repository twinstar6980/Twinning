import '/common.dart';
import '/setting.dart';
import '/utility/control_helper.dart';
import '/utility/platform_method.dart';
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:path/path.dart' as p;
import 'package:file_selector/file_selector.dart' as file_selector;
import 'package:path_provider/path_provider.dart' as path_provider;

// ----------------

class StorageHelper {

  // #region path

  static String regularize(
    String path,
  ) {
    return toPosixStyle(path);
  }

  static String toPosixStyle(
    String path,
  ) {
    return path.replaceAll('\\', '/');
  }

  static String toWindowsStyle(
    String path,
  ) {
    return path.replaceAll('/', '\\');
  }

  // ----------------

  static String parent(
    String path,
  ) {
    return regularize(p.dirname(path));
  }

  static String name(
    String path,
  ) {
    return regularize(p.basename(path));
  }

  // ----------------

  static Future<String> temporary(
  ) async {
    var parent = await queryApplicationCacheDirectory();
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
    String path,
  ) async {
    return await File(path).exists() || await Directory(path).exists();
  }

  static Future<Boolean> existFile(
    String path,
  ) async {
    return await File(path).exists();
  }

  static Future<Boolean> existDirectory(
    String path,
  ) async {
    return await Directory(path).exists();
  }

  // ----------------

  static Boolean existSync(
    String path,
  ) {
    return File(path).existsSync() || Directory(path).existsSync();
  }

  static Boolean existFileSync(
    String path,
  ) {
    return File(path).existsSync();
  }

  static Boolean existDirectorySync(
    String path,
  ) {
    return Directory(path).existsSync();
  }

  // ----------------

  static Future<Void> copyFile(
    String source,
    String destination,
  ) async {
    var destinationParent = parent(destination);
    if (!await exist(destinationParent)) {
      await Directory(destinationParent).create(recursive: true);
    }
    await File(destination).writeAsBytes(await File(source).readAsBytes());
    return;
  }

  static Future<Void> copyDirectory(
    String source,
    String destination,
  ) async {
    // TODO
    throw UnimplementedError();
  }

  // ----------------

  static Future<Void> removeFile(
    String source,
  ) async {
    if (await File(source).exists()) {
      await File(source).delete(recursive: true);
    }
    return;
  }

  static Future<Void> removeDirectory(
    String source,
  ) async {
    if (await Directory(source).exists()) {
      await Directory(source).delete(recursive: true);
    }
    return;
  }

  // #endregion

  // #region file

  static Future<Uint8List> readFile(
    String target,
  ) async {
    return await File(target).readAsBytes();
  }

  static Future<Void> writeFile(
    String    target,
    Uint8List data,
  ) async {
    await File(target).create(recursive: true);
    await File(target).writeAsBytes(data, flush: true);
    return;
  }

  // #endregion

  // #region file - text

  static Future<String> readFileText(
    String target,
  ) async {
    return await File(target).readAsString();
  }

  static Future<Void> writeFileText(
    String target,
    String text,
  ) async {
    await File(target).create(recursive: true);
    await File(target).writeAsString(text, flush: true);
    return;
  }

  // #endregion

  // #region shell

  static Map<String, String> _taggedHistoryDirectory = {};

  static Future<String?> pickOpenFile(
    BuildContext context,
    String       tag,
  ) async {
    var target = null as String?;
    var initialDirectory = _taggedHistoryDirectory[tag] ?? '/';
    if (Platform.isWindows) {
      target = (await file_selector.openFile(initialDirectory: toWindowsStyle(initialDirectory)))?.path;
      if (target != null) {
        target = regularize(target);
      }
      if (target == '') {
        target = null;
      }
    }
    if (Platform.isLinux || Platform.isMacOS) {
      target = (await file_selector.openFile(initialDirectory: initialDirectory))?.path;
    }
    if (Platform.isAndroid) {
      target = await PlatformMethod.pickStorageItem('open_file', initialDirectory);
      if (target != null) {
        target = await parseAndroidContentUri(context, Uri.parse(target), true);
      }
    }
    if (Platform.isIOS) {
      target = await PlatformMethod.pickStorageItem('open_file', initialDirectory);
    }
    if (target != null) {
      _taggedHistoryDirectory[tag] = parent(target);
    }
    return target;
  }

  static Future<String?> pickOpenDirectory(
    BuildContext context,
    String       tag,
  ) async {
    var target = null as String?;
    var initialDirectory = _taggedHistoryDirectory[tag] ?? '/';
    if (Platform.isWindows) {
      // NOTE : use `file_selector.getDirectoryPath` instead of `FilePicker.platform.getDirectoryPath`, on windows, the later one will throw an exception if it is the first file dialog since application start.
      target = await file_selector.getDirectoryPath(initialDirectory: toWindowsStyle(initialDirectory));
      if (target != null) {
        target = regularize(target);
      }
      if (target == '') {
        target = null;
      }
    }
    if (Platform.isLinux || Platform.isMacOS) {
      target = await file_selector.getDirectoryPath(initialDirectory: initialDirectory);
    }
    if (Platform.isAndroid) {
      target = await PlatformMethod.pickStorageItem('open_directory', initialDirectory);
      if (target != null) {
        target = await parseAndroidContentUri(context, Uri.parse(target), false);
      }
    }
    if (Platform.isIOS) {
      target = await PlatformMethod.pickStorageItem('open_directory', initialDirectory);
    }
    if (target != null) {
      _taggedHistoryDirectory[tag] = parent(target);
    }
    return target;
  }

  static Future<String?> pickSaveFile(
    BuildContext context,
    String       tag,
  ) async {
    var target = null as String?;
    var initialDirectory = _taggedHistoryDirectory[tag] ?? '/';
    if (Platform.isWindows) {
      target = (await file_selector.getSaveLocation(initialDirectory: toWindowsStyle(initialDirectory)))?.path;
      if (target != null) {
        target = regularize(target);
      }
      if (target == '') {
        target = null;
      }
    }
    if (Platform.isLinux || Platform.isMacOS) {
      target = (await file_selector.getSaveLocation(initialDirectory: initialDirectory))?.path;
    }
    if (Platform.isAndroid) {
      target = await PlatformMethod.pickStorageItem('save_file', initialDirectory);
      if (target != null) {
        target = await parseAndroidContentUri(context, Uri.parse(target), false);
      }
    }
    if (Platform.isIOS) {
      throw UnimplementedError();
    }
    if (target != null) {
      _taggedHistoryDirectory[tag] = parent(target);
    }
    return target;
  }

  // #endregion

  // #region miscellaneous

  static Future<String> queryApplicationSharedDirectory(
  ) async {
    var result = null as String?;
    if (Platform.isWindows) {
      result = (await path_provider.getApplicationSupportDirectory()).path;
      result = regularize(result);
    }
    if (Platform.isLinux) {
      result = (await path_provider.getApplicationSupportDirectory()).path;
    }
    if (Platform.isMacOS) {
      result = (await path_provider.getApplicationSupportDirectory()).path;
    }
    if (Platform.isAndroid) {
      result = (await path_provider.getExternalStorageDirectory())!.path;
    }
    if (Platform.isIOS) {
      result = (await path_provider.getApplicationDocumentsDirectory()).path;
    }
    return result!;
  }

  static Future<String> queryApplicationCacheDirectory(
  ) async {
    var result = null as String?;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS || Platform.isIOS) {
      result = (await queryApplicationSharedDirectory()) + '/cache';
    }
    if (Platform.isAndroid) {
      result = (await path_provider.getApplicationCacheDirectory()).path;
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
          result = '${await PlatformMethod.queryExternalStoragePath()}${result.isEmpty ? '' : '/'}${result}';
        }
        // /tree/primary:<path-relative-external-storage>
        if (path.startsWith('/tree/primary:')) {
          result = path.substring('/tree/primary:'.length);
          result = '${await PlatformMethod.queryExternalStoragePath()}${result.isEmpty ? '' : '/'}${result}';
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
        if (path.startsWith('/') && await exist(path)) {
          result = path;
        }
        break;
      }
    }
    if (result == null) {
      var duplicate = await ControlHelper.showCustomModalDialog<Boolean>(
        context: context,
        title: 'Unparsable Content Uri',
        contentBuilder: (context, setState) => [
          SelectionArea(
            child: Text(
              uri.toString(),
              overflow: TextOverflow.clip,
            ),
          ),
        ],
        actionBuilder: (context) => [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('Ignore'),
          ),
          FilledButton.tonal(
            onPressed: !copyable ? null : () => Navigator.pop(context, true),
            child: const Text('Duplicate'),
          ),
        ],
      ) ?? false;
      if (duplicate) {
        var setting = Provider.of<SettingProvider>(context, listen: false);
        result = await PlatformMethod.copyStorageFile(uri.toString(), setting.data.mFallbackDirectory);
      }
    }
    return result;
  }

  // #endregion

}
