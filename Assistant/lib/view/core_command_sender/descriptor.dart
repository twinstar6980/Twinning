import '/common.dart';
import '/module.dart';
import '/utility/json_type.dart';
import '/utility/convert_helper.dart';
import '/widget/export.dart';
import '/view/core_command_sender/main_page.dart';
import '/view/core_command_sender/setting_panel.dart';
import '/view/core_command_sender/setting.dart';
import '/view/core_command_sender/configuration.dart';
import '/view/core_command_sender/option.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: false,
  type: .coreCommandSender,
  name: 'Command Sender',
  icon: IconSet.send,
  buildMainPage: (key, setting, configuration, option) => .new(key: key, setting: setting, configuration: configuration, option: option),
  buildSettingPanel: (data, onUpdate) => .new(data: data, onUpdate: onUpdate),
  makeSetting: (data) => {
    'parallel_forward': data.parallelForward,
  },
  parseSetting: (json) => Setting(
    parallelForward: json.jsonIn('parallel_forward').jsonBoolean(),
  ),
  generateDefaultSetting: () => Setting(
    parallelForward: false,
  ),
  parseConfiguration: (json) => Configuration(
    method: json.jsonIn('method').jsonArray().map((json) => MethodGroupConfiguration(
      identifier: json.jsonIn('identifier').jsonString(),
      name: json.jsonIn('name').jsonString(),
      icon: json.jsonIn('icon').jsonString(),
      item: json.jsonIn('item').jsonArray().map((json) => MethodConfiguration(
        identifier: json.jsonIn('identifier').jsonString(),
        name: json.jsonIn('name').jsonString(),
        icon: json.jsonIn('icon').jsonString(),
        argument: json.jsonIn('argument').jsonArray().map((json) => ArgumentConfiguration(
          identifier: json.jsonIn('identifier').jsonString(),
          name: json.jsonIn('name').jsonString(),
          type: json.jsonIn('type').jsonString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, ArgumentType.values)),
          option: json.jsonIn('option')?.jsonArray().cast<Object>(),
        )).toList(),
        batch: json.jsonIn('batch')?.jsonArray().cast<String>(),
        preset: json.jsonIn('preset').jsonArray().map((json) => json == null ? null : PresetConfiguration(
          name: json.jsonIn('name').jsonString(),
          argument: json.jsonIn('argument').jsonObject().cast<String, Object>(),
        )).toList(),
      )).toList(),
    )).toList(),
  ),
  makeOption: (data) => {
    'parallel_forward': data.parallelForward,
    'command': data.command?.map((data) => {
      'method': data.method,
      'batch': data.batch,
      'argument': data.argument,
      'expanded': data.expanded,
    }).toList(),
  },
  parseOption: (json) => Option(
    parallelForward: json.jsonInOr('parallel_forward')?.jsonBoolean(),
    command: json.jsonInOr('command')?.jsonArray().map((json) => (
      method: json.jsonIn('method').jsonString(),
      batch: json.jsonIn('batch').jsonBoolean(),
      argument: json.jsonIn('argument').jsonObject(),
      expanded: json.jsonIn('expanded').jsonBoolean(),
    )).toList(),
  ),
  generateDefaultOption: () => Option(
    parallelForward: null,
    command: null,
  ),
  generateForwardOption: (resource) async {
    return null;
  },
);
