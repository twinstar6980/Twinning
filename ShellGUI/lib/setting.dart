import '/common.dart';
import '/utility/font_helper.dart';
import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';

// ----------------

class SettingData {

  ThemeMode    mThemeMode;
  Boolean      mThemeColorInheritFromSystem;
  Color        mThemeColorLight;
  Color        mThemeColorDark;
  List<String> mPrimaryFont;
  List<String> mConsoleFont;
  Boolean      mConsoleFontUseLargerSize;
  Boolean      mWindowPositionAlignToCenter;
  Integer      mWindowPositionX;
  Integer      mWindowPositionY;
  Boolean      mWindowSizeAdhereToDefault;
  Integer      mWindowSizeWidth;
  Integer      mWindowSizeHeight;
  String       mConsoleKernel;
  String       mConsoleScript;
  List<String> mConsoleArgument;
  String       mFallbackDirectory;

  // ----------------

  SettingData(
    this.mThemeMode,
    this.mThemeColorInheritFromSystem,
    this.mThemeColorLight,
    this.mThemeColorDark,
    this.mPrimaryFont,
    this.mConsoleFont,
    this.mConsoleFontUseLargerSize,
    this.mWindowPositionAlignToCenter,
    this.mWindowPositionX,
    this.mWindowPositionY,
    this.mWindowSizeAdhereToDefault,
    this.mWindowSizeWidth,
    this.mWindowSizeHeight,
    this.mConsoleKernel,
    this.mConsoleScript,
    this.mConsoleArgument,
    this.mFallbackDirectory,
  );

  SettingData.init(
  ) : this(
    ThemeMode.system,
    true,
    const Color(0xff6750a4),
    const Color(0xff1750a4),
    [],
    [],
    false,
    true,
    0,
    0,
    true,
    0,
    0,
    '',
    '',
    [],
    '',
  );

  // ----------------

  Void fromJson(
    Map<String, dynamic> json,
  ) {
    this.mThemeMode = ThemeMode.values.byName(json['theme_mode'] as String);
    this.mThemeColorInheritFromSystem = json['theme_color_inherit_from_system'] as Boolean;
    this.mThemeColorLight = Color(json['theme_color_light'] as Integer);
    this.mThemeColorDark = Color(json['theme_color_dark'] as Integer);
    this.mPrimaryFont = (json['primary_font'] as List<dynamic>).cast<String>();
    this.mConsoleFont = (json['console_font'] as List<dynamic>).cast<String>();
    this.mConsoleFontUseLargerSize = json['console_font_use_larger_size'] as Boolean;
    this.mWindowPositionAlignToCenter = json['window_position_align_to_center'] as Boolean;
    this.mWindowPositionX = json['window_position_x'] as Integer;
    this.mWindowPositionY = json['window_position_y'] as Integer;
    this.mWindowSizeAdhereToDefault = json['window_size_adhere_to_default'] as Boolean;
    this.mWindowSizeWidth = json['window_size_width'] as Integer;
    this.mWindowSizeHeight = json['window_size_height'] as Integer;
    this.mConsoleKernel = json['console_kernel'] as String;
    this.mConsoleScript = json['console_script'] as String;
    this.mConsoleArgument = (json['console_argument'] as List<dynamic>).cast<String>();
    this.mFallbackDirectory = json['fallback_directory'] as String;
    return;
  }

  Void toJson(
    Map<String, dynamic> json,
  ) {
    json['theme_mode'] = this.mThemeMode.name;
    json['theme_color_inherit_from_system'] = this.mThemeColorInheritFromSystem;
    json['theme_color_light'] = this.mThemeColorLight.value;
    json['theme_color_dark'] = this.mThemeColorDark.value;
    json['primary_font'] = this.mPrimaryFont;
    json['console_font'] = this.mConsoleFont;
    json['console_font_use_larger_size'] = this.mConsoleFontUseLargerSize;
    json['window_position_align_to_center'] = this.mWindowPositionAlignToCenter;
    json['window_position_x'] = this.mWindowPositionX;
    json['window_position_y'] = this.mWindowPositionY;
    json['window_size_adhere_to_default'] = this.mWindowSizeAdhereToDefault;
    json['window_size_width'] = this.mWindowSizeWidth;
    json['window_size_height'] = this.mWindowSizeHeight;
    json['console_kernel'] = this.mConsoleKernel;
    json['console_script'] = this.mConsoleScript;
    json['console_argument'] = this.mConsoleArgument;
    json['fallback_directory'] = this.mFallbackDirectory;
    return;
  }

  // ----------------

  Future<Void> load(
  ) async {
    var file = File('${await queryApplicationSharedDirectory()}/setting.json');
    if (!await file.exists()) {
      return;
    }
    var json = jsonDecode(await file.readAsString());
    this.fromJson(json);
    return;
  }

  Future<Void> save(
  ) async {
    var file = File('${await queryApplicationSharedDirectory()}/setting.json');
    var json = <String, dynamic>{};
    this.toJson(json);
    await file.writeAsString(jsonEncode(json));
    return;
  }

}

class SettingState {

  List<String>  mPrimaryFontFamliy;
  List<String>  mConsoleFontFamliy;
  List<String>? mAdditionalArgument;

  // ----------------

  SettingState(
    this.mPrimaryFontFamliy,
    this.mConsoleFontFamliy,
    this.mAdditionalArgument,
  );

  SettingState.init(
  ) : this(
    [],
    [],
    null,
  );

  // ----------------

  Future<Void> apply(
    SettingData data,
  ) async {
    this.mPrimaryFontFamliy.clear();
    for (var index = 0; index < data.mPrimaryFont.length; index++) {
      var family = await FontHelper.loadFile(data.mPrimaryFont[index]);
      if (family != null && !this.mPrimaryFontFamliy.contains(family)) {
        this.mPrimaryFontFamliy.add(family);
      }
    }
    this.mConsoleFontFamliy.clear();
    for (var index = 0; index < data.mConsoleFont.length; index++) {
      var family = await FontHelper.loadFile(data.mConsoleFont[index]);
      if (family != null && !this.mConsoleFontFamliy.contains(family)) {
        this.mConsoleFontFamliy.add(family);
      }
    }
    return;
  }

}

class SettingProvider with ChangeNotifier {

  SettingData  data;
  SettingState state;

  // ----------------

  SettingProvider(
    this.data,
    this.state,
  );

  SettingProvider.init(
  ) : this(
    SettingData.init(),
    SettingState.init(),
  );

  // ----------------

  Future<Void> update(
  ) async {
    await this.state.apply(this.data);
    await this.data.save();
    this.notifyListeners();
    return;
  }

}
