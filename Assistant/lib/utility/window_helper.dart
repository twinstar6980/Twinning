import '/common.dart';
import '/utility/platform_integration_manager.dart';

// ----------------

class WindowHelper {

  // #region utility

  static Future<({Integer x, Integer y, Integer width, Integer height})> queryScreenPlacement(
  ) async {
    var platformResult = await PlatformIntegrationManager.instance.invokeQueryScreenPlacement();
    return platformResult;
  }

  static Future<({Integer x, Integer y, Integer width, Integer height})> queryWindowPlacement(
  ) async {
    var platformResult = await PlatformIntegrationManager.instance.invokeQueryWindowPlacement();
    return platformResult;
  }

  static Future<Void> updateWindowPlacement(
    Integer x,
    Integer y,
    Integer width,
    Integer height,
  ) async {
    // ignore: unused_local_variable
    var platformResult = await PlatformIntegrationManager.instance.invokeUpdateWindowPlacement(x, y, width, height);
    return;
  }

  // ----------------

  static Future<Void> bringToCenter(
    Integer width,
    Integer height,
  ) async {
    var screenPlacement = await WindowHelper.queryScreenPlacement();
    var x = screenPlacement.x + (screenPlacement.width - width) ~/ 2;
    var y = screenPlacement.y + (screenPlacement.height - height) ~/ 2;
    await WindowHelper.updateWindowPlacement(x, y, width, height);
    return;
  }

  // #endregion

}
