import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/json_type.dart';
import '/utility/json_helper.dart';
import '/widget/export.dart';
import '/view/core_task_worker/descriptor.dart' as core_task_worker;
import '/view/core_command_sender/descriptor.dart' as core_command_sender;
import '/view/core_resource_shipper/descriptor.dart' as core_resource_shipper;
import '/view/popcap_reflection_descriptor/descriptor.dart' as popcap_reflection_descriptor;
import '/view/popcap_animation_viewer/descriptor.dart' as popcap_animation_viewer;
import '/view/popcap_map_designer/descriptor.dart' as popcap_map_designer;
import '/view/popcap_package_previewer/descriptor.dart' as popcap_package_previewer;
import '/view/popcap_package_builder/descriptor.dart' as popcap_package_builder;
import '/view/kairosoft_game_manager/descriptor.dart' as kairosoft_game_manager;
import 'package:collection/collection.dart' as lib;

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

class ModuleDescriptor {
  Boolean                                                                             draft;
  ModuleType                                                                          type;
  String                                                                              identifier;
  String                                                                              name;
  IconData                                                                            icon;
  Widget Function(GlobalKey key, Object setting, Object configuration, Object option) buildMainPage;
  Widget Function(BuildContext context)                                               buildSettingPanel;
  Object Function(BuildContext context)                                               querySetting;
  Object Function(JsonObject json)                                                    parseSetting;
  JsonObject Function(Object data)                                                    makeSetting;
  Object Function()                                                                   generateDefaultSetting;
  Object Function(JsonObject json)                                                    parseConfiguration;
  JsonObject Function(Object data)                                                    makeOption;
  Object Function(JsonObject json)                                                    parseOption;
  Object Function()                                                                   generateDefaultOption;
  Future<Object?> Function(List<StoragePath> resource)                                generateForwardOption;
  ModuleDescriptor({
    required this.draft,
    required this.type,
    required this.identifier,
    required this.name,
    required this.icon,
    required this.buildMainPage,
    required this.buildSettingPanel,
    required this.querySetting,
    required this.parseSetting,
    required this.makeSetting,
    required this.generateDefaultSetting,
    required this.parseConfiguration,
    required this.makeOption,
    required this.parseOption,
    required this.generateDefaultOption,
    required this.generateForwardOption,
  });
}

// ----------------

class ModuleLauncherConfiguration {
  String     title;
  ModuleType type;
  Object     option;
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

// ----------------

class ModuleHelper {

  static final List<ModuleDescriptor> _descriptor = [
    core_task_worker.kModuleDescriptor,
    core_command_sender.kModuleDescriptor,
    core_resource_shipper.kModuleDescriptor,
    popcap_reflection_descriptor.kModuleDescriptor,
    popcap_animation_viewer.kModuleDescriptor,
    popcap_map_designer.kModuleDescriptor,
    popcap_package_previewer.kModuleDescriptor,
    popcap_package_builder.kModuleDescriptor,
    kairosoft_game_manager.kModuleDescriptor,
  ];

  static ModuleDescriptor query(
    ModuleType type,
  ) {
    return ModuleHelper._descriptor[type.index];
  }

  // ----------------

  static ModuleDescriptor createDescriptor<TMainPage extends Widget, TSettingPanel extends Widget, TSetting extends Object, TConfiguration extends Object, TOption extends Object>({
    required Boolean                                                                                           draft,
    required ModuleType                                                                                        type,
    required String                                                                                            name,
    required IconData                                                                                          icon,
    required TMainPage Function(GlobalKey key, TSetting setting, TConfiguration configuration, TOption option) buildMainPage,
    required TSettingPanel Function(TSetting data, Void Function() onUpdate)                                   buildSettingPanel,
    required JsonObject Function(TSetting data)                                                                makeSetting,
    required TSetting Function(JsonObject json)                                                                parseSetting,
    required TSetting Function()                                                                               generateDefaultSetting,
    required TConfiguration Function(JsonObject json)                                                          parseConfiguration,
    required JsonObject Function(TOption data)                                                                 makeOption,
    required TOption Function(JsonObject json)                                                                 parseOption,
    required TOption Function()                                                                                generateDefaultOption,
    required Future<TOption?> Function(List<StoragePath> resource)                                             generateForwardOption,
  }) {
    return ModuleDescriptor(
      draft: draft,
      type: type,
      identifier: type.selfLet(ConvertHelper.makeEnumerationToStringOfSnakeCase),
      name: name,
      icon: icon,
      buildMainPage: (key, setting, configuration, option) => buildMainPage(
        key,
        setting.as(),
        configuration.as(),
        option.as(),
      ),
      buildSettingPanel: (context) => buildSettingPanel(
        SettingProvider.of(context, listen: false).data.moduleSetting[type]!.as(),
        () => SettingProvider.of(context, listen: false).save(),
      ),
      querySetting: (context) => SettingProvider.of(context, listen: false).data.moduleSetting[type]!,
      makeSetting: (data) => makeSetting(data.as()),
      parseSetting: (json) => parseSetting(json),
      generateDefaultSetting: () => generateDefaultSetting(),
      parseConfiguration: (json) => parseConfiguration(json),
      makeOption: (data) => makeOption(data.as()),
      parseOption: (json) => parseOption(json),
      generateDefaultOption: () => generateDefaultOption(),
      generateForwardOption: (resource) async => await generateForwardOption(resource),
    );
  }

  // ----------------

  static Boolean compareLauncher(
    ModuleLauncherConfiguration thix,
    ModuleLauncherConfiguration that,
  ) {
    return thix.type == that.type && lib.MapEquality().equals(ModuleHelper.query(thix.type).makeOption(thix.option), ModuleHelper.query(that.type).makeOption(that.option));
  }

  static List<String> generateArgument(
    ModuleLauncherConfiguration launcher,
  ) {
    return [
      '-launch',
      launcher.title,
      ModuleHelper.query(launcher.type).identifier,
      JsonHelper.encodeText(launcher.option, indented: true),
    ];
  }

}
