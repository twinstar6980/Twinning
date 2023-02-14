// ignore_for_file: unused_import, unnecessary_import

import '/common.dart';
import 'dart:io';
import 'dart:convert';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart';
import 'package:path_provider/path_provider.dart';

// ----------------

class Setting {

  ThemeMode    mThemeMode;
  Boolean      mThemeColorInheritFromSystem;
  Color        mThemeColorLight;
  Color        mThemeColorDark;
  Boolean      mUseLargerFontInConsole;
  String       mCore;
  String       mScript;
  List<String> mArgument;
  Boolean      mExitAfterCommandSucceed;

  // ----------------

  Setting(
    this.mThemeMode,
    this.mThemeColorInheritFromSystem,
    this.mThemeColorLight,
    this.mThemeColorDark,
    this.mUseLargerFontInConsole,
    this.mCore,
    this.mScript,
    this.mArgument,
    this.mExitAfterCommandSucceed,
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
  );

  // ----------------

  static
  Void
  fromJson(
    Map<String, dynamic> json,
    Setting   data,
  ) {
    data.mThemeMode = ThemeMode.values.byName(json['theme_mode'] as String);
    data.mThemeColorInheritFromSystem = json['theme_color_inherit_from_system'] as Boolean;
    data.mThemeColorLight = Color(json['theme_color_light'] as Integer);
    data.mThemeColorDark = Color(json['theme_color_dark'] as Integer);
    data.mUseLargerFontInConsole = json['use_larger_font_in_console'] as Boolean;
    data.mCore = json['core'] as String;
    data.mScript = json['script'] as String;
    data.mArgument = (json['argument'] as List<dynamic>).map((e) => e as String).toList();
    data.mExitAfterCommandSucceed = json['exit_after_command_finish'] as Boolean;
    return;
  }

  static
  Void
  toJson(
    Map<String, dynamic> json,
    Setting   data,
  ) {
    json['theme_mode'] = data.mThemeMode.name;
    json['theme_color_inherit_from_system'] = data.mThemeColorInheritFromSystem;
    json['theme_color_light'] = data.mThemeColorLight.value;
    json['theme_color_dark'] = data.mThemeColorDark.value;
    json['use_larger_font_in_console'] = data.mUseLargerFontInConsole;
    json['core'] = data.mCore;
    json['script'] = data.mScript;
    json['argument'] = data.mArgument;
    json['exit_after_command_finish'] = data.mExitAfterCommandSucceed;
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

  Setting _data;
  Boolean _loaded;

  SettingProvider(
  ) : this._data = Setting.init(),
      this._loaded = false {
    () async {
      late Setting data;
      try {
        data = await Setting.load();
      } catch (e) {
        data = Setting.init();
        await Setting.save(data);
      }
      this._data = data;
      this._loaded = true;
      this.notifyListeners();
    }();
  }

  Setting
  get data {
    return this._data;
  }

  Boolean
  get loaded {
    return this._loaded;
  }

  // ----------------

  Future<Void>
  notify(
  ) async {
    this.notifyListeners();
    Setting.save(this._data);
    return;
  }

  Future<Void>
  reset(
  ) async {
    this._data = Setting.init();
    this.notify();
    return;
  }

}
