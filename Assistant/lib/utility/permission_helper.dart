import '/common.dart';
import '/utility/platform_method.dart';
import 'dart:io';

// ----------------

class PermissionHelper {

  // #region storage

  static Future<Boolean> checkStorage(
  ) async {
    var result = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = true;
    }
    if (Platform.isAndroid) {
      result = await PlatformMethod.checkStoragePermission('check');
    }
    if (Platform.isIOS) {
      result = true;
    }
    return result;
  }

  static Future<Boolean> requestStorage(
  ) async {
    var result = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = true;
    }
    if (Platform.isAndroid) {
      result = await PlatformMethod.checkStoragePermission('request');
    }
    if (Platform.isIOS) {
      result = true;
    }
    return result;
  }

  // #endregion

}
