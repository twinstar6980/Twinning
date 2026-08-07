import '/common.dart';
import '/module.dart';
import '/utility/json_type.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/convert_helper.dart';
import '/widget/export.dart';
import '/view/core_resource_shipper/main_page.dart';
import '/view/core_resource_shipper/setting_panel.dart';
import '/view/core_resource_shipper/setting.dart';
import '/view/core_resource_shipper/configuration.dart';
import '/view/core_resource_shipper/option.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: false,
  type: .coreResourceShipper,
  name: 'Resource Shipper',
  icon: IconSet.share_windows,
  buildMainPage: (key, setting, configuration, option) => .new(key: key, setting: setting, configuration: configuration, option: option),
  buildSettingPanel: (data, onUpdate) => .new(data: data, onUpdate: onUpdate),
  makeSetting: (data) => {
    'parallel_forward': data.parallelForward,
    'enable_filter': data.enableFilter,
    'enable_batch': data.enableBatch,
  },
  parseSetting: (json) => Setting(
    parallelForward: json.jsonIn('parallel_forward').jsonBoolean(),
    enableFilter: json.jsonIn('enable_filter').jsonBoolean(),
    enableBatch: json.jsonIn('enable_batch').jsonBoolean(),
  ),
  generateDefaultSetting: () => Setting(
    parallelForward: false,
    enableFilter: true,
    enableBatch: false,
  ),
  parseConfiguration: (json) => Configuration(
    option: json.jsonIn('option').jsonArray().map((json) => OptionGroupConfiguration(
      name: json.jsonIn('name').jsonString(),
      icon: json.jsonIn('icon').jsonString(),
      item: json.jsonIn('item').jsonArray().map((json) => OptionConfiguration(
        name: json.jsonIn('name').jsonString(),
        icon: json.jsonIn('icon').jsonString(),
        filter: json.jsonIn('filter')?.jsonObject().selfLet((json) => FilterConfiguration(
          name: json.jsonIn('name').jsonString(),
          type: json.jsonIn('type').jsonString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, FilterType.values)),
        )),
        batch: json.jsonIn('batch').jsonBoolean(),
        method: json.jsonIn('method')?.jsonString(),
        preset: json.jsonIn('preset').jsonArray().map((json) => json == null ? null : PresetConfiguration(
          name: json.jsonIn('name').jsonString(),
          argument: json.jsonIn('argument').jsonObject().cast<String, Object>(),
        )).toList(),
      )).toList(),
    )).toList(),
  ),
  makeOption: (data) => {
    'parallel_forward': data.parallelForward,
    'enable_filter': data.enableFilter,
    'enable_batch': data.enableBatch,
    'resource': data.resource?.map((data) => {
      'path': data.path.emit(),
    }).toList(),
  },
  parseOption: (json) => Option(
    parallelForward: json.jsonInOr('parallel_forward')?.jsonBoolean(),
    enableFilter: json.jsonInOr('enable_filter')?.jsonBoolean(),
    enableBatch: json.jsonInOr('enable_batch')?.jsonBoolean(),
    resource: json.jsonInOr('resource')?.jsonArray().map((json) => (
      path: json.jsonIn('path').jsonString().selfLet((it) => StoragePath.of(it)),
    )).toList(),
  ),
  generateDefaultOption: () => Option(
    parallelForward: null,
    enableFilter: null,
    enableBatch: null,
    resource: null,
  ),
  generateForwardOption: (resource) async {
    for (var resourceItem in resource) {
      if (!await StorageHelper.exist(resourceItem)) {
        return null;
      }
    }
    return .new(
      resource: resource.map((it) => (path: it)).toList(),
    );
  },
);
