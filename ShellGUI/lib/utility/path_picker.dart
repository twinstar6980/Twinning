import '/common.dart';
import '/utility/platform_method.dart';
import 'dart:io';
import 'package:file_picker/file_picker.dart';

// ----------------

class PathPicker {

  static Future<String?> pickFile(
    String fallbackDirectory,
  ) async {
    var result = null as String?;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = (await FilePicker.platform.pickFiles(allowMultiple: false, lockParentWindow: true))?.files.single.path;
      if (Platform.isWindows && result != null) {
        result = normalizePath(result);
      }
    }
    if (Platform.isAndroid || Platform.isIOS) {
      result = await PlatformMethod.pickPath(false, fallbackDirectory);
    }
    return result;
  }

  static Future<String?> pickDirectory(
  ) async {
    var result = null as String?;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = await FilePicker.platform.getDirectoryPath(lockParentWindow: true);
      if (Platform.isWindows && result != null) {
        result = normalizePath(result);
      }
    }
    if (Platform.isAndroid || Platform.isIOS) {
      result = await PlatformMethod.pickPath(true, '');
    }
    return result;
  }

}
