import '/common.dart';
import '/module.dart';
import '/utility/storage_helper.dart';
import '/utility/json_helper.dart';
import '/utility/font_helper.dart';
import '/widget/export.dart';
import '/view/modding_worker/setting.dart' as modding_worker;
import '/view/modding_worker/submission_type.dart' as modding_worker;
import '/view/modding_worker/value_expression.dart' as modding_worker;
import '/view/command_sender/setting.dart' as command_sender;
import '/view/resource_shipper/setting.dart' as resource_shipper;
import '/view/animation_viewer/setting.dart' as animation_viewer;
import 'package:flutter/widgets.dart';

// ----------------

class SettingData {
  String                   version;
  StyledThemeMode          themeMode;
  Boolean                  themeColorState;
  Color                    themeColorLight;
  Color                    themeColorDark;
  Boolean                  themeFontState;
  List<String>             themeFontPath;
  Boolean                  windowPositionState;
  Integer                  windowPositionX;
  Integer                  windowPositionY;
  Boolean                  windowSizeState;
  Integer                  windowSizeWidth;
  Integer                  windowSizeHeight;
  String                   storagePickerFallbackDirectory;
  Map<String, String>      storagePickerHistoryLocation;
  ModuleType               forwarderDefaultTarget;
  Boolean                  forwarderImmediateJump;
  ModuleLauncherSetting    moduleLauncher;
  modding_worker.Setting   moddingWorker;
  command_sender.Setting   commandSender;
  resource_shipper.Setting resourceShipper;
  animation_viewer.Setting animationViewer;
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
    required this.moduleLauncher,
    required this.moddingWorker,
    required this.commandSender,
    required this.resourceShipper,
    required this.animationViewer,
  });
}

