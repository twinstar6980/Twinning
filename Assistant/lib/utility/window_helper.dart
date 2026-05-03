import '/common.dart';
import 'package:window_manager/window_manager.dart' as lib;

// ----------------

class WindowHelper {

  // #region utility

  static Future<Void> setPosition(
    Integer x,
    Integer y,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    await lib.windowManager.setPosition(.new(x.toDouble(), y.toDouble()));
    return;
  }

  static Future<Void> setSize(
    Integer width,
    Integer height,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    await lib.windowManager.setSize(.new(width.toDouble(), height.toDouble()));
    return;
  }

  static Future<Void> setAtCenter(
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    await lib.windowManager.center();
    return;
  }

  // ----------------

  static Future<Void> show(
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    await lib.windowManager.show();
    return;
  }

  // ----------------

  static Future<Void> ensureInitialized(
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    await lib.windowManager.ensureInitialized();
    return;
  }

  static Future<Void> waitUntilReadyToShow(
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh);
    await lib.windowManager.waitUntilReadyToShow();
    return;
  }

  // #endregion

}
