import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/core_task_worker/main_page.dart' as core_task_worker;
import '/view/core_task_worker/setting_panel.dart' as core_task_worker;
import '/view/core_task_worker/configuration.dart' as core_task_worker;
import '/view/core_command_sender/main_page.dart' as core_command_sender;
import '/view/core_command_sender/setting_panel.dart' as core_command_sender;
import '/view/core_command_sender/configuration.dart' as core_command_sender;
import '/view/core_resource_shipper/main_page.dart' as core_resource_shipper;
import '/view/core_resource_shipper/setting_panel.dart' as core_resource_shipper;
import '/view/core_resource_shipper/configuration.dart' as core_resource_shipper;
import '/view/popcap_reflection_descriptor/main_page.dart' as popcap_reflection_descriptor;
import '/view/popcap_reflection_descriptor/setting_panel.dart' as popcap_reflection_descriptor;
import '/view/popcap_reflection_descriptor/configuration.dart' as popcap_reflection_descriptor;
import '/view/popcap_animation_viewer/main_page.dart' as popcap_animation_viewer;
import '/view/popcap_animation_viewer/setting_panel.dart' as popcap_animation_viewer;
import '/view/popcap_animation_viewer/configuration.dart' as popcap_animation_viewer;
import '/view/popcap_animation_viewer/visual_helper.dart' as popcap_animation_viewer;
import '/view/popcap_map_designer/main_page.dart' as popcap_map_designer;
import '/view/popcap_map_designer/setting_panel.dart' as popcap_map_designer;
import '/view/popcap_map_designer/configuration.dart' as popcap_map_designer;
import '/view/popcap_package_previewer/main_page.dart' as popcap_package_previewer;
import '/view/popcap_package_previewer/setting_panel.dart' as popcap_package_previewer;
import '/view/popcap_package_previewer/configuration.dart' as popcap_package_previewer;
import '/view/popcap_package_builder/main_page.dart' as popcap_package_builder;
import '/view/popcap_package_builder/setting_panel.dart' as popcap_package_builder;
import '/view/popcap_package_builder/configuration.dart' as popcap_package_builder;
import '/view/kairosoft_game_manager/main_page.dart' as kairosoft_game_manager;
import '/view/kairosoft_game_manager/setting_panel.dart' as kairosoft_game_manager;
import '/view/kairosoft_game_manager/configuration.dart' as kairosoft_game_manager;
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

enum ModuleType {
  coreTaskWorker,
  coreCommandSender,
  coreResourceShipper,
  popcapReflectionDescriptor,
  popcapAnimationViewer,
  popcapMapDesigner,
  popcapPackagePreviewer,
  popcapPackageBuilder,
  kairosoftGameManager,
}