class SettingState {
  Future<Void> Function(String, ModuleType, List<String>)? handleLaunch;
  Future<Void> Function(List<String>)?                     handleForward;
  Future<Void> Function(List<String>)?                     handleCommand;
  Future<Void> Function(Uri)?                              handleLink;
  GlobalKey<NavigatorState>                                applicationNavigatorKey;
  List<String>                                             themeFontFamliy;
  Future<Void> Function()?                                 homeShowLauncherPanel;
  Future<Void> Function(ModuleLauncherConfiguration)?      homeInsertTabItem;
  List<String>                                             moddingWorkerMessageFontFamily;
  List<List<modding_worker.ValueExpression>>               moddingWorkerSubmissionHistory;
  SettingState({
    required this.handleLaunch,
    required this.handleForward,
    required this.handleCommand,
    required this.handleLink,
    required this.applicationNavigatorKey,
    required this.themeFontFamliy,
    required this.homeShowLauncherPanel,
    required this.homeInsertTabItem,
    required this.moddingWorkerMessageFontFamily,
    required this.moddingWorkerSubmissionHistory,
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
    this.state.moddingWorkerMessageFontFamily.clear();
    for (var index = 0; index < this.data.moddingWorker.messageFont.length; index++) {
      var family = await FontHelper.loadFile(this.data.moddingWorker.messageFont[index]);
      if (family != null && !this.state.moddingWorkerMessageFontFamily.contains(family)) {
        this.state.moddingWorkerMessageFontFamily.add(family);
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
    return SettingData(
      version: ApplicationInformation.version,
      themeMode: .system,
      themeColorState: false,
      themeColorLight: Color(0xFF6200EE),
      themeColorDark: Color(0xFFBB86FC),
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
      forwarderDefaultTarget: .resource_shipper,
      forwarderImmediateJump: false,
      moduleLauncher: ModuleLauncherSetting(
        module: ModuleType.values.map((e) => ModuleLauncherConfiguration(title: ModuleHelper.query(e).name, type: e, option: [])).toList(),
        pinned: [],
        recent: [],
      ),
      moddingWorker: modding_worker.Setting(
        kernel: '',
        script: '',
        argument: [],
        immediateLaunch: true,
        messageFont: [],
      ),
      commandSender: command_sender.Setting(
        methodConfiguration: '',
        parallelForward: false,
      ),
      resourceShipper: resource_shipper.Setting(
        optionConfiguration: '',
        parallelForward: false,
        enableFilter: true,
        enableBatch: false,
      ),
      animationViewer: animation_viewer.Setting(
        immediateSelect: true,
        automaticPlay: true,
        repeatPlay: true,
        keepSpeed: true,
        showBoundary: true,
      ),
    );
  }

  static SettingState _createDefaultState(
  ) {
    return SettingState(
      handleLaunch: null,
      handleForward: null,
      handleCommand: null,
      handleLink: null,
      applicationNavigatorKey: GlobalKey<NavigatorState>(),
      themeFontFamliy: [],
      homeShowLauncherPanel: null,
      homeInsertTabItem: null,
      moddingWorkerMessageFontFamily: [],
      moddingWorkerSubmissionHistory: modding_worker.SubmissionType.values.map((value) => <modding_worker.ValueExpression>[]).toList(),
    );
  }

  // ----------------

  static dynamic _makeDataToJson(
    SettingData data,
  ) {
    return {
      'version': data.version.selfAlso((it) { assertTest(it == ApplicationInformation.version); }),
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
      'modding_worker': {
        'kernel': data.moddingWorker.kernel,
        'script': data.moddingWorker.script,
        'argument': data.moddingWorker.argument,
        'immediate_launch': data.moddingWorker.immediateLaunch,
        'message_font': data.moddingWorker.messageFont,
      },
      'command_sender': {
        'method_configuration': data.commandSender.methodConfiguration,
        'parallel_forward': data.commandSender.parallelForward,
      },
      'resource_shipper': {
        'option_configuration': data.resourceShipper.optionConfiguration,
        'parallel_forward': data.resourceShipper.parallelForward,
        'enable_filter': data.resourceShipper.enableFilter,
        'enable_batch': data.resourceShipper.enableBatch,
      },
      'animation_viewer': {
        'immediate_select': data.animationViewer.immediateSelect,
        'automatic_play': data.animationViewer.automaticPlay,
        'repeat_play': data.animationViewer.repeatPlay,
        'keep_speed': data.animationViewer.keepSpeed,
        'show_boundary': data.animationViewer.showBoundary,
      },
    };
  }

  static SettingData _parseDataFromJson(
    dynamic json,
  ) {
    return SettingData(
      version: (json['version'] as String).selfAlso((it) { assertTest(it == ApplicationInformation.version); }),
      themeMode: (json['theme_mode'] as String).selfLet((it) => .values.byName(it)),
      themeColorState: (json['theme_color_state'] as Boolean),
      themeColorLight: (json['theme_color_light'] as Integer).selfLet((it) => Color(it)),
      themeColorDark: (json['theme_color_dark'] as Integer).selfLet((it) => Color(it)),
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
      moddingWorker: (json['modding_worker'] as Map<dynamic, dynamic>).selfLet((jsonPart) => modding_worker.Setting(
        kernel: (jsonPart['kernel'] as String),
        script: (jsonPart['script'] as String),
        argument: (jsonPart['argument'] as List<dynamic>).cast<String>(),
        immediateLaunch: (jsonPart['immediate_launch'] as Boolean),
        messageFont: (jsonPart['message_font'] as List<dynamic>).cast<String>(),
      )),
      commandSender: (json['command_sender'] as Map<dynamic, dynamic>).selfLet((jsonPart) => command_sender.Setting(
        methodConfiguration: (jsonPart['method_configuration'] as String),
        parallelForward: (jsonPart['parallel_forward'] as Boolean),
      )),
      resourceShipper: (json['resource_shipper'] as Map<dynamic, dynamic>).selfLet((jsonPart) => resource_shipper.Setting(
        optionConfiguration: (jsonPart['option_configuration'] as String),
        parallelForward: (jsonPart['parallel_forward'] as Boolean),
        enableFilter: (jsonPart['enable_filter'] as Boolean),
        enableBatch: (jsonPart['enable_batch'] as Boolean),
      )),
      animationViewer: (json['animation_viewer'] as Map<dynamic, dynamic>).selfLet((jsonPart) => animation_viewer.Setting(
        immediateSelect: (jsonPart['immediate_select'] as Boolean),
        automaticPlay: (jsonPart['automatic_play'] as Boolean),
        repeatPlay: (jsonPart['repeat_play'] as Boolean),
        keepSpeed: (jsonPart['keep_speed'] as Boolean),
        showBoundary: (jsonPart['show_boundary'] as Boolean),
      )),
    );
  }

  // #endregion

}
