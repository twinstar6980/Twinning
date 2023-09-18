import '/common.dart';
import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';

// ----------------

class Setting {

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
  String       mCommandKernel;
  String       mCommandScript;
  List<String> mCommandArgument;
  Boolean      mExitAfterCommandSucceed;
  String       mFallbackDirectoryForInvisibleFile;

  // ----------------

  Setting(
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
    this.mCommandKernel,
    this.mCommandScript,
    this.mCommandArgument,
    this.mExitAfterCommandSucceed,
    this.mFallbackDirectoryForInvisibleFile,
  );

  Setting.init(
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
    true,
    '',
  );

  // ----------------

  static
  Void
  fromJson(
    Map<String, dynamic> json,
    Setting              data,
  ) {
    data.mThemeMode = ThemeMode.values.byName(json['theme_mode'] as String);
    data.mThemeColorInheritFromSystem = json['theme_color_inherit_from_system'] as Boolean;
    data.mThemeColorLight = Color(json['theme_color_light'] as Integer);
    data.mThemeColorDark = Color(json['theme_color_dark'] as Integer);
    data.mPrimaryFont = (json['primary_font'] as List<dynamic>).cast<String>();
    data.mConsoleFont = (json['console_font'] as List<dynamic>).cast<String>();
    data.mConsoleFontUseLargerSize = json['console_font_use_larger_size'] as Boolean;
    data.mWindowPositionAlignToCenter = json['window_position_align_to_center'] as Boolean;
    data.mWindowPositionX = json['window_position_x'] as Integer;
    data.mWindowPositionY = json['window_position_y'] as Integer;
    data.mWindowSizeAdhereToDefault = json['window_size_adhere_to_default'] as Boolean;
    data.mWindowSizeWidth = json['window_size_width'] as Integer;
    data.mWindowSizeHeight = json['window_size_height'] as Integer;
    data.mCommandKernel = json['command_kernel'] as String;
    data.mCommandScript = json['command_script'] as String;
    data.mCommandArgument = (json['command_argument'] as List<dynamic>).cast<String>();
    data.mExitAfterCommandSucceed = json['exit_after_command_succeed'] as Boolean;
    data.mFallbackDirectoryForInvisibleFile = json['fallback_directory_for_invisible_file'] as String;
    return;
  }

  static
  Void
  toJson(
    Map<String, dynamic> json,
    Setting              data,
  ) {
    json['theme_mode'] = data.mThemeMode.name;
    json['theme_color_inherit_from_system'] = data.mThemeColorInheritFromSystem;
    json['theme_color_light'] = data.mThemeColorLight.value;
    json['theme_color_dark'] = data.mThemeColorDark.value;
    json['primary_font'] = data.mPrimaryFont;
    json['console_font'] = data.mConsoleFont;
    json['console_font_use_larger_size'] = data.mConsoleFontUseLargerSize;
    json['window_position_align_to_center'] = data.mWindowPositionAlignToCenter;
    json['window_position_x'] = data.mWindowPositionX;
    json['window_position_y'] = data.mWindowPositionY;
    json['window_size_adhere_to_default'] = data.mWindowSizeAdhereToDefault;
    json['window_size_width'] = data.mWindowSizeWidth;
    json['window_size_height'] = data.mWindowSizeHeight;
    json['command_kernel'] = data.mCommandKernel;
    json['command_script'] = data.mCommandScript;
    json['command_argument'] = data.mCommandArgument;
    json['exit_after_command_succeed'] = data.mExitAfterCommandSucceed;
    json['fallback_directory_for_invisible_file'] = data.mFallbackDirectoryForInvisibleFile;
    return;
  }

  // ----------------

  static
  Future<Setting>
  load(
  ) async {
    var file = File('${await queryApplicationSharedDirectory()}/setting.json');
    file.create();
    var text = await file.readAsString();
    var json = jsonDecode(text);
    var data = Setting.init();
    Setting.fromJson(json, data);
    return data;
  }

  static
  Future<Void>
  save(
    Setting data,
  ) async {
    var file = File('${await queryApplicationSharedDirectory()}/setting.json');
    file.create();
    var json = <String, dynamic>{};
    Setting.toJson(json, data);
    var text = jsonEncode(json);
    await file.writeAsString(text);
    return;
  }

}

class SettingProvider with ChangeNotifier {

  Setting data;

  SettingProvider(
    this.data,
  );

  // ----------------

  Void
  notify(
  ) async {
    this.notifyListeners();
    Setting.save(this.data);
    return;
  }

  Void
  set(
    Setting data,
  ) async {
    this.data = data;
    this.notify();
    return;
  }

}
