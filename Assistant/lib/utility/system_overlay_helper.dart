import '/common.dart';
import '/widget/export.dart';
import 'package:flutter/services.dart';

// ----------------

class SystemOverlayHelper {

  // #region utility

  static final SystemUiOverlayStyle _styleForLight = SystemUiOverlayStyle(
    systemNavigationBarColor: ColorSet.transparent,
    systemNavigationBarDividerColor: ColorSet.transparent,
    systemNavigationBarIconBrightness: .dark,
    systemNavigationBarContrastEnforced: false,
    statusBarColor: ColorSet.transparent,
    statusBarBrightness: .light,
    statusBarIconBrightness: .dark,
    systemStatusBarContrastEnforced: false,
  );

  static final SystemUiOverlayStyle _styleForDark = SystemUiOverlayStyle(
    systemNavigationBarColor: ColorSet.transparent,
    systemNavigationBarDividerColor: ColorSet.transparent,
    systemNavigationBarIconBrightness: .light,
    systemNavigationBarContrastEnforced: false,
    statusBarColor: ColorSet.transparent,
    statusBarBrightness: .dark,
    statusBarIconBrightness: .light,
    systemStatusBarContrastEnforced: false,
  );

  // ----------------

  static SystemUiOverlayStyle query(
    Brightness brightness,
  ) {
    return brightness == .light ? _styleForLight : _styleForDark;
  }

  static Future<Void> apply(
    Brightness brightness,
  ) async {
    await SystemChrome.restoreSystemUIOverlays();
    SystemChrome.setSystemUIOverlayStyle(query(brightness));
    return;
  }

  // #endregion

}