class ModuleInformation {
  Boolean                                                                                   draft;
  ModuleType                                                                                type;
  String                                                                                    identifier;
  String                                                                                    name;
  IconData                                                                                  icon;
  Widget Function(GlobalKey key, Object setting, Object configuration, List<String> option) buildMainPage;
  Widget Function(BuildContext context)                                                     buildSettingPanel;
  Object Function(BuildContext context)                                                     querySetting;
  Object Function(Object json)                                                              parseConfiguration;
  Future<List<String>?> Function(List<StoragePath> resource)                                generateForwardOption;
  ModuleInformation({
    required this.draft,
    required this.type,
    required this.identifier,
    required this.name,
    required this.icon,
    required this.buildMainPage,
    required this.buildSettingPanel,
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
      draft: false,
      type: .coreTaskWorker,
      identifier: ModuleType.coreTaskWorker.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'Task Worker',
      icon: IconSet.rule_settings,
      buildMainPage: (key, setting, configuration, option) => core_task_worker.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => core_task_worker.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.coreTaskWorker,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.coreTaskWorker,
      parseConfiguration: (json) => core_task_worker.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return ['-additional_argument', ...resource.map((it) => it.emit())];
      },
    ),
    .new(
      draft: false,
      type: .coreCommandSender,
      identifier: ModuleType.coreCommandSender.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'Command Sender',
      icon: IconSet.send,
      buildMainPage: (key, setting, configuration, option) => core_command_sender.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => core_command_sender.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.coreCommandSender,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.coreCommandSender,
      parseConfiguration: (json) => core_command_sender.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return null;
      },
    ),
    .new(
      draft: false,
      type: .coreResourceShipper,
      identifier: ModuleType.coreResourceShipper.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'Resource Shipper',
      icon: IconSet.share_windows,
      buildMainPage: (key, setting, configuration, option) => core_resource_shipper.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => core_resource_shipper.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.coreResourceShipper,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.coreResourceShipper,
      parseConfiguration: (json) => core_resource_shipper.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        for (var resourceItem in resource) {
          if (!await StorageHelper.exist(resourceItem)) {
            return null;
          }
        }
        return ['-resource', ...resource.map((it) => it.emit())];
      },
    ),
    .new(
      draft: true, // TODO: draft
      type: .popcapReflectionDescriptor,
      identifier: ModuleType.popcapReflectionDescriptor.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'PopCap Reflection Descriptor',
      icon: IconSet.add,
      buildMainPage: (key, setting, configuration, option) => popcap_reflection_descriptor.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => popcap_reflection_descriptor.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.popcapReflectionDescriptor,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.popcapReflectionDescriptor,
      parseConfiguration: (json) => popcap_reflection_descriptor.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return null;
      },
    ),
    .new(
      draft: false,
      type: .popcapAnimationViewer,
      identifier: ModuleType.popcapAnimationViewer.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'PopCap Animation Viewer',
      icon: IconSet.thread_unread,
      buildMainPage: (key, setting, configuration, option) => popcap_animation_viewer.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => popcap_animation_viewer.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.popcapAnimationViewer,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.popcapAnimationViewer,
      parseConfiguration: (json) => popcap_animation_viewer.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        if (resource.length != 1) {
          return null;
        }
        var animationFile = null as StoragePath?;
        if (await StorageHelper.existFile(resource.first)) {
          animationFile = await popcap_animation_viewer.VisualHelper.checkAnimationFilePath(resource.first);
        }
        if (await StorageHelper.existDirectory(resource.first)) {
          animationFile = await popcap_animation_viewer.VisualHelper.checkAnimationDirectoryPath(resource.first);
        }
        if (animationFile == null) {
          return null;
        }
        return ['-animation_file', animationFile.emit()];
      },
    ),
    .new(
      draft: true, // TODO: draft
      type: .popcapMapDesigner,
      identifier: ModuleType.popcapMapDesigner.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'PopCap Map Designer',
      icon: IconSet.add,
      buildMainPage: (key, setting, configuration, option) => popcap_map_designer.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => popcap_map_designer.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.popcapMapDesigner,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.popcapMapDesigner,
      parseConfiguration: (json) => popcap_map_designer.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return null;
      },
    ),
    .new(
      draft: true, // TODO: draft
      type: .popcapPackagePreviewer,
      identifier: ModuleType.popcapPackagePreviewer.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'PopCap Package Previewer',
      icon: IconSet.add,
      buildMainPage: (key, setting, configuration, option) => popcap_package_previewer.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => popcap_package_previewer.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.popcapPackagePreviewer,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.popcapPackagePreviewer,
      parseConfiguration: (json) => popcap_package_previewer.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return null;
      },
    ),
    .new(
      draft: true, // TODO: draft
      type: .popcapPackageBuilder,
      identifier: ModuleType.popcapPackageBuilder.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'PopCap Package Builder',
      icon: IconSet.add,
      buildMainPage: (key, setting, configuration, option) => popcap_package_builder.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => popcap_package_builder.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.popcapPackageBuilder,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.popcapPackageBuilder,
      parseConfiguration: (json) => popcap_package_builder.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return null;
      },
    ),
    .new(
      draft: false,
      type: .kairosoftGameManager,
      identifier: ModuleType.kairosoftGameManager.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: 'Kairosoft Game Manager',
      icon: IconSet.history_edu,
      buildMainPage: (key, setting, configuration, option) => kairosoft_game_manager.MainPage(
        key: key,
        setting: setting.as(),
        configuration: configuration.as(),
        option: option,
      ),
      buildSettingPanel: (context) => kairosoft_game_manager.SettingPanel(
        data: Provider.of<SettingProvider>(context, listen: false).data.kairosoftGameManager,
        onUpdate: () => Provider.of<SettingProvider>(context, listen: false).save(),
      ),
      querySetting: (context) => Provider.of<SettingProvider>(context, listen: false).data.kairosoftGameManager,
      parseConfiguration: (json) => kairosoft_game_manager.ConfigurationHelper.parseDataFromJson(json),
      generateForwardOption: (resource) async {
        return null;
      },
    ),
  ];

  static ModuleInformation query(
    ModuleType type,
  ) {
    return ModuleHelper._information[type.index];
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
      ModuleHelper.query(launcher.type).identifier,
      ...launcher.option,
    ];
  }

}
