import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import 'widget/export.dart';
import '/view/modding_worker/main_page.dart' as modding_worker;
import '/view/modding_worker/setting_panel.dart' as modding_worker;
import '/view/command_sender/main_page.dart' as command_sender;
import '/view/command_sender/setting_panel.dart' as command_sender;
import '/view/resource_shipper/main_page.dart' as resource_shipper;
import '/view/resource_shipper/setting_panel.dart' as resource_shipper;
import '/view/animation_viewer/main_page.dart' as animation_viewer;
import '/view/animation_viewer/setting_panel.dart' as animation_viewer;
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

enum ModuleType {
  modding_worker, // ignore: constant_identifier_names
  command_sender, // ignore: constant_identifier_names
  resource_shipper, // ignore: constant_identifier_names
  animation_viewer, // ignore: constant_identifier_names
}

class ModuleInformation {
  ModuleType                                   type;
  String                                       name;
  IconData                                     icon;
  Widget Function(List<String>)                mainPage;
  Widget Function(BuildContext)                settingPanel;
  Future<List<String>?> Function(List<String>) generateForwardOption;
  ModuleInformation({
    required this.type,
    required this.name,
    required this.icon,
    required this.mainPage,
    required this.settingPanel,
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
      type: .modding_worker,
      name: 'Modding Worker',
      icon: IconSet.rule_settings,
      mainPage: (option) => modding_worker.MainPage(
        key: GlobalKey(),
        option: option,
      ),
      settingPanel: (context) => modding_worker.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.moddingWorker,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      generateForwardOption: (resource) async => ['-additional_argument', ...resource],
    ),
    .new(
      type: .command_sender,
      name: 'Command Sender',
      icon: IconSet.send,
      mainPage: (option) => command_sender.MainPage(
        key: GlobalKey(),
        option: option,
      ),
      settingPanel: (context) => command_sender.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.commandSender,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      generateForwardOption: (resource) async => null,
    ),
    .new(
      type: .resource_shipper,
      name: 'Resource Shipper',
      icon: IconSet.share_windows,
      mainPage: (option) => resource_shipper.MainPage(
        key: GlobalKey(),
        option: option,
      ),
      settingPanel: (context) => resource_shipper.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.resourceShipper,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      generateForwardOption: (resource) async => ['-resource', ...resource],
    ),
    .new(
      type: .animation_viewer,
      name: 'Animation Viewer',
      icon: IconSet.thread_unread,
      mainPage: (option) => animation_viewer.MainPage(
        key: GlobalKey(),
        option: option,
      ),
      settingPanel: (context) => animation_viewer.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.animationViewer,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      generateForwardOption: (resource) async => resource.length != 1 || !RegExp(r'(\.pam\.json)$', caseSensitive: false).hasMatch(resource.first) || !await StorageHelper.existFile(resource.first) ? null : ['-animation_file', resource.first],
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
