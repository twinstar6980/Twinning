import '/common.dart';
import '/utility/json_type.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_size.dart';
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

class SingleValueExpression<TValue> extends ValueExpression {
  TValue value;
  SingleValueExpression(
    this.value,
  );
  @override
  operator ==(other) => other is SingleValueExpression<TValue> && this.value == other.value;
  @override
  get hashCode => Object.hashAll([this.value]);
}

typedef BooleanExpression = SingleValueExpression<Boolean>;

typedef IntegerExpression = SingleValueExpression<Integer>;

typedef FloaterExpression = SingleValueExpression<Floater>;

typedef StringExpression = SingleValueExpression<String>;

typedef SizeExpression = SingleValueExpression<StorageSize>;

typedef PathExpression = SingleValueExpression<StoragePath>;

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
      SizeExpression    _ => '${value.value.emit()}',
      PathExpression    _ => '${value.value.emit()}',
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
      SizeExpression    _ => '${value.value.emit()}',
      PathExpression    _ => '${value.value.emit()}',
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
        json.jsonString().selfLet((it) => StorageSize.of(it)),
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
