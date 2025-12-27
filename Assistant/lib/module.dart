import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import 'widget/export.dart';
import '/view/core_task_worker/main_page.dart' as core_task_worker;
import '/view/core_task_worker/setting_panel.dart' as core_task_worker;
import '/view/core_task_worker/configuration.dart' as core_task_worker;
import '/view/core_command_sender/main_page.dart' as core_command_sender;
import '/view/core_command_sender/setting_panel.dart' as core_command_sender;
import '/view/core_command_sender/configuration.dart' as core_command_sender;
import '/view/core_resource_shipper/main_page.dart' as core_resource_shipper;
import '/view/core_resource_shipper/setting_panel.dart' as core_resource_shipper;
import '/view/core_resource_shipper/configuration.dart' as core_resource_shipper;
import '/view/popcap_animation_viewer/main_page.dart' as popcap_animation_viewer;
import '/view/popcap_animation_viewer/setting_panel.dart' as popcap_animation_viewer;
import '/view/popcap_animation_viewer/configuration.dart' as popcap_animation_viewer;
import '/view/kairosoft_game_manager/main_page.dart' as kairosoft_game_manager;
import '/view/kairosoft_game_manager/setting_panel.dart' as kairosoft_game_manager;
import '/view/kairosoft_game_manager/configuration.dart' as kairosoft_game_manager;
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

enum ModuleType {
  core_task_worker, // ignore: constant_identifier_names
  core_command_sender, // ignore: constant_identifier_names
  core_resource_shipper, // ignore: constant_identifier_names
  popcap_animation_viewer, // ignore: constant_identifier_names
  kairosoft_game_manager, // ignore: constant_identifier_names
}

class ModuleInformation {
  ModuleType                                    type;
  String                                        name;
  IconData                                      icon;
  Widget Function(Object, Object, List<String>) mainPage;
  Widget Function(BuildContext)                 settingPanel;
  Object Function(BuildContext)                 querySetting;
  Object Function(Object)                       parseConfiguration;
  Future<List<String>?> Function(List<String>)  generateForwardOption;
  ModuleInformation({
    required this.type,
    required this.name,
    required this.icon,
    required this.mainPage,
    required this.settingPanel,
    required this.querySetting,
    required this.parseConfiguration,
    required this.generateForwardOption,
  });
}

// ----------------

class ModuleLauncherConfiguration {
  String       title;
  ModuleType   type;
  List<String> option;
  ModuleLauncherConfiguration({
    required this.title,
    required this.type,
    required this.option,
  });
}

enum ModuleLauncherCategory {
  module,
  pinned,
  recent,
}

class ModuleLauncherSetting {
  List<ModuleLauncherConfiguration> module;
  List<ModuleLauncherConfiguration> pinned;
  List<ModuleLauncherConfiguration> recent;
  ModuleLauncherSetting({
    required this.module,
    required this.pinned,
    required this.recent,
  });
}

// ----------------

class ModuleHelper {

  static final List<ModuleInformation> _information = [
    .new(
      type: .core_task_worker,
      name: 'Task Worker',
      icon: IconSet.rule_settings,
      mainPage: (setting, configuration, option) => core_task_worker.MainPage(
        key: GlobalKey(),
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      settingPanel: (context) => core_task_worker.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.coreTaskWorker,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.coreTaskWorker,
      parseConfiguration: (json) => core_task_worker.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async => ['-additional_argument', ...resource],
    ),
    .new(
      type: .core_command_sender,
      name: 'Command Sender',
      icon: IconSet.send,
      mainPage: (setting, configuration, option) => core_command_sender.MainPage(
        key: GlobalKey(),
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      settingPanel: (context) => core_command_sender.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.coreCommandSender,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.coreCommandSender,
      parseConfiguration: (json) => core_command_sender.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async => null,
    ),
    .new(
      type: .core_resource_shipper,
      name: 'Resource Shipper',
      icon: IconSet.share_windows,
      mainPage: (setting, configuration, option) => core_resource_shipper.MainPage(
        key: GlobalKey(),
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      settingPanel: (context) => core_resource_shipper.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.coreResourceShipper,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.coreResourceShipper,
      parseConfiguration: (json) => core_resource_shipper.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async => ['-resource', ...resource],
    ),
    .new(
      type: .popcap_animation_viewer,
      name: 'PopCap Animation Viewer',
      icon: IconSet.thread_unread,
      mainPage: (setting, configuration, option) => popcap_animation_viewer.MainPage(
        key: GlobalKey(),
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      settingPanel: (context) => popcap_animation_viewer.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.popcapAnimationViewer,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.popcapAnimationViewer,
      parseConfiguration: (json) => popcap_animation_viewer.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async => resource.length != 1 || !RegExp(r'(\.pam\.json)$', caseSensitive: false).hasMatch(resource.first) || !await StorageHelper.existFile(resource.first) ? null : ['-animation_file', resource.first],
    ),
    .new(
      type: .kairosoft_game_manager,
      name: 'Kairosoft Game Manager',
      icon: IconSet.history_edu,
      mainPage: (setting, configuration, option) => kairosoft_game_manager.MainPage(
        key: GlobalKey(),
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      settingPanel: (context) => kairosoft_game_manager.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.kairosoftGameManager,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.kairosoftGameManager,
      parseConfiguration: (json) => kairosoft_game_manager.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async => null,
    ),
  ];

  static ModuleInformation query(
    ModuleType type,
  ) {
    return _information[type.index];
  }

  // ----------------

  static Boolean compareLauncher(
    ModuleLauncherConfiguration thix,
    ModuleLauncherConfiguration that,
  ) {
    return thix.type == that.type && thix.option.equals(that.option);
  }

  static List<String> generateArgument(
    ModuleLauncherConfiguration launcher,
  ) {
    return [
      '-launch',
      launcher.title,
      launcher.type.name,
      ...launcher.option,
    ];
  }

}
