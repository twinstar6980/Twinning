import '/common.dart';
import '/module.dart';
import '/utility/json_type.dart';
import '/utility/storage_path.dart';
import '/widget/export.dart';
import '/view/kairosoft_game_manager/main_page.dart';
import '/view/kairosoft_game_manager/setting_panel.dart';
import '/view/kairosoft_game_manager/setting.dart';
import '/view/kairosoft_game_manager/configuration.dart';
import '/view/kairosoft_game_manager/option.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: false,
  type: .kairosoftGameManager,
  name: 'Kairosoft Game Manager',
  icon: IconSet.history_edu,
  buildMainPage: (key, setting, configuration, option) => .new(key: key, setting: setting, configuration: configuration, option: option),
  buildSettingPanel: (data, onUpdate) => .new(data: data, onUpdate: onUpdate),
  makeSetting: (data) => {
    'repository_of_windows_steam': data.repositoryOfWindowsSteam.emit(),
  },
  parseSetting: (json) => Setting(
    repositoryOfWindowsSteam: json.jsonIn('repository_of_windows_steam').jsonString().selfLet((it) => StoragePath.of(it)),
  ),
  generateDefaultSetting: () => Setting(
    repositoryOfWindowsSteam: .of('C:/Program Files (x86)/Steam'),
  ),
  parseConfiguration: (json) => Configuration(
    game: json.jsonIn('game').jsonArray().map((json) => GameConfiguration(
      identifier: json.jsonIn('identifier').jsonString(),
      name: json.jsonIn('name').jsonString(),
      packageIdentifierOnSteam: json.jsonIn('package_identifier_on_steam')?.jsonString(),
      packageIdentifierOnAndroid: json.jsonIn('package_identifier_on_android')?.jsonString(),
    )).toList(),
  ),
  makeOption: (data) => {
  },
  parseOption: (json) => Option(
  ),
  generateDefaultOption: () => Option(
  ),
  generateForwardOption: (resource) async {
    return null;
  },
);
