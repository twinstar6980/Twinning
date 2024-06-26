import '/common.dart';
import '/view/modding_worker/main_page.dart' as modding_worker;
import '/view/modding_worker/setting_panel.dart' as modding_worker;
import '/view/command_sender/main_page.dart' as command_sender;
import '/view/command_sender/setting_panel.dart' as command_sender;
import '/view/resource_forwarder/main_page.dart' as resource_forwarder;
import '/view/resource_forwarder/setting_panel.dart' as resource_forwarder;
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';

// ----------------

enum ModuleType {
  modding_worker, // ignore: constant_identifier_names
  command_sender, // ignore: constant_identifier_names
  resource_forwarder, // ignore: constant_identifier_names
}

class ModuleInformation {
  ModuleType                    type;
  IconData                      icon;
  String                        name;
  Widget Function(List<String>) mainPage;
  Widget Function()             settingPanel;
  ModuleInformation({
    required this.type,
    required this.icon,
    required this.name,
    required this.mainPage,
    required this.settingPanel,
  });
}

// ----------------

class ModuleLauncherConfiguration {
  String            title;
  ModuleType        type;
  List<String>      option;
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
  List<ModuleLauncherConfiguration> mModule;
  List<ModuleLauncherConfiguration> mPinned;
  List<ModuleLauncherConfiguration> mRecent;
  ModuleLauncherSetting({
    required this.mModule,
    required this.mPinned,
    required this.mRecent,
  });
}

// ----------------

class ModuleHelper {

  static List<ModuleInformation> information = [
    ModuleInformation(
      type: ModuleType.modding_worker,
      icon: IconSymbols.rule_settings,
      name: 'Modding Worker',
      mainPage: (option) => modding_worker.MainPage(option: option),
      settingPanel: () => const modding_worker.SettingPanel(),
    ),
    ModuleInformation(
      type: ModuleType.command_sender,
      icon: IconSymbols.send,
      name: 'Command Sender',
      mainPage: (option) => command_sender.MainPage(option: option),
      settingPanel: () => const command_sender.SettingPanel(),
    ),
    ModuleInformation(
      type: ModuleType.resource_forwarder,
      icon: IconSymbols.share_windows,
      name: 'Resource Forwarder',
      mainPage: (option) => resource_forwarder.MainPage(option: option),
      settingPanel: () => const resource_forwarder.SettingPanel(),
    ),
  ];

  static ModuleInformation query(
    ModuleType type,
  ) {
    return information[type.index];
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
      '-module_type',
      launcher.type.name,
      '-module_option',
      ...launcher.option,
    ];
  }

}
