import '/common.dart';
import '/utility/json_type.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/view/core_command_sender/configuration.dart';

// ----------------

class ValueExpression {
  ValueExpression(
  );
  @override
  operator ==(other) => other is ValueExpression;
  @override
  get hashCode => Object.hashAll([]);
}

// ----------------

class BooleanExpression extends ValueExpression {
  Boolean value;
  BooleanExpression(
    this.value,
  );
  @override
  operator ==(other) => other is BooleanExpression && this.value == other.value;
  @override
  get hashCode => Object.hashAll([this.value]);
}

class IntegerExpression extends ValueExpression {
  Integer value;
  IntegerExpression(
    this.value,
  );
  @override
  operator ==(other) => other is IntegerExpression && this.value == other.value;
  @override
  get hashCode => Object.hashAll([this.value]);
}

class FloaterExpression extends ValueExpression {
  Floater value;
  FloaterExpression(
    this.value,
  );
  @override
  operator ==(other) => other is FloaterExpression && this.value == other.value;
  @override
  get hashCode => Object.hashAll([this.value]);
}

class StringExpression extends ValueExpression {
  String value;
  StringExpression(
    this.value,
  );
  @override
  operator ==(other) => other is StringExpression && this.value == other.value;
  @override
  get hashCode => Object.hashAll([this.value]);
}

class SizeExpression extends ValueExpression {
  Floater count;
  Integer exponent;
  SizeExpression(
    this.count,
    this.exponent,
  );
  @override
  operator ==(other) => other is SizeExpression && this.count == other.count && this.exponent == other.exponent;
  @override
  get hashCode => Object.hashAll([this.count, this.exponent]);
}

class PathExpression extends ValueExpression {
  StoragePath content;
  PathExpression(
    this.content,
  );
  @override
  operator ==(other) => other is PathExpression && this.content == other.content;
  @override
  get hashCode => Object.hashAll([this.content]);
}

// ----------------

class ValueExpressionHelper {

  // #region utility

  static String makeString(
    ValueExpression value,
  ) {
    return switch (value) {
      BooleanExpression _ => '${ConvertHelper.makeBooleanToStringOfConfirmationCharacter(value.value)}',
      IntegerExpression _ => '${ConvertHelper.makeIntegerToString(value.value)}',
      FloaterExpression _ => '${ConvertHelper.makeFloaterToString(value.value)}',
      StringExpression  _ => '${value.value}',
      SizeExpression    _ => '${ConvertHelper.makeFloaterToString(value.count)}${['b', 'k', 'm', 'g'][value.exponent]}',
      PathExpression    _ => '${value.content.emit()}',
      _                   => throw UnreachableException(),
    };
  }

  // ----------------

  static JsonNode makeArgumentValueJson(
    ValueExpression value,
  ) {
    return switch (value) {
      BooleanExpression _ => value.value,
      IntegerExpression _ => value.value,
      FloaterExpression _ => value.value,
      StringExpression  _ => value.value,
      SizeExpression    _ => '${ConvertHelper.makeFloaterToString(value.count)}${['b', 'k', 'm', 'g'][value.exponent]}',
      PathExpression    _ => '${value.content.emit()}',
      _                   => throw UnreachableException(),
    };
  }

  static ValueExpression parseArgumentValueJson(
    ArgumentType type,
    JsonNode     json,
  ) {
    return switch (type) {
      .boolean => BooleanExpression(
        json.jsonBoolean(),
      ),
      .integer => IntegerExpression(
        json.jsonInteger(),
      ),
      .floater => FloaterExpression(
        json.jsonFloater(),
      ),
      .string => StringExpression(
        json.jsonString(),
      ),
      .size => SizeExpression(
        json.jsonString().selfLet((it) => Floater.parse(it.substring(0, it.length - 1))),
        json.jsonString().selfLet((it) => ['b', 'k', 'm', 'g'].indexOf(it[it.length - 1])).selfAlso((it) => assertTest(it != -1)),
      ),
      .path => PathExpression(
        json.jsonString().selfLet((it) => StoragePath.of(it)),
      ),
    };
  }

  // ----------------

  static JsonObject makeArgumentValueListJson(
    List<ArgumentConfiguration>     configuration,
    List<Wrapper<ValueExpression?>> value,
  ) {
    assertTest(configuration.length == value.length);
    var json = JsonObject();
    for (var index = 0; index < configuration.length; index++) {
      var itemConfiguration = configuration[index];
      var itemValue = value[index];
      if (itemValue.value != null) {
        json[itemConfiguration.identifier] = ValueExpressionHelper.makeArgumentValueJson(itemValue.value!);
      }
    }
    return json;
  }

  static List<Wrapper<ValueExpression?>> parseArgumentValueListJson(
    List<ArgumentConfiguration> configuration,
    JsonObject                  json,
  ) {
    var value = <Wrapper<ValueExpression?>>[];
    for (var index = 0; index < configuration.length; index++) {
      var itemConfiguration = configuration[index];
      var itemJson = json[itemConfiguration.identifier];
      value.add(.of(itemJson == null ? null : ValueExpressionHelper.parseArgumentValueJson(itemConfiguration.type, itemJson)));
    }
    return value;
  }

  // #endregion

}
