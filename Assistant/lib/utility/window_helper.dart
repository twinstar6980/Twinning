import '/common.dart';
import '/utility/platform_integration_manager.dart';

// ----------------

class WindowHelper {

  // #region utility

  static Future<({Integer x, Integer y, Integer width, Integer height})> queryScreenPlacement(
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    var platformResult = await PlatformIntegrationManager.instance.invokeOnDesktopQueryScreenPlacement();
    return platformResult;
  }

  static Future<({Integer x, Integer y, Integer width, Integer height})> queryWindowPlacement(
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    var platformResult = await PlatformIntegrationManager.instance.invokeOnDesktopQueryWindowPlacement();
    return platformResult;
  }

  static Future<Void> updateWindowPlacement(
    Integer x,
    Integer y,
    Integer width,
    Integer height,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    // ignore: unused_local_variable
    var platformResult = await PlatformIntegrationManager.instance.invokeOnDesktopUpdateWindowPlacement(x, y, width, height);
    return;
  }

  // ----------------

  static Future<Void> bringToCenter(
    Integer width,
    Integer height,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    var screenPlacement = await WindowHelper.queryScreenPlacement();
    var x = screenPlacement.x + (screenPlacement.width - width) ~/ 2;
    var y = screenPlacement.y + (screenPlacement.height - height) ~/ 2;
    await WindowHelper.updateWindowPlacement(x, y, width, height);
    return;
  }

  // #endregion

}
