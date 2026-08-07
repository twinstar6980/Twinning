import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/json_type.dart';
import '/utility/json_helper.dart';
import '/utility/application_font_manager.dart';
import '/widget/export.dart';
import '/view/core_task_worker/setting.dart' as core_task_worker;
import '/view/core_task_worker/submission_type.dart' as core_task_worker;
import '/view/core_task_worker/value_expression.dart' as core_task_worker;

// ----------------

class SettingData {
  String                                                         version;
  StyledThemeMode                                                themeMode;
  Boolean                                                        themeColorState;
  Color                                                          themeColorLight;
  Color                                                          themeColorDark;
  Boolean                                                        themeFontState;
  List<StoragePath>                                              themeFontPath;
  Integer                                                        windowSizeWidth;
  Integer                                                        windowSizeHeight;
  ModuleType                                                     forwarderDefaultTarget;
  Boolean                                                        forwarderImmediateJump;
  StoragePath                                                    moduleConfigurationDirectory;
  Map<String, StoragePath>                                       storagePickerHistoryLocation;
  Map<ModuleLauncherCategory, List<ModuleLauncherConfiguration>> moduleLauncher;
  Map<ModuleType, Object>                                        moduleSetting;
  SettingData({
    required this.version,
    required this.themeMode,
    required this.themeColorState,
    required this.themeColorLight,
    required this.themeColorDark,
    required this.themeFontState,
    required this.themeFontPath,
    required this.windowSizeWidth,
    required this.windowSizeHeight,
    required this.forwarderDefaultTarget,
    required this.forwarderImmediateJump,
    required this.moduleConfigurationDirectory,
    required this.storagePickerHistoryLocation,
    required this.moduleLauncher,
    required this.moduleSetting,
  });
}

class SettingState {
  Future<Void> Function(String title, ModuleType type, Object option)?                             handleLaunch;
  Future<Void> Function(List<StoragePath> resource)?                                               handleForward;
  Future<Void> Function(List<String> command)?                                                     handleCommand;
  Future<Void> Function(Uri link)?                                                                 handleLink;
  GlobalKey<NavigatorState>                                                                        applicationNavigatorKey;
  List<String>                                                                                     themeFontFamliy;
  Future<Void> Function()?                                                                         homeShowOnboarding;
  Future<Void> Function()?                                                                         homeShowLauncher;
  Future<Void> Function(GlobalKey key, ModuleLauncherConfiguration configuration, Boolean silent)? homeInsertPage;
  Future<Void> Function(GlobalKey key)?                                                            homeRemovePage;
  List<List<core_task_worker.ValueExpression>>                                                     coreTaskWorkerSubmissionHistory;
  SettingState({
    required this.handleLaunch,
    required this.handleForward,
    required this.handleCommand,
    required this.handleLink,
    required this.applicationNavigatorKey,
    required this.themeFontFamliy,
    required this.homeShowOnboarding,
    required this.homeShowLauncher,
    required this.homeInsertPage,
    required this.homeRemovePage,
    required this.coreTaskWorkerSubmissionHistory,
  });
}

class SettingProvider with ChangeNotifier {

  // #region constructor

  SettingData data;

  SettingState state;

  // ----------------

  SettingProvider(
  ) :
    this.data = SettingProvider._createDefaultData(),
    this.state = SettingProvider._createDefaultState();

  // #endregion

  // #region action

  Future<Void> reset(
  ) async {
    this.data = SettingProvider._createDefaultData();
    return;
  }

  Future<Void> apply(
  ) async {
    this.state.themeFontFamliy.clear();
    for (var index = 0; index < (!this.data.themeFontState ? 0 : this.data.themeFontPath.length); index++) {
      var family = await ApplicationFontManager.instance.loadFile(this.data.themeFontPath[index]);
      if (family != null && !this.state.themeFontFamliy.contains(family)) {
        this.state.themeFontFamliy.add(family);
      }
    }
    this.notifyListeners();
    return;
  }

  // #endregion

  // #region storage

  Future<StoragePath> file(
  ) async {
    return (await StorageHelper.query(.applicationShared)).join('setting.json');
  }

  // ----------------

  Future<Void> load({
    StoragePath? file = null,
  }) async {
    if (file == null) {
      file = await this.file();
    }
    this.data = SettingProvider._parseDataFromJson((await JsonHelper.decodeFile(file)).jsonObject());
    return;
  }

  Future<Void> save({
    StoragePath? file = null,
    Boolean      apply = true,
  }) async {
    if (file == null) {
      file = await this.file();
    }
    if (apply) {
      await this.apply();
    }
    if (!await StorageHelper.existFile(file)) {
      await StorageHelper.createFile(file);
    }
    await JsonHelper.encodeFile(file, SettingProvider._makeDataToJson(this.data));
    return;
  }

  // #endregion

  // #region utility

  static SettingProvider of(
    BuildContext context, {
    Boolean listen = true,
  }) {
    return Provider.of<SettingProvider>(context, listen: listen);
  }

