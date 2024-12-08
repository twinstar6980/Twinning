import '/common.dart';

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
  FilterConfiguration        filter;
  Boolean                    batchable;
  String?                    method;
  List<PresetConfiguration?> preset;
  OptionConfiguration({
    required this.name,
    required this.icon,
    required this.filter,
    required this.batchable,
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

// ----------------

class ConfigurationHelper {

  // #region convert

  static List<OptionGroupConfiguration> parseDataFromJson(
    dynamic json,
  ) {
    return (json as List<dynamic>).map((jsonGroup) => OptionGroupConfiguration(
      name: (jsonGroup['name'] as String),
      icon: (jsonGroup['icon'] as String),
      item: (jsonGroup['item'] as List<dynamic>).map((jsonItem) => OptionConfiguration(
        name: (jsonItem['name'] as String),
        icon: (jsonItem['icon'] as String),
        filter: (jsonItem['filter'] as Map<dynamic, dynamic>).selfLet((jsonFilter) => FilterConfiguration(
          name: (jsonFilter['name'] as String),
          type: (jsonFilter['type'] as String).selfLet((it) => FilterType.values.byName(it)),
        )),
        batchable: (jsonItem['batchable'] as Boolean),
        method: (jsonItem['method'] as String?),
        preset: (jsonItem['preset'] as List<dynamic>).map((jsonPreset) => jsonPreset == null ? null : PresetConfiguration(
          name: (jsonPreset['name'] as String),
          argument: (jsonPreset['argument'] as Map<dynamic, dynamic>).cast<String, Object>(),
        )).toList(),
      )).toList(),
    )).toList();
  }

  // #endregion

}
