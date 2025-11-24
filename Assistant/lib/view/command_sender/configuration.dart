import '/common.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/view/command_sender/value_expression.dart';

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
  size,
  string,
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

// ----------------

class ConfigurationHelper {

  // #region utility

  static Object makeArgumentValueJson(
    ValueExpression value,
  ) {
    return switch (value) {
      BooleanExpression _ => value.value,
      IntegerExpression _ => value.value,
      FloaterExpression _ => value.value,
      SizeExpression    _ => '${ConvertHelper.makeFloaterToString(value.count, false)}${['b', 'k', 'm', 'g'][value.exponent]}',
      StringExpression  _ => value.value,
      PathExpression    _ => '${value.content}',
      _                   => throw UnreachableException(),
    };
  }

  static ValueExpression parseArgumentValueJson(
    ArgumentType type,
    Object       json,
  ) {
    return switch (type) {
      .boolean => BooleanExpression(
        json.as<Boolean>(),
      ),
      .integer => IntegerExpression(
        json.as<Integer>(),
      ),
      .floater => FloaterExpression(
        json.as<Floater>(),
      ),
      .size => SizeExpression(
        json.as<String>().selfLet((it) => Floater.parse(it.substring(0, it.length - 1))),
        json.as<String>().selfLet((it) => ['b', 'k', 'm', 'g'].indexOf(it[it.length - 1])).selfAlso((it) => assertTest(it != -1)),
      ),
      .string => StringExpression(
        json.as<String>(),
      ),
      .path => PathExpression(
        json.as<String>(),
      ),
    };
  }

  // ----------------

  static Map<String, Object> makeArgumentValueListJson(
    List<ArgumentConfiguration>     configuration,
    List<Wrapper<ValueExpression?>> value,
  ) {
    assertTest(configuration.length == value.length);
    var json = <String, Object>{};
    for (var index = 0; index < configuration.length; index++) {
      var itemConfiguration = configuration[index];
      var itemValue = value[index];
      if (itemValue.value != null) {
        json[itemConfiguration.identifier] = makeArgumentValueJson(itemValue.value!);
      }
    }
    return json;
  }

  static List<Wrapper<ValueExpression?>> parseArgumentValueListJson(
    List<ArgumentConfiguration> configuration,
    Map<String, Object>         json,
  ) {
    var value = <Wrapper<ValueExpression?>>[];
    for (var index = 0; index < configuration.length; index++) {
      var itemConfiguration = configuration[index];
      var itemJson = json[itemConfiguration.identifier];
      value.add(.new(itemJson == null ? null : ConfigurationHelper.parseArgumentValueJson(itemConfiguration.type, itemJson)));
    }
    return value;
  }

  // #endregion

  // #region convert

  static Configuration parseDataFromJson(
    dynamic json,
  ) {
    return Configuration(
      method: (json['method'] as List<dynamic>).map((jsonGroup) => MethodGroupConfiguration(
        identifier: (jsonGroup['identifier'] as String),
        name: (jsonGroup['name'] as String),
        icon: (jsonGroup['icon'] as String),
        item: (jsonGroup['item'] as List<dynamic>).map((jsonItem) => MethodConfiguration(
          identifier: (jsonItem['identifier'] as String),
          name: (jsonItem['name'] as String),
          icon: (jsonItem['icon'] as String),
          argument: (jsonItem['argument'] as List<dynamic>).map((jsonArgument) => ArgumentConfiguration(
            identifier: (jsonArgument['identifier'] as String),
            name: (jsonArgument['name'] as String),
            type: (jsonArgument['type'] as String).selfLet((it) => .values.byName(it)),
            option: (jsonArgument['option'] as List<dynamic>?)?.cast<Object>(),
          )).toList(),
          batch: (jsonItem['batch'] as List<dynamic>?)?.cast<String>(),
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
