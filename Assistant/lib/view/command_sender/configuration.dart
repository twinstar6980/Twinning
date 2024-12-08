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
  String        id;
  String        name;
  ArgumentType  type;
  List<Object>? option;
  ArgumentConfiguration({
    required this.id,
    required this.name,
    required this.type,
    required this.option,
  });
}

class MethodConfiguration {
  String                      id;
  String                      name;
  String                      icon;
  List<ArgumentConfiguration> argument;
  List<String>?               batchable;
  List<PresetConfiguration?>  preset;
  MethodConfiguration({
    required this.id,
    required this.name,
    required this.icon,
    required this.argument,
    required this.batchable,
    required this.preset,
  });
}

class MethodGroupConfiguration {
  String                    id;
  String                    name;
  String                    icon;
  List<MethodConfiguration> item;
  MethodGroupConfiguration({
    required this.id,
    required this.name,
    required this.icon,
    required this.item,
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
      _                   => throw Exception(),
    };
  }

  static ValueExpression parseArgumentValueJson(
    ArgumentType type,
    Object       json,
  ) {
    return switch (type) {
      ArgumentType.boolean => BooleanExpression(
        json.as<Boolean>(),
      ),
      ArgumentType.integer => IntegerExpression(
        json.as<Integer>(),
      ),
      ArgumentType.floater => FloaterExpression(
        json.as<Floater>(),
      ),
      ArgumentType.size    => SizeExpression(
        json.as<String>().selfLet((it) => Floater.parse(it.substring(0, it.length - 1))),
        json.as<String>().selfLet((it) => ['b', 'k', 'm', 'g'].indexOf(it[it.length - 1])).selfAlso((it) { assertTest(it != -1); }),
      ),
      ArgumentType.string  => StringExpression(
        json.as<String>(),
      ),
      ArgumentType.path    => PathExpression(
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
        json[itemConfiguration.id] = makeArgumentValueJson(itemValue.value!);
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
      var itemJson = json[itemConfiguration.id];
      value.add(Wrapper(itemJson == null ? null : ConfigurationHelper.parseArgumentValueJson(itemConfiguration.type, itemJson)));
    }
    return value;
  }

  // #endregion

  // #region convert

  static List<MethodGroupConfiguration> parseDataFromJson(
    dynamic json,
  ) {
    return (json as List<dynamic>).map((jsonGroup) => MethodGroupConfiguration(
      id: (jsonGroup['id'] as String),
      name: (jsonGroup['name'] as String),
      icon: (jsonGroup['icon'] as String),
      item: (jsonGroup['item'] as List<dynamic>).map((jsonItem) => MethodConfiguration(
        id: (jsonItem['id'] as String),
        name: (jsonItem['name'] as String),
        icon: (jsonItem['icon'] as String),
        argument: (jsonItem['argument'] as List<dynamic>).map((jsonArgument) => ArgumentConfiguration(
          id: (jsonArgument['id'] as String),
          name: (jsonArgument['name'] as String),
          type: (jsonArgument['type'] as String).selfLet((it) => ArgumentType.values.byName(it)),
          option: (jsonArgument['option'] as List<dynamic>?)?.cast<Object>(),
        )).toList(),
        batchable: (jsonItem['batchable'] as List<dynamic>?)?.cast<String>(),
        preset: (jsonItem['preset'] as List<dynamic>).map((jsonPreset) => jsonPreset == null ? null : PresetConfiguration(
          name: (jsonPreset['name'] as String),
          argument: (jsonPreset['argument'] as Map<dynamic, dynamic>).cast<String, Object>(),
        )).toList(),
      )).toList(),
    )).toList();
  }

  // #endregion

}
