import '/common.dart';
import '/utility/platform_method.dart';

// ----------------

class PermissionHelper {

  // #region storage

  static Future<Boolean> checkStorage(
  ) async {
    var state = true;
    if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
      state = true;
    }
    if (SystemChecker.isAndroid) {
      state = await PlatformMethod.checkExternalStoragePermission('check');
    }
    if (SystemChecker.isIphone) {
      state = true;
    }
    return state;
  }

  static Future<Boolean> requestStorage(
  ) async {
    var state = true;
    if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
      state = true;
    }
    if (SystemChecker.isAndroid) {
      state = await PlatformMethod.checkExternalStoragePermission('request');
    }
    if (SystemChecker.isIphone) {
      state = true;
    }
    return state;
  }

  // #endregion

}
