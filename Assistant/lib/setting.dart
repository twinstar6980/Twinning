import '/common.dart';
import '/module.dart';
import '/utility/storage_helper.dart';
import '/utility/font_helper.dart';
import '/view/modding_worker/setting.dart' as modding_worker;
import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';

// ----------------

class SettingData {
  String                 mVersion;
  ThemeMode              mThemeMode;
  Boolean                mThemeColorState;
  Color                  mThemeColorLight;
  Color                  mThemeColorDark;
  Boolean                mThemeFontState;
  List<String>           mThemeFontPath;
  Boolean                mWindowPositionState;
  Integer                mWindowPositionX;
  Integer                mWindowPositionY;
  Boolean                mWindowSizeState;
  Integer                mWindowSizeWidth;
  Integer                mWindowSizeHeight;
  String                 mFallbackDirectory;
  ModuleLauncherSetting  mModuleLauncher;
  modding_worker.Setting mModdingWorker;
  SettingData({
    required this.mVersion,
    required this.mThemeMode,
    required this.mThemeColorState,
    required this.mThemeColorLight,
    required this.mThemeColorDark,
    required this.mThemeFontState,
    required this.mThemeFontPath,
    required this.mWindowPositionState,
    required this.mWindowPositionX,
    required this.mWindowPositionY,
    required this.mWindowSizeState,
    required this.mWindowSizeWidth,
    required this.mWindowSizeHeight,
    required this.mFallbackDirectory,
    required this.mModuleLauncher,
    required this.mModdingWorker,
  });
}

class SettingState {
  ModuleLauncherConfiguration? mInitialTab;
  List<String>                 mThemeFontFamliy;
  List<String>                 mModdingWorkerMessageFontFamily;
  SettingState({
    required this.mInitialTab,
    required this.mThemeFontFamliy,
    required this.mModdingWorkerMessageFontFamily,
  });
}

class SettingProvider with ChangeNotifier {

  // #region structor

  SettingData data;

  SettingState state;

  // ----------------

  SettingProvider(
  ) :
    data = _createDefaultData(),
    state = _createDefaultState();

  // #endregion

  // #region action

  Future<Void> reset(
  ) async {
    this.data = _createDefaultData();
    this.state = _createDefaultState();
    return;
  }

  Future<Void> apply(
  ) async {
    this.state.mThemeFontFamliy.clear();
    for (var index = 0; index < (!this.data.mThemeFontState ? 0 : this.data.mThemeFontPath.length); index++) {
      var family = await FontHelper.loadFile(this.data.mThemeFontPath[index]);
      if (family != null && !this.state.mThemeFontFamliy.contains(family)) {
        this.state.mThemeFontFamliy.add(family);
      }
    }
    this.state.mModdingWorkerMessageFontFamily.clear();
    for (var index = 0; index < this.data.mModdingWorker.mMessageFont.length; index++) {
      var family = await FontHelper.loadFile(this.data.mModdingWorker.mMessageFont[index]);
      if (family != null && !this.state.mModdingWorkerMessageFontFamily.contains(family)) {
        this.state.mModdingWorkerMessageFontFamily.add(family);
      }
    }
    this.notifyListeners();
    return;
  }

  // #endregion

  // #region storage

  Future<String> get file async {
    return '${await StorageHelper.queryApplicationSharedDirectory()}/setting.json';
  }

  // ----------------

  Future<Void> load({
    String? file = null,
  }) async {
    file ??= await this.file;
    var json = jsonDecode(await File(file).readAsString());
    _convertDataFromJson(this.data, json);
    return;
  }

  Future<Void> save({
    String? file = null,
    Boolean apply = true,
  }) async {
    file ??= await this.file;
    if (apply) {
      await this.apply();
    }
    var json = <String, dynamic>{};
    _convertDataToJson(this.data, json);
    await File(file).writeAsString(jsonEncode(json));
    return;
  }

  // #endregion

  // #region utility

  static SettingData _createDefaultData(
  ) => SettingData(
    mVersion: kApplicationVersion,
    mThemeMode: ThemeMode.system,
    mThemeColorState: false,
    mThemeColorLight: const Color(0xff6750a4),
    mThemeColorDark: const Color(0xff1750a4),
    mThemeFontState: false,
    mThemeFontPath: [],
    mWindowPositionState: false,
    mWindowPositionX: 0,
    mWindowPositionY: 0,
    mWindowSizeState: false,
    mWindowSizeWidth: 0,
    mWindowSizeHeight: 0,
    mFallbackDirectory: '',
    mModuleLauncher: ModuleLauncherSetting(
      mModule: ModuleType.values.map((e) => (ModuleLauncherConfiguration(title: ModuleHelper.query(e).name, type: e, option: []))).toList(),
      mPinned: [],
      mRecent: [],
    ),
    mModdingWorker: modding_worker.Setting(
      mKernel: '',
      mScript: '',
      mArgument: [],
      mImmediateLaunch: true,
      mMessageFont: [],
    ),
  );

  static SettingState _createDefaultState(
  ) => SettingState(
    mInitialTab: null,
    mThemeFontFamliy: [],
    mModdingWorkerMessageFontFamily: [],
  );

