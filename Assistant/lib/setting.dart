import '/common.dart';
import '/module.dart';
import '/utility/storage_helper.dart';
import '/utility/json_helper.dart';
import '/utility/font_helper.dart';
import '/widget/export.dart';
import '/view/core_modding_worker/setting.dart' as core_modding_worker;
import '/view/core_modding_worker/submission_type.dart' as core_modding_worker;
import '/view/core_modding_worker/value_expression.dart' as core_modding_worker;
import '/view/core_command_sender/setting.dart' as core_command_sender;
import '/view/core_resource_shipper/setting.dart' as core_resource_shipper;
import '/view/popcap_animation_viewer/setting.dart' as popcap_animation_viewer;
import '/view/kairosoft_game_manager/setting.dart' as kairosoft_game_manager;
import 'package:flutter/widgets.dart';

// ----------------

class SettingData {
  String                          version;
  StyledThemeMode                 themeMode;
  Boolean                         themeColorState;
  Color                           themeColorLight;
  Color                           themeColorDark;
  Boolean                         themeFontState;
  List<String>                    themeFontPath;
  Boolean                         windowPositionState;
  Integer                         windowPositionX;
  Integer                         windowPositionY;
  Boolean                         windowSizeState;
  Integer                         windowSizeWidth;
  Integer                         windowSizeHeight;
  String                          storagePickerFallbackDirectory;
  Map<String, String>             storagePickerHistoryLocation;
  ModuleType                      forwarderDefaultTarget;
  Boolean                         forwarderImmediateJump;
  String                          moduleConfigurationDirectory;
  ModuleLauncherSetting           moduleLauncher;
  core_modding_worker.Setting     coreModdingWorker;
  core_command_sender.Setting     coreCommandSender;
  core_resource_shipper.Setting   coreResourceShipper;
  popcap_animation_viewer.Setting popcapAnimationViewer;
  kairosoft_game_manager.Setting  kairosoftGameManager;
  SettingData({
    required this.version,
    required this.themeMode,
    required this.themeColorState,
    required this.themeColorLight,
    required this.themeColorDark,
    required this.themeFontState,
    required this.themeFontPath,
    required this.windowPositionState,
    required this.windowPositionX,
    required this.windowPositionY,
    required this.windowSizeState,
    required this.windowSizeWidth,
    required this.windowSizeHeight,
    required this.storagePickerFallbackDirectory,
    required this.storagePickerHistoryLocation,
    required this.forwarderDefaultTarget,
    required this.forwarderImmediateJump,
    required this.moduleConfigurationDirectory,
    required this.moduleLauncher,
    required this.coreModdingWorker,
    required this.coreCommandSender,
    required this.coreResourceShipper,
    required this.popcapAnimationViewer,
    required this.kairosoftGameManager,
  });
}

class SettingState {
  Future<Void> Function(String, ModuleType, List<String>)? handleLaunch;
  Future<Void> Function(List<String>)?                     handleForward;
  Future<Void> Function(List<String>)?                     handleCommand;
  Future<Void> Function(Uri)?                              handleLink;
  GlobalKey<NavigatorState>                                applicationNavigatorKey;
  List<String>                                             themeFontFamliy;
  Future<Void> Function()?                                 homeShowLauncher;
  Future<Void> Function(ModuleLauncherConfiguration)?      homeInsertPage;
  List<String>                                             coreModdingWorkerMessageFontFamily;
  List<List<core_modding_worker.ValueExpression>>          coreModdingWorkerSubmissionHistory;
  SettingState({
    required this.handleLaunch,
    required this.handleForward,
    required this.handleCommand,
    required this.handleLink,
    required this.applicationNavigatorKey,
    required this.themeFontFamliy,
    required this.homeShowLauncher,
    required this.homeInsertPage,
    required this.coreModdingWorkerMessageFontFamily,
    required this.coreModdingWorkerSubmissionHistory,
  });
}

class SettingProvider with ChangeNotifier {

  // #region constructor

  SettingData data;

  SettingState state;

  // ----------------

  SettingProvider(
  ) :
    this.data = _createDefaultData(),
    this.state = _createDefaultState();

  // #endregion

  // #region action

  Future<Void> reset(
  ) async {
    this.data = _createDefaultData();
    return;
  }

