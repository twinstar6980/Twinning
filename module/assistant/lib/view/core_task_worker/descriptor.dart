import '/common.dart';
import '/module.dart';
import '/utility/json_type.dart';
import '/utility/storage_path.dart';
import '/widget/export.dart';
import '/view/core_task_worker/main_page.dart';
import '/view/core_task_worker/setting_panel.dart';
import '/view/core_task_worker/setting.dart';
import '/view/core_task_worker/configuration.dart';
import '/view/core_task_worker/option.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: false,
  type: .coreTaskWorker,
  name: 'Task Worker',
  icon: IconSet.rule_settings,
  buildMainPage: (key, setting, configuration, option) => .new(key: key, setting: setting, configuration: configuration, option: option),
  buildSettingPanel: (data, onUpdate) => .new(data: data, onUpdate: onUpdate),
  makeSetting: (data) => {
    'kernel': data.kernel.emit(),
    'script': data.script.emit(),
    'argument': data.argument,
    'immediate_launch': data.immediateLaunch,
  },
  parseSetting: (json) => Setting(
    kernel: json.jsonIn('kernel').jsonString().selfLet((it) => StoragePath.of(it)),
    script: json.jsonIn('script').jsonString().selfLet((it) => StoragePath.of(it)),
    argument: json.jsonIn('argument').jsonArray().cast<String>(),
    immediateLaunch: json.jsonIn('immediate_launch').jsonBoolean(),
  ),
  generateDefaultSetting: () => Setting(
    kernel: .new(),
    script: .new(),
    argument: [],
    immediateLaunch: true,
  ),
  parseConfiguration: (json) => Configuration(
  ),
  makeOption: (data) => {
    'immediate_launch': data.immediateLaunch,
    'additional_argument': data.additionalArgument,
  },
  parseOption: (json) => Option(
    immediateLaunch: json.jsonInOr('immediate_launch')?.jsonBoolean(),
    additionalArgument: json.jsonInOr('additional_argument')?.jsonArray().cast<String>(),
  ),
  generateDefaultOption: () => Option(
    immediateLaunch: null,
    additionalArgument: null,
  ),
  generateForwardOption: (resource) async {
    return .new(
      additionalArgument: resource.map((it) => it.emit()).toList(),
    );
  },
);
