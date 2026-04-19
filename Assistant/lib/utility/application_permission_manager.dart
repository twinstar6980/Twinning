import '/common.dart';
import '/utility/application_platform_method.dart';

// ----------------

class ApplicationPermissionManager {

  // #region singleton

  static final ApplicationPermissionManager instance = ._();

  // #endregion

  // #region constructor

  // ----------------

  ApplicationPermissionManager._(
  );

  // #endregion

  // #region utility

  Future<Boolean> checkStorage(
  ) async {
    var state = true;
    if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
      state = true;
    }
    if (SystemChecker.isAndroid) {
      state = (await ApplicationPlatformMethod.instance.checkStoragePermission('check')).state;
    }
    if (SystemChecker.isIphone) {
      state = true;
    }
    return state;
  }

  Future<Boolean> requestStorage(
  ) async {
    var state = true;
    if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
      state = true;
    }
    if (SystemChecker.isAndroid) {
      state = (await ApplicationPlatformMethod.instance.checkStoragePermission('request')).state;
    }
    if (SystemChecker.isIphone) {
      state = true;
    }
    return state;
  }

  // #endregion

}
