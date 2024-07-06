import '/common.dart';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';

// ----------------

class SystemOverlayHelper {

  static const SystemUiOverlayStyle _styleForLight = SystemUiOverlayStyle(
    systemNavigationBarColor: Colors.transparent,
    systemNavigationBarDividerColor: Colors.transparent,
    systemNavigationBarIconBrightness: Brightness.dark,
    systemNavigationBarContrastEnforced: false,
    statusBarColor: Colors.transparent,
    statusBarBrightness: Brightness.light,
    statusBarIconBrightness: Brightness.dark,
    systemStatusBarContrastEnforced: false,
  );

  static const SystemUiOverlayStyle _styleForDark = SystemUiOverlayStyle(
    systemNavigationBarColor: Colors.transparent,
    systemNavigationBarDividerColor: Colors.transparent,
    systemNavigationBarIconBrightness: Brightness.light,
    systemNavigationBarContrastEnforced: false,
    statusBarColor: Colors.transparent,
    statusBarBrightness: Brightness.dark,
    statusBarIconBrightness: Brightness.light,
    systemStatusBarContrastEnforced: false,
  );

  // ----------------

  static SystemUiOverlayStyle query(
    Brightness brightness,
  ) {
    return brightness == Brightness.light ? _styleForLight : _styleForDark;
  }

  static Future<Void> apply(
    Brightness brightness,
  ) async {
    await SystemChrome.restoreSystemUIOverlays();
    SystemChrome.setSystemUIOverlayStyle(query(brightness));
    return;
  }

}
