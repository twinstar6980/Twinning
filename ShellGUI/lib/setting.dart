// ignore_for_file: unused_import, unnecessary_import

import '/common.dart';
import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:path_provider/path_provider.dart';
import '/common/path_picker.dart';

// ----------------

class Setting {

  ThemeMode    mThemeMode;
  Boolean      mThemeColorInheritFromSystem;
  Color        mThemeColorLight;
  Color        mThemeColorDark;
  Boolean      mUseLargerFontInConsole;
  String       mKernel;
  String       mScript;
  List<String> mArgument;
  Boolean      mBehaviorAfterCommandSucceed;
  String       mFallbackDirectoryForInvisibleFile;

  // ----------------

  Setting(
    this.mThemeMode,
    this.mThemeColorInheritFromSystem,
    this.mThemeColorLight,
    this.mThemeColorDark,
    this.mUseLargerFontInConsole,
    this.mKernel,
    this.mScript,
    this.mArgument,
    this.mBehaviorAfterCommandSucceed,
    this.mFallbackDirectoryForInvisibleFile,
  );

  Setting.init(
  ) : this(
    ThemeMode.system,
    true,
    const Color(0xff6750a4),
    const Color(0xff1750a4),
    false,
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
    data.mUseLargerFontInConsole = json['use_larger_font_in_console'] as Boolean;
    data.mKernel = json['kernel'] as String;
    data.mScript = json['script'] as String;
    data.mArgument = (json['argument'] as List<dynamic>).map((e) => e as String).toList();
    data.mBehaviorAfterCommandSucceed = json['behavior_after_command_succeed'] as Boolean;
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
    json['use_larger_font_in_console'] = data.mUseLargerFontInConsole;
    json['kernel'] = data.mKernel;
    json['script'] = data.mScript;
    json['argument'] = data.mArgument;
    json['behavior_after_command_succeed'] = data.mBehaviorAfterCommandSucceed;
    json['fallback_directory_for_invisible_file'] = data.mFallbackDirectoryForInvisibleFile;
    return;
  }

  // ----------------

  static
  Future<Setting>
  load(
  ) async {
    var directory = await getApplicationSupportDirectory();
    var file = File('${directory.path}/setting.json');
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
    var directory = await getApplicationSupportDirectory();
    var file = File('${directory.path}/setting.json');
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
    PathPicker.fallbackDirectory = this.data.mFallbackDirectoryForInvisibleFile;
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
