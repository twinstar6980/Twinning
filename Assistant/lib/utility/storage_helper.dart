import '/common.dart';
import '/utility/platform_method.dart';
import 'dart:io';
import 'package:path/path.dart' as p;
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

  static String toNativeStyle(
    String path,
  ) {
    if (Platform.isWindows) {
      return toWindowsStyle(path);
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return toPosixStyle(path);
    }
    throw UnimplementedError();
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
    var parent = '${await queryApplicationSharedDirectory()}/cache';
    if (Platform.isAndroid) {
      parent = (await getApplicationCacheDirectory()).path;
    }
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

  // #endregion

  // #region shell

  static Map<String, String> _taggedHistoryDirectory = {};

  static Future<String?> pickOpenFile(
    String tag,
    String fallbackDirectory,
  ) async {
    var target = null as String?;
    var initialDirectory = toNativeStyle(_taggedHistoryDirectory[tag] ?? '/');
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      target = (await FilePicker.platform.pickFiles(
        lockParentWindow: true,
        initialDirectory: initialDirectory,
      ))?.files.single.path;
      if (Platform.isWindows && target != null) {
        target = regularize(target);
      }
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
    var initialDirectory = toNativeStyle(_taggedHistoryDirectory[tag] ?? '/');
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      target = await FilePicker.platform.getDirectoryPath(
        lockParentWindow: true,
        initialDirectory: initialDirectory,
      );
      if (Platform.isWindows && target != null) {
        target = regularize(target);
      }
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
    var initialDirectory = toNativeStyle(_taggedHistoryDirectory[tag] ?? '/');
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      target = await FilePicker.platform.saveFile(
        lockParentWindow: true,
        initialDirectory: initialDirectory,
      );
      if (Platform.isWindows && target != null) {
        target = regularize(target);
      }
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

  // #endregion

}
