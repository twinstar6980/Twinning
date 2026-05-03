import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/application_platform_method.dart';

// ----------------

enum ApplicationPermissionName {
  storage,
  notification,
}

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

  Future<Boolean> check(
    ApplicationPermissionName name,
  ) async {
    var platformResult = await ApplicationPlatformMethod.instance.checkApplicationPermission(ConvertHelper.makeEnumerationToStringOfSnakeCase(name));
    return platformResult.state;
  }

  Future<Void> update(
    ApplicationPermissionName name,
  ) async {
    // ignore: unused_local_variable
    var platformResult = await ApplicationPlatformMethod.instance.updateApplicationPermission(ConvertHelper.makeEnumerationToStringOfSnakeCase(name));
    return;
  }

  // #endregion

}
