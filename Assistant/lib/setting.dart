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
  Integer                mVersion;
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

extension SettingExtension on SettingData {

  static SettingData createDefaultData(
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

  // ----------------

  Void fromJson(
    Map<String, dynamic> json,
  ) {
    {
      this.mVersion = (json['version'] as Integer);
      assertTest(this.mVersion == kApplicationVersion);
    }
    {
      this.mThemeMode = ThemeMode.values.byName(json['theme_mode'] as String);
      this.mThemeColorState = (json['theme_color_state'] as Boolean);
      this.mThemeColorLight = Color(json['theme_color_light'] as Integer);
      this.mThemeColorDark = Color(json['theme_color_dark'] as Integer);
      this.mThemeFontState = (json['theme_font_state'] as Boolean);
      this.mThemeFontPath = (json['theme_font_path'] as List<dynamic>).cast<String>();
    }
    {
      this.mWindowPositionState = (json['window_position_state'] as Boolean);
      this.mWindowPositionX = (json['window_position_x'] as Integer);
      this.mWindowPositionY = (json['window_position_y'] as Integer);
      this.mWindowSizeState = (json['window_size_state'] as Boolean);
      this.mWindowSizeWidth = (json['window_size_width'] as Integer);
      this.mWindowSizeHeight = (json['window_size_height'] as Integer);
    }
    {
      this.mFallbackDirectory = (json['fallback_directory'] as String);
    }
    {
      this.mModuleLauncher.mModule = (json['module_launcher.module'] as List<dynamic>).cast<Map<String, dynamic>>().map((e) => (ModuleLauncherConfiguration(
        title: (e['title'] as String),
        type: ModuleType.values.byName(e['type'] as String),
        option: (e['option'] as List<dynamic>).cast<String>(),
      ))).toList();
      this.mModuleLauncher.mPinned = (json['module_launcher.pinned'] as List<dynamic>).cast<Map<String, dynamic>>().map((e) => (ModuleLauncherConfiguration(
        title: (e['title'] as String),
        type: ModuleType.values.byName(e['type'] as String),
        option: (e['option'] as List<dynamic>).cast<String>(),
      ))).toList();
      this.mModuleLauncher.mRecent = (json['module_launcher.recent'] as List<dynamic>).cast<Map<String, dynamic>>().map((e) => (ModuleLauncherConfiguration(
        title: (e['title'] as String),
        type: ModuleType.values.byName(e['type'] as String),
        option: (e['option'] as List<dynamic>).cast<String>(),
      ))).toList();
    }
    {
      this.mModdingWorker.mKernel = (json['modding_worker.kernel'] as String);
      this.mModdingWorker.mScript = (json['modding_worker.script'] as String);
      this.mModdingWorker.mArgument = (json['modding_worker.argument'] as List<dynamic>).cast<String>();
      this.mModdingWorker.mImmediateLaunch = (json['modding_worker.immediate_launch'] as Boolean);
      this.mModdingWorker.mMessageFont = (json['modding_worker.message_font'] as List<dynamic>).cast<String>();
    }
    return;
  }

  Void toJson(
    Map<String, dynamic> json,
  ) {
    {
      assertTest(this.mVersion == kApplicationVersion);
      json['version'] = this.mVersion;
    }
    {
      json['theme_mode'] = this.mThemeMode.name;
      json['theme_color_state'] = this.mThemeColorState;
      json['theme_color_light'] = this.mThemeColorLight.value;
      json['theme_color_dark'] = this.mThemeColorDark.value;
      json['theme_font_state'] = this.mThemeFontState;
      json['theme_font_path'] = this.mThemeFontPath;
    }
    {
      json['window_position_state'] = this.mWindowPositionState;
      json['window_position_x'] = this.mWindowPositionX;
      json['window_position_y'] = this.mWindowPositionY;
      json['window_size_state'] = this.mWindowSizeState;
      json['window_size_width'] = this.mWindowSizeWidth;
      json['window_size_height'] = this.mWindowSizeHeight;
    }
    {
      json['fallback_directory'] = this.mFallbackDirectory;
    }
    {
      json['module_launcher.module'] = this.mModuleLauncher.mModule.map((e) => (<String, dynamic>{
        'title': e.title,
        'type': e.type.name,
        'option': e.option,
      })).toList();
      json['module_launcher.pinned'] = this.mModuleLauncher.mPinned.map((e) => (<String, dynamic>{
        'title': e.title,
        'type': e.type.name,
        'option': e.option,
      })).toList();
      json['module_launcher.recent'] = this.mModuleLauncher.mRecent.map((e) => (<String, dynamic>{
        'title': e.title,
        'type': e.type.name,
        'option': e.option,
      })).toList();
    }
    {
      json['modding_worker.kernel'] = this.mModdingWorker.mKernel;
      json['modding_worker.script'] = this.mModdingWorker.mScript;
      json['modding_worker.argument'] = this.mModdingWorker.mArgument;
      json['modding_worker.immediate_launch'] = this.mModdingWorker.mImmediateLaunch;
      json['modding_worker.message_font'] = this.mModdingWorker.mMessageFont;
    }
    return;
  }

  // ----------------

  Future<Void> load({
    String? file,
  }) async {
    file ??= '${await StorageHelper.queryApplicationSharedDirectory()}/setting.json';
    var json = jsonDecode(await File(file).readAsString());
    this.fromJson(json);
    return;
  }

  Future<Void> save({
    String? file,
  }) async {
    file ??= '${await StorageHelper.queryApplicationSharedDirectory()}/setting.json';
    var json = <String, dynamic>{};
    this.toJson(json);
    await File(file).writeAsString(jsonEncode(json));
    return;
  }

}

class SettingState {

  List<String>                        mThemeFontFamliy;
  List<String>                        mConsoleFontFamliy;
  (String, ModuleType, List<String>)? mInitialTab;

  // ----------------

  SettingState(
    this.mThemeFontFamliy,
    this.mConsoleFontFamliy,
    this.mInitialTab,
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
    this.mThemeFontFamliy.clear();
    for (var index = 0; index < (!data.mThemeFontState ? 0 : data.mThemeFontPath.length); index++) {
      var family = await FontHelper.loadFile(data.mThemeFontPath[index]);
      if (family != null && !this.mThemeFontFamliy.contains(family)) {
        this.mThemeFontFamliy.add(family);
      }
    }
    this.mConsoleFontFamliy.clear();
    for (var index = 0; index < data.mModdingWorker.mMessageFont.length; index++) {
      var family = await FontHelper.loadFile(data.mModdingWorker.mMessageFont[index]);
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
    SettingExtension.createDefaultData(),
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
