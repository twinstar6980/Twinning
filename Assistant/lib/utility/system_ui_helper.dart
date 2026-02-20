import '/common.dart';
import '/widget/export.dart';
import 'package:flutter/services.dart';

// ----------------

class SystemUiHelper {

  // #region utility

  static Future<Void> applyMode(
    SystemUiMode mode,
  ) async {
    await SystemChrome.setEnabledSystemUIMode(mode);
    return;
  }

  // ----------------

  static final SystemUiOverlayStyle _overlayStyleForLight = .new(
    systemNavigationBarColor: ColorSet.transparent,
    systemNavigationBarDividerColor: ColorSet.transparent,
    systemNavigationBarIconBrightness: .dark,
    systemNavigationBarContrastEnforced: false,
    statusBarColor: ColorSet.transparent,
    statusBarBrightness: .light,
    statusBarIconBrightness: .dark,
    systemStatusBarContrastEnforced: false,
  );

  static final SystemUiOverlayStyle _overlayStyleForDark = .new(
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

  static SystemUiOverlayStyle queryOverlayStyle(
    Brightness brightness,
  ) {
    return brightness == .light ? _overlayStyleForLight : _overlayStyleForDark;
  }

  static Future<Void> applyOverlayStyle(
    Brightness brightness,
  ) async {
    await SystemChrome.restoreSystemUIOverlays();
    SystemChrome.setSystemUIOverlayStyle(queryOverlayStyle(brightness));
    return;
  }

  // #endregion

}
