import '/common.dart';
import '/common/platform_method.dart';
import 'dart:io';
import 'package:path/path.dart' as p_path;
import 'package:file_picker/file_picker.dart';

// ----------------

class PathPicker {

  static String fallbackDirectory = '';

  // ----------------

  static
  Future<String?>
  pickFile (
  ) async {
    var result = null as String?;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      var selection = await FilePicker.platform.pickFiles(allowMultiple: false, lockParentWindow: true);
      result = selection?.files.single.path?.replaceAll('\\', '/');
    }
    if (Platform.isAndroid) {
      result = await PlatformMethod.pickPath(false, fallbackDirectory);
    }
    if (Platform.isIOS) {
      if (Directory(fallbackDirectory).existsSync()) {
        var selection = await FilePicker.platform.pickFiles(allowMultiple: false);
        if (selection != null) {
          try {
            var fallbackFile = p_path.join(fallbackDirectory, selection.files.single.name);
            File(selection.files.single.path!).renameSync(fallbackFile);
            result = fallbackFile;
          }
          catch (e) {
          }
          FilePicker.platform.clearTemporaryFiles();
        }
      }
    }
    return result;
  }

  static
  Future<String?>
  pickDirectory (
  ) async {
    var result = null as String?;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      var selection = await FilePicker.platform.getDirectoryPath(lockParentWindow: true);
      result = selection?.replaceAll('\\', '/');
    }
    if (Platform.isAndroid) {
      result = await PlatformMethod.pickPath(true, null);
    }
    if (Platform.isIOS) {
      var selection = await FilePicker.platform.getDirectoryPath();
      result = selection;
    }
    return result;
  }

}
