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

enum ArgumentType {
  boolean,
  integer,
  floater,
  string,
  size,
  path,
}

class ArgumentConfiguration {
  String        identifier;
  String        name;
  ArgumentType  type;
  List<Object>? option;
  ArgumentConfiguration({
    required this.identifier,
    required this.name,
    required this.type,
    required this.option,
  });
}

class MethodConfiguration {
  String                      identifier;
  String                      name;
  String                      icon;
  List<ArgumentConfiguration> argument;
  List<String>?               batch;
  List<PresetConfiguration?>  preset;
  MethodConfiguration({
    required this.identifier,
    required this.name,
    required this.icon,
    required this.argument,
    required this.batch,
    required this.preset,
  });
}

class MethodGroupConfiguration {
  String                    identifier;
  String                    name;
  String                    icon;
  List<MethodConfiguration> item;
  MethodGroupConfiguration({
    required this.identifier,
    required this.name,
    required this.icon,
    required this.item,
  });
}

class Configuration {
  List<MethodGroupConfiguration> method;
  Configuration({
    required this.method,
  });
}
