import '/common.dart';
import '/widget/export.dart';
import '/utility/platform_integration_manager.dart';
import 'package:flutter/services.dart' as lib;
import 'package:material_ui/material_ui.dart' as lib;

// ----------------

class SystemUiHelper {

  // #region mode

  static Future<Void> applyMode(
    lib.SystemUiMode mode,
  ) async {
    await lib.SystemChrome.setEnabledSystemUIMode(mode);
    return;
  }

  // #endregion

  // #region overlay

  static final lib.SystemUiOverlayStyle _overlayStyleForLight = .new(
    systemNavigationBarColor: ColorSet.transparent,
    systemNavigationBarDividerColor: ColorSet.transparent,
    systemNavigationBarIconBrightness: .dark,
    systemNavigationBarContrastEnforced: false,
    statusBarColor: ColorSet.transparent,
    statusBarBrightness: .light,
    statusBarIconBrightness: .dark,
    systemStatusBarContrastEnforced: false,
  );

  static final lib.SystemUiOverlayStyle _overlayStyleForDark = .new(
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

  static lib.SystemUiOverlayStyle queryOverlayStyle(
    lib.Brightness brightness,
  ) {
    return brightness == .light ? SystemUiHelper._overlayStyleForLight : SystemUiHelper._overlayStyleForDark;
  }

  static Future<Void> applyOverlayStyle(
    lib.Brightness brightness,
  ) async {
    await lib.SystemChrome.restoreSystemUIOverlays();
    lib.SystemChrome.setSystemUIOverlayStyle(SystemUiHelper.queryOverlayStyle(brightness));
    return;
  }

  // #endregion

  // #region theme

  static Future<({lib.ColorScheme light, lib.ColorScheme dark})?> queryThemePalette(
  ) async {
    var theme = await PlatformIntegrationManager.instance.invokeQuerySystemTheme();
    if (theme.accent == null) {
      return null;
    }
    var accent = Color(theme.accent!);
    return (
      light: lib.ColorScheme.fromSeed(seedColor: accent, brightness: .light),
      dark: lib.ColorScheme.fromSeed(seedColor: accent, brightness: .dark),
    );
  }

  // #endregion

}
