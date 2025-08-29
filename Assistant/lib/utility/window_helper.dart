import '/common.dart';
import 'package:flutter/widgets.dart';
import 'package:window_manager/window_manager.dart';

// ----------------

class WindowHelper {

  // #region utility

  static Future<Void> setPosition(
    Floater x,
    Floater y,
  ) async {
    await windowManager.setPosition(Offset(x, y));
    return;
  }

  static Future<Void> setSize(
    Floater width,
    Floater height,
  ) async {
    await windowManager.setSize(Size(width, height));
    return;
  }

  static Future<Void> setAtCenter(
  ) async {
    await windowManager.center();
    return;
  }

  // ----------------

  static Future<Void> show(
  ) async {
    await windowManager.show();
    return;
  }

  // ----------------

  static Future<Void> ensureInitialized(
  ) async {
    await windowManager.ensureInitialized();
    return;
  }

  static Future<Void> waitUntilReadyToShow(
  ) async {
    await windowManager.waitUntilReadyToShow();
    return;
  }

  // #endregion

}
