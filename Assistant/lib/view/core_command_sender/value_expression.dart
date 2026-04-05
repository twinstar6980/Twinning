import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';

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
      PathExpression    _ => '${value.content.emitGeneric()}',
      _                   => throw UnreachableException(),
    };
  }

  // #endregion

}