  // ----------------

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
      windowSizeWidth: 480,
      windowSizeHeight: 840,
      forwarderDefaultTarget: .coreResourceShipper,
      forwarderImmediateJump: false,
      moduleConfigurationDirectory: .new(),
      storagePickerHistoryLocation: {},
      moduleLauncher: {
        for (var category in ModuleLauncherCategory.values) ...{
          if (category == .module) ...{
            category: ModuleType.values.map(ModuleHelper.query).where((it) => !it.draft).map((it) => ModuleLauncherConfiguration(
              title: it.name,
              type: it.type,
              option: it.generateDefaultOption(),
            )).toList(),
          },
          if (category != .module) ...{
            category: [],
          },
        },
      },
      moduleSetting: {
        for (var type in ModuleType.values) ...{
          type: ModuleHelper.query(type).generateDefaultSetting(),
        },
      },
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
      homeShowOnboarding: null,
      homeShowLauncher: null,
      homeInsertPage: null,
      homeRemovePage: null,
      coreTaskWorkerSubmissionHistory: core_task_worker.SubmissionType.values.map((value) => <core_task_worker.ValueExpression>[]).toList(),
    );
  }

  // ----------------

  static JsonObject _makeDataToJson(
    SettingData data,
  ) {
    return {
      'version': data.version.selfAlso((it) => assertTest(it == ApplicationInformation.version)),
      'theme_mode': data.themeMode.selfLet((it) => ConvertHelper.makeEnumerationToStringOfSnakeCase(it)),
      'theme_color_state': data.themeColorState,
      'theme_color_light': data.themeColorLight.toARGB32(),
      'theme_color_dark': data.themeColorDark.toARGB32(),
      'theme_font_state': data.themeFontState,
      'theme_font_path': data.themeFontPath.map((it) => it.emit()).toList(),
      'window_size_width': data.windowSizeWidth,
      'window_size_height': data.windowSizeHeight,
      'forwarder_default_target': data.forwarderDefaultTarget.selfLet((it) => ModuleHelper.query(it).identifier),
      'forwarder_immediate_jump': data.forwarderImmediateJump,
      'module_configuration_directory': data.moduleConfigurationDirectory.emit(),
      'storage_picker_history_location': data.storagePickerHistoryLocation.map((key, value) => .new(key, value.emit())),
      'module_launcher': data.moduleLauncher.selfLet((data) => {
        for (var category in ModuleLauncherCategory.values) ...{
          category.name.jsonKey(): data[category]!.map((data) => {
            'title': data.title,
            'type': data.type.selfLet((it) => ModuleHelper.query(it).identifier),
            'option': data.option.selfLet((it) => ModuleHelper.query(data.type).makeOption(it)),
          }).toList(),
        },
      }),
      'module_setting': data.moduleSetting.selfLet((data) => {
        for (var type in ModuleType.values) ...{
          type.name.jsonKey(): data[type]!.selfLet((data) => ModuleHelper.query(type).makeSetting(data)),
        },
      }),
    };
  }

  static SettingData _parseDataFromJson(
    JsonObject json,
  ) {
    return SettingData(
      version: json.jsonIn('version').jsonString().selfAlso((it) => assertTest(it == ApplicationInformation.version)),
      themeMode: json.jsonIn('theme_mode').jsonString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, StyledThemeMode.values)),
      themeColorState: json.jsonIn('theme_color_state').jsonBoolean(),
      themeColorLight: json.jsonIn('theme_color_light').jsonInteger().selfLet((it) => .new(it)),
      themeColorDark: json.jsonIn('theme_color_dark').jsonInteger().selfLet((it) => .new(it)),
      themeFontState: json.jsonIn('theme_font_state').jsonBoolean(),
      themeFontPath: json.jsonIn('theme_font_path').jsonArray().cast<String>().map((it) => StoragePath.of(it)).toList(),
      windowSizeWidth: json.jsonIn('window_size_width').jsonInteger(),
      windowSizeHeight: json.jsonIn('window_size_height').jsonInteger(),
      forwarderDefaultTarget: json.jsonIn('forwarder_default_target').jsonString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, ModuleType.values)),
      forwarderImmediateJump: json.jsonIn('forwarder_immediate_jump').jsonBoolean(),
      moduleConfigurationDirectory: json.jsonIn('module_configuration_directory').jsonString().selfLet((it) => StoragePath.of(it)),
      storagePickerHistoryLocation: json.jsonIn('storage_picker_history_location').jsonObject().cast<String, String>().map((key, value) => .new(key, StoragePath.of(value))),
      moduleLauncher: json.jsonIn('module_launcher').jsonObject().selfLet((json) => {
        for (var category in ModuleLauncherCategory.values) ...{
          category: json.jsonIn(category.name.jsonKey()).jsonArray().map((json) => ModuleLauncherConfiguration(
            title: json.jsonIn('title').jsonString(),
            type: json.jsonIn('type').jsonString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, ModuleType.values)),
            option: json.jsonIn('option').jsonObject().selfLet((it) => ModuleHelper.query(json.jsonIn('type').jsonString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, ModuleType.values))).parseOption(it)),
          )).toList(),
        },
      }),
      moduleSetting: json.jsonIn('module_setting').jsonObject().selfLet((json) => {
        for (var type in ModuleType.values) ...{
          type: ModuleHelper.query(type).parseSetting(json.jsonIn(type.name.jsonKey()).jsonObject()),
        },
      }),
    );
  }

  // ----------------

  Future<Void> quickSetup(
    StoragePath homeDirectory,
  ) async {
    this.data.selfAlso((data) {
      data.moduleConfigurationDirectory = homeDirectory.join('assistant');
      data.moduleSetting[ModuleType.coreTaskWorker]!.as<core_task_worker.Setting>().selfAlso((data) {
        data.kernel = homeDirectory.join('kernel');
        data.script = homeDirectory.join('script').join('main.js');
        data.argument = [homeDirectory.emit()];
      });
    });
    return;
  }

  // #endregion

}
