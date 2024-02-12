import '/common.dart';
import '/utility/platform_method.dart';
import 'dart:io';

// ----------------

class PermissionHelper {

  static Future<Boolean> checkStoragePermission(
  ) async {
    var result = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = true;
    }
    if (Platform.isAndroid) {
      result = (await PlatformMethod.checkStoragePermission());
    }
    if (Platform.isIOS) {
      result = true;
    }
    return result;
  }

  static Future<Boolean> requestStoragePermission(
  ) async {
    var result = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = true;
    }
    if (Platform.isAndroid) {
      result = (await PlatformMethod.requestStoragePermission());
    }
    if (Platform.isIOS) {
      result = true;
    }
    return result;
  }

}
