import '/common.dart';
import '/utility/platform_method.dart';
import 'dart:io';

// ----------------

class PermissionHelper {

  // #region storage

  static Future<Boolean> checkStorage(
  ) async {
    var state = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      state = true;
    }
    if (Platform.isAndroid) {
      state = await PlatformMethod.checkExternalStoragePermission('check');
    }
    if (Platform.isIOS) {
      state = true;
    }
    return state;
  }

  static Future<Boolean> requestStorage(
  ) async {
    var state = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      state = true;
    }
    if (Platform.isAndroid) {
      state = await PlatformMethod.checkExternalStoragePermission('request');
    }
    if (Platform.isIOS) {
      state = true;
    }
    return state;
  }

  // #endregion

}
