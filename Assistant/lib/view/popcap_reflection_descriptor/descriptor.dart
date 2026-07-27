import '/common.dart'; // ignore: unused_import
import '/module.dart';
import '/widget/export.dart';
import '/view/popcap_reflection_descriptor/main_page.dart';
import '/view/popcap_reflection_descriptor/setting_panel.dart';
import '/view/popcap_reflection_descriptor/setting.dart';
import '/view/popcap_reflection_descriptor/configuration.dart';
import '/view/popcap_reflection_descriptor/option.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: true, // TODO: draft
  type: .popcapReflectionDescriptor,
  name: 'PopCap Reflection Descriptor',
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
