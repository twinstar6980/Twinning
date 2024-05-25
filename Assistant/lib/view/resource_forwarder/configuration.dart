import '/common.dart';

// ----------------

enum OptionFilterFileObjectType {
  any,
  file,
  directory,
}

class OptionFilterConfiguration {
  OptionFilterFileObjectType type;
  String                     name;
  OptionFilterConfiguration({
    required this.type,
    required this.name,
  });
}

class OptionPresetConfiguration {
  String              name;
  Map<String, Object> argument;
  OptionPresetConfiguration({
    required this.name,
    required this.argument,
  });
}

class OptionConfiguration {
  String                           name;
  String                           icon;
  OptionFilterConfiguration        filter;
  Boolean                          batchable;
  String?                          method;
  List<OptionPresetConfiguration?> preset;
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
    var data = (json as List).map((jsonGroup) => OptionGroupConfiguration(
      name: (jsonGroup['name'] as String),
      icon: (jsonGroup['icon'] as String),
      item: (jsonGroup['item'] as List).map((jsonItem) => OptionConfiguration(
        name: (jsonItem['name'] as String),
        icon: (jsonItem['icon'] as String),
        filter: OptionFilterConfiguration(
          name: (jsonItem['filter']['name'] as String),
          type: OptionFilterFileObjectType.values.byName(jsonItem['filter']['type'] as String),
        ),
        batchable: (jsonItem['batchable'] as Boolean),
        method: (jsonItem['method'] as String?),
        preset: (jsonItem['preset'] as List).map((jsonPreset) => jsonPreset == null ? null : OptionPresetConfiguration(
          name: (jsonPreset['name'] as String),
          argument: (jsonPreset['argument'] as Map).cast<String, Object>(),
        )).toList(),
      )).toList(),
    )).toList();
    return data;
  }

  // #endregion

}
