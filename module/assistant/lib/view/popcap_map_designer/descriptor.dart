import '/common.dart'; // ignore: unused_import
import '/module.dart';
import '/widget/export.dart';
import '/view/popcap_map_designer/main_page.dart';
import '/view/popcap_map_designer/setting_panel.dart';
import '/view/popcap_map_designer/setting.dart';
import '/view/popcap_map_designer/configuration.dart';
import '/view/popcap_map_designer/option.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: true, // TODO: draft
  type: .popcapMapDesigner,
  name: 'PopCap Map Designer',
  icon: IconSet.add,
  buildMainPage: (key, setting, configuration, option) => .new(key: key, setting: setting, configuration: configuration, option: option),
  buildSettingPanel: (data, onUpdate) => .new(data: data, onUpdate: onUpdate),
  makeSetting: (data) => {
  },
  parseSetting: (json) => Setting(
  ),
  generateDefaultSetting: () => Setting(
  ),
  parseConfiguration: (json) => Configuration(
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
