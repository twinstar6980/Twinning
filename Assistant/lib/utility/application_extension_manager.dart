import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/platform_integration_manager.dart';

// ----------------

enum ApplicationExtensionName {
  forwarder,
}

// ----------------

class ApplicationExtensionManager {

  // #region singleton

  static final ApplicationExtensionManager instance = ._();

  // #endregion

  // #region constructor

  // ----------------

  ApplicationExtensionManager._(
  );

  // #endregion

  // #region utility

  Future<Boolean> check(
    ApplicationExtensionName name,
  ) async {
    var platformResult = await PlatformIntegrationManager.instance.invokeCheckApplicationExtension(ConvertHelper.makeEnumerationToStringOfSnakeCase(name));
    return platformResult.state;
  }

  Future<Void> update(
    ApplicationExtensionName name,
    Boolean                  state,
  ) async {
    // ignore: unused_local_variable
    var platformResult = await PlatformIntegrationManager.instance.invokeUpdateApplicationExtension(ConvertHelper.makeEnumerationToStringOfSnakeCase(name), state);
    return;
  }

  // #endregion

}
