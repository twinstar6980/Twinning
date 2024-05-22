import '/common.dart';
import '/utility/platform_method.dart';
import 'dart:io';
import 'package:path/path.dart' as p;
import 'package:file_selector/file_selector.dart' as file_selector;
import 'package:file_picker/file_picker.dart';
import 'package:path_provider/path_provider.dart';

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

  // #region general

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

  // #region shell

  static Map<String, String> _taggedHistoryDirectory = {};

  static Future<String?> pickOpenFile(
    String tag,
    String fallbackDirectory,
  ) async {
    var target = null as String?;
    var initialDirectory = _taggedHistoryDirectory[tag] ?? '/';
    if (Platform.isWindows) {
      target = (await file_selector.openFile(initialDirectory: toWindowsStyle(initialDirectory)))?.path;
      if (target != null) {
        target = regularize(target);
      }
    }
    if (Platform.isLinux || Platform.isMacOS) {
      target = (await FilePicker.platform.pickFiles(
        lockParentWindow: true,
        initialDirectory: initialDirectory,
      ))?.files.single.path;
    }
    if (Platform.isAndroid || Platform.isIOS) {
      target = await PlatformMethod.pickStoragePath('open_file', initialDirectory, fallbackDirectory);
    }
    if (target != null) {
      _taggedHistoryDirectory[tag] = parent(target);
    }
    return target;
  }

  static Future<String?> pickOpenDirectory(
    String tag,
  ) async {
    var target = null as String?;
    var initialDirectory = _taggedHistoryDirectory[tag] ?? '/';
    if (Platform.isWindows) {
      // NOTE : use `file_selector.getDirectoryPath` instead of `FilePicker.platform.getDirectoryPath`, on windows, the later one will throw an exception if it is the first file dialog since application start.
      target = await file_selector.getDirectoryPath(initialDirectory: toWindowsStyle(initialDirectory));
      if (target != null) {
        target = regularize(target);
      }
    }
    if (Platform.isLinux || Platform.isMacOS) {
      target = await FilePicker.platform.getDirectoryPath(
        lockParentWindow: true,
        initialDirectory: initialDirectory,
      );
    }
    if (Platform.isAndroid || Platform.isIOS) {
      target = await PlatformMethod.pickStoragePath('open_directory', initialDirectory, '');
    }
    if (target != null) {
      _taggedHistoryDirectory[tag] = parent(target);
    }
    return target;
  }

  static Future<String?> pickSaveFile(
    String tag,
  ) async {
    var target = null as String?;
    var initialDirectory = _taggedHistoryDirectory[tag] ?? '/';
    if (Platform.isWindows) {
      target = (await file_selector.getSaveLocation(initialDirectory: toWindowsStyle(initialDirectory)))?.path;
      if (target != null) {
        target = regularize(target);
      }
    }
    if (Platform.isLinux || Platform.isMacOS) {
      target = await FilePicker.platform.saveFile(
        lockParentWindow: true,
        initialDirectory: initialDirectory,
      );
    }
    if (Platform.isAndroid) {
      target = await PlatformMethod.pickStoragePath('save_file', initialDirectory, '');
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
      result = (await getApplicationSupportDirectory()).path;
      result = regularize(result);
    }
    if (Platform.isLinux) {
      result = (await getApplicationSupportDirectory()).path;
    }
    if (Platform.isMacOS) {
      result = (await getApplicationSupportDirectory()).path;
    }
    if (Platform.isAndroid) {
      result = (await getExternalStorageDirectory())!.path;
    }
    if (Platform.isIOS) {
      result = (await getApplicationDocumentsDirectory()).path;
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
      result = (await getApplicationCacheDirectory()).path;
    }
    return result!;
  }

  // #endregion

}
