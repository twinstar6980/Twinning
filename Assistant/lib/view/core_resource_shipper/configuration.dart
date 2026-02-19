import '/common.dart';
import '/utility/convert_helper.dart';

// ----------------

class PresetConfiguration {
  String              name;
  Map<String, Object> argument;
  PresetConfiguration({
    required this.name,
    required this.argument,
  });
}

enum FilterType {
  any,
  file,
  directory,
}

class FilterConfiguration {
  FilterType type;
  String     name;
  FilterConfiguration({
    required this.type,
    required this.name,
  });
}

class OptionConfiguration {
  String                     name;
  String                     icon;
  FilterConfiguration?       filter;
  Boolean                    batch;
  String?                    method;
  List<PresetConfiguration?> preset;
  OptionConfiguration({
    required this.name,
    required this.icon,
    required this.filter,
    required this.batch,
    required this.method,
    required this.preset,
  });
}

class OptionGroupConfiguration {
  String                    name;
  String                    icon;
  List<OptionConfiguration> item;
  OptionGroupConfiguration({
    required this.name,
    required this.icon,
    required this.item,
  });
}

class Configuration {
  List<OptionGroupConfiguration> option;
  Configuration({
    required this.option,
  });
}

// ----------------

class ConfigurationHelper {

  // #region convert

  static Configuration parseDataFromJson(
    dynamic json,
  ) {
    return Configuration(
      option: (json['option'] as List<dynamic>).map((jsonGroup) => OptionGroupConfiguration(
        name: (jsonGroup['name'] as String),
        icon: (jsonGroup['icon'] as String),
        item: (jsonGroup['item'] as List<dynamic>).map((jsonItem) => OptionConfiguration(
          name: (jsonItem['name'] as String),
          icon: (jsonItem['icon'] as String),
          filter: (jsonItem['filter'] as Map<dynamic, dynamic>?)?.selfLet((jsonFilter) => FilterConfiguration(
            name: (jsonFilter['name'] as String),
            type: (jsonFilter['type'] as String).selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, FilterType.values)),
          )),
          batch: (jsonItem['batch'] as Boolean),
          method: (jsonItem['method'] as String?),
          preset: (jsonItem['preset'] as List<dynamic>).map((jsonPreset) => jsonPreset == null ? null : PresetConfiguration(
            name: (jsonPreset['name'] as String),
            argument: (jsonPreset['argument'] as Map<dynamic, dynamic>).cast<String, Object>(),
          )).toList(),
        )).toList(),
      )).toList(),
    );
  }

  // #endregion

}
