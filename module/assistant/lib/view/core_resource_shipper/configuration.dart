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