  // ----------------

  static Void _convertDataToJson(
    SettingData          data,
    Map<String, dynamic> json,
  ) {
    {
      assertTest(data.mVersion == kApplicationVersion);
      json['version'] = data.mVersion;
    }
    {
      json['theme_mode'] = data.mThemeMode.name;
      json['theme_color_state'] = data.mThemeColorState;
      json['theme_color_light'] = data.mThemeColorLight.value;
      json['theme_color_dark'] = data.mThemeColorDark.value;
      json['theme_font_state'] = data.mThemeFontState;
      json['theme_font_path'] = data.mThemeFontPath;
    }
    {
      json['window_position_state'] = data.mWindowPositionState;
      json['window_position_x'] = data.mWindowPositionX;
      json['window_position_y'] = data.mWindowPositionY;
      json['window_size_state'] = data.mWindowSizeState;
      json['window_size_width'] = data.mWindowSizeWidth;
      json['window_size_height'] = data.mWindowSizeHeight;
    }
    {
      json['fallback_directory'] = data.mFallbackDirectory;
    }
    {
      json['module_launcher.module'] = data.mModuleLauncher.mModule.map((e) => (<String, dynamic>{
        'title': e.title,
        'type': e.type.name,
        'option': e.option,
      })).toList();
      json['module_launcher.pinned'] = data.mModuleLauncher.mPinned.map((e) => (<String, dynamic>{
        'title': e.title,
        'type': e.type.name,
        'option': e.option,
      })).toList();
      json['module_launcher.recent'] = data.mModuleLauncher.mRecent.map((e) => (<String, dynamic>{
        'title': e.title,
        'type': e.type.name,
        'option': e.option,
      })).toList();
    }
    {
      json['modding_worker.kernel'] = data.mModdingWorker.mKernel;
      json['modding_worker.script'] = data.mModdingWorker.mScript;
      json['modding_worker.argument'] = data.mModdingWorker.mArgument;
      json['modding_worker.immediate_launch'] = data.mModdingWorker.mImmediateLaunch;
      json['modding_worker.message_font'] = data.mModdingWorker.mMessageFont;
    }
    return;
  }

  static Void _convertDataFromJson(
    SettingData          data,
    Map<String, dynamic> json,
  ) {
    {
      data.mVersion = (json['version'] as String);
      assertTest(data.mVersion == kApplicationVersion);
    }
    {
      data.mThemeMode = ThemeMode.values.byName(json['theme_mode'] as String);
      data.mThemeColorState = (json['theme_color_state'] as Boolean);
      data.mThemeColorLight = Color(json['theme_color_light'] as Integer);
      data.mThemeColorDark = Color(json['theme_color_dark'] as Integer);
      data.mThemeFontState = (json['theme_font_state'] as Boolean);
      data.mThemeFontPath = (json['theme_font_path'] as List<dynamic>).cast<String>();
    }
    {
      data.mWindowPositionState = (json['window_position_state'] as Boolean);
      data.mWindowPositionX = (json['window_position_x'] as Integer);
      data.mWindowPositionY = (json['window_position_y'] as Integer);
      data.mWindowSizeState = (json['window_size_state'] as Boolean);
      data.mWindowSizeWidth = (json['window_size_width'] as Integer);
      data.mWindowSizeHeight = (json['window_size_height'] as Integer);
    }
    {
      data.mFallbackDirectory = (json['fallback_directory'] as String);
    }
    {
      data.mModuleLauncher.mModule = (json['module_launcher.module'] as List<dynamic>).cast<Map<String, dynamic>>().map((e) => (ModuleLauncherConfiguration(
        title: (e['title'] as String),
        type: ModuleType.values.byName(e['type'] as String),
        option: (e['option'] as List<dynamic>).cast<String>(),
      ))).toList();
      data.mModuleLauncher.mPinned = (json['module_launcher.pinned'] as List<dynamic>).cast<Map<String, dynamic>>().map((e) => (ModuleLauncherConfiguration(
        title: (e['title'] as String),
        type: ModuleType.values.byName(e['type'] as String),
        option: (e['option'] as List<dynamic>).cast<String>(),
      ))).toList();
      data.mModuleLauncher.mRecent = (json['module_launcher.recent'] as List<dynamic>).cast<Map<String, dynamic>>().map((e) => (ModuleLauncherConfiguration(
        title: (e['title'] as String),
        type: ModuleType.values.byName(e['type'] as String),
        option: (e['option'] as List<dynamic>).cast<String>(),
      ))).toList();
    }
    {
      data.mModdingWorker.mKernel = (json['modding_worker.kernel'] as String);
      data.mModdingWorker.mScript = (json['modding_worker.script'] as String);
      data.mModdingWorker.mArgument = (json['modding_worker.argument'] as List<dynamic>).cast<String>();
      data.mModdingWorker.mImmediateLaunch = (json['modding_worker.immediate_launch'] as Boolean);
      data.mModdingWorker.mMessageFont = (json['modding_worker.message_font'] as List<dynamic>).cast<String>();
    }
    return;
  }

  // #endregion

}