  Future<Void> apply(
  ) async {
    this.state.themeFontFamliy.clear();
    for (var index = 0; index < (!this.data.themeFontState ? 0 : this.data.themeFontPath.length); index++) {
      var family = await FontHelper.loadFile(this.data.themeFontPath[index]);
      if (family != null && !this.state.themeFontFamliy.contains(family)) {
        this.state.themeFontFamliy.add(family);
      }
    }
    this.state.coreModdingWorkerMessageFontFamily.clear();
    for (var index = 0; index < this.data.coreModdingWorker.messageFont.length; index++) {
      var family = await FontHelper.loadFile(this.data.coreModdingWorker.messageFont[index]);
      if (family != null && !this.state.coreModdingWorkerMessageFontFamily.contains(family)) {
        this.state.coreModdingWorkerMessageFontFamily.add(family);
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
    this.data = _parseDataFromJson(await JsonHelper.deserializeFile(file));
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
    await JsonHelper.serializeFile(file, _makeDataToJson(this.data));
    return;
  }

  // #endregion

  // #region utility

  static SettingData _createDefaultData(
  ) {
    return .new(
      version: ApplicationInformation.version,
      themeMode: .system,
      themeColorState: false,
      themeColorLight: .new(0xFF6200EE),
      themeColorDark: .new(0xFFBB86FC),
      themeFontState: false,
      themeFontPath: [],
      windowPositionState: false,
      windowPositionX: 0,
      windowPositionY: 0,
      windowSizeState: false,
      windowSizeWidth: 0,
      windowSizeHeight: 0,
      storagePickerFallbackDirectory: '',
      storagePickerHistoryLocation: {},
      forwarderDefaultTarget: .core_resource_shipper,
      forwarderImmediateJump: false,
      moduleConfigurationDirectory: '',
      moduleLauncher: .new(
        module: ModuleType.values.map((e) => ModuleLauncherConfiguration(title: ModuleHelper.query(e).name, type: e, option: [])).toList(),
        pinned: [],
        recent: [],
      ),
      coreModdingWorker: .new(
        kernel: '',
        script: '',
        argument: [],
        immediateLaunch: true,
        messageFont: [],
      ),
      coreCommandSender: .new(
        parallelForward: false,
      ),
      coreResourceShipper: .new(
        parallelForward: false,
        enableFilter: true,
        enableBatch: false,
      ),
      popcapAnimationViewer: .new(
        immediateSelect: true,
        automaticPlay: true,
        repeatPlay: true,
        keepSpeed: true,
        showBoundary: true,
      ),
      kairosoftGameManager: .new(
      ),
    );
  }

  static SettingState _createDefaultState(
  ) {
    return .new(
      handleLaunch: null,
      handleForward: null,
      handleCommand: null,
      handleLink: null,
      applicationNavigatorKey: .new(),
      themeFontFamliy: [],
      homeShowLauncher: null,
      homeInsertPage: null,
      coreModdingWorkerMessageFontFamily: [],
      coreModdingWorkerSubmissionHistory: core_modding_worker.SubmissionType.values.map((value) => <core_modding_worker.ValueExpression>[]).toList(),
    );
  }

  // ----------------

  static dynamic _makeDataToJson(
    SettingData data,
  ) {
    return {
      'version': data.version.selfAlso((it) => assertTest(it == ApplicationInformation.version)),
      'theme_mode': data.themeMode.name,
      'theme_color_state': data.themeColorState,
      'theme_color_light': data.themeColorLight.toARGB32(),
      'theme_color_dark': data.themeColorDark.toARGB32(),
      'theme_font_state': data.themeFontState,
      'theme_font_path': data.themeFontPath,
      'window_position_state': data.windowPositionState,
      'window_position_x': data.windowPositionX,
      'window_position_y': data.windowPositionY,
      'window_size_state': data.windowSizeState,
      'window_size_width': data.windowSizeWidth,
      'window_size_height': data.windowSizeHeight,
      'storage_picker_fallback_directory': data.storagePickerFallbackDirectory,
      'storage_picker_history_location': data.storagePickerHistoryLocation,
      'forwarder_default_target': data.forwarderDefaultTarget.name,
      'forwarder_immediate_jump': data.forwarderImmediateJump,
      'module_configuration_directory': data.moduleConfigurationDirectory,
      'module_launcher': {
        'module': data.moduleLauncher.module.map((dataItem) => {
          'title': dataItem.title,
          'type': dataItem.type.name,
          'option': dataItem.option,
        }).toList(),
        'pinned': data.moduleLauncher.pinned.map((dataItem) => {
          'title': dataItem.title,
          'type': dataItem.type.name,
          'option': dataItem.option,
        }).toList(),
        'recent': data.moduleLauncher.recent.map((dataItem) => {
          'title': dataItem.title,
          'type': dataItem.type.name,
          'option': dataItem.option,
        }).toList(),
      },
      'core_modding_worker': {
        'kernel': data.coreModdingWorker.kernel,
        'script': data.coreModdingWorker.script,
        'argument': data.coreModdingWorker.argument,
        'immediate_launch': data.coreModdingWorker.immediateLaunch,
        'message_font': data.coreModdingWorker.messageFont,
      },
      'core_command_sender': {
        'parallel_forward': data.coreCommandSender.parallelForward,
      },
      'core_resource_shipper': {
        'parallel_forward': data.coreResourceShipper.parallelForward,
        'enable_filter': data.coreResourceShipper.enableFilter,
        'enable_batch': data.coreResourceShipper.enableBatch,
      },
      'popcap_animation_viewer': {
        'immediate_select': data.popcapAnimationViewer.immediateSelect,
        'automatic_play': data.popcapAnimationViewer.automaticPlay,
        'repeat_play': data.popcapAnimationViewer.repeatPlay,
        'keep_speed': data.popcapAnimationViewer.keepSpeed,
        'show_boundary': data.popcapAnimationViewer.showBoundary,
      },
      'kairosoft_game_manager': {
      },
    };
  }

  static SettingData _parseDataFromJson(
    dynamic json,
  ) {
    return SettingData(
      version: (json['version'] as String).selfAlso((it) => assertTest(it == ApplicationInformation.version)),
      themeMode: (json['theme_mode'] as String).selfLet((it) => .values.byName(it)),
      themeColorState: (json['theme_color_state'] as Boolean),
      themeColorLight: (json['theme_color_light'] as Integer).selfLet((it) => .new(it)),
      themeColorDark: (json['theme_color_dark'] as Integer).selfLet((it) => .new(it)),
      themeFontState: (json['theme_font_state'] as Boolean),
      themeFontPath: (json['theme_font_path'] as List<dynamic>).cast<String>(),
      windowPositionState: (json['window_position_state'] as Boolean),
      windowPositionX: (json['window_position_x'] as Integer),
      windowPositionY: (json['window_position_y'] as Integer),
      windowSizeState: (json['window_size_state'] as Boolean),
      windowSizeWidth: (json['window_size_width'] as Integer),
      windowSizeHeight: (json['window_size_height'] as Integer),
      storagePickerFallbackDirectory: (json['storage_picker_fallback_directory'] as String),
      storagePickerHistoryLocation: (json['storage_picker_history_location'] as Map<dynamic, dynamic>).cast<String, String>(),
      forwarderDefaultTarget: (json['forwarder_default_target'] as String).selfLet((it) => .values.byName(it)),
      forwarderImmediateJump: (json['forwarder_immediate_jump'] as Boolean),
      moduleConfigurationDirectory: (json['module_configuration_directory'] as String),
      moduleLauncher: (json['module_launcher'] as Map<dynamic, dynamic>).selfLet((jsonPart) => ModuleLauncherSetting(
        module: (jsonPart['module'] as List<dynamic>).cast<Map<dynamic, dynamic>>().map((jsonItem) => ModuleLauncherConfiguration(
          title: (jsonItem['title'] as String),
          type: (jsonItem['type'] as String).selfLet((it) => .values.byName(it)),
          option: (jsonItem['option'] as List<dynamic>).cast<String>(),
        )).toList(),
        pinned: (jsonPart['pinned'] as List<dynamic>).cast<Map<dynamic, dynamic>>().map((jsonItem) => ModuleLauncherConfiguration(
          title: (jsonItem['title'] as String),
          type: (jsonItem['type'] as String).selfLet((it) => .values.byName(it)),
          option: (jsonItem['option'] as List<dynamic>).cast<String>(),
        )).toList(),
        recent: (jsonPart['recent'] as List<dynamic>).cast<Map<dynamic, dynamic>>().map((jsonItem) => ModuleLauncherConfiguration(
          title: (jsonItem['title'] as String),
          type: (jsonItem['type'] as String).selfLet((it) => .values.byName(it)),
          option: (jsonItem['option'] as List<dynamic>).cast<String>(),
        )).toList(),
      )),
      coreModdingWorker: (json['core_modding_worker'] as Map<dynamic, dynamic>).selfLet((jsonPart) => core_modding_worker.Setting(
        kernel: (jsonPart['kernel'] as String),
        script: (jsonPart['script'] as String),
        argument: (jsonPart['argument'] as List<dynamic>).cast<String>(),
        immediateLaunch: (jsonPart['immediate_launch'] as Boolean),
        messageFont: (jsonPart['message_font'] as List<dynamic>).cast<String>(),
      )),
      coreCommandSender: (json['core_command_sender'] as Map<dynamic, dynamic>).selfLet((jsonPart) => core_command_sender.Setting(
        parallelForward: (jsonPart['parallel_forward'] as Boolean),
      )),
      coreResourceShipper: (json['core_resource_shipper'] as Map<dynamic, dynamic>).selfLet((jsonPart) => core_resource_shipper.Setting(
        parallelForward: (jsonPart['parallel_forward'] as Boolean),
        enableFilter: (jsonPart['enable_filter'] as Boolean),
        enableBatch: (jsonPart['enable_batch'] as Boolean),
      )),
      popcapAnimationViewer: (json['popcap_animation_viewer'] as Map<dynamic, dynamic>).selfLet((jsonPart) => popcap_animation_viewer.Setting(
        immediateSelect: (jsonPart['immediate_select'] as Boolean),
        automaticPlay: (jsonPart['automatic_play'] as Boolean),
        repeatPlay: (jsonPart['repeat_play'] as Boolean),
        keepSpeed: (jsonPart['keep_speed'] as Boolean),
        showBoundary: (jsonPart['show_boundary'] as Boolean),
      )),
      kairosoftGameManager: (json['kairosoft_game_manager'] as Map<dynamic, dynamic>).selfLet((jsonPart) => kairosoft_game_manager.Setting(
      )),
    );
  }

  // #endregion

}
