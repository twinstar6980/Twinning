import '/common.dart';
import '/utility/convert_helper.dart';

// ----------------

class ValueExpression {
  ValueExpression(
  );
  @override
  get hashCode => super.hashCode; // ignore: unnecessary_overrides
  @override
  operator ==(other) => other is ValueExpression;
}

// ----------------

class PauseExpression extends ValueExpression {
  PauseExpression(
  );
  @override
  get hashCode => super.hashCode; // ignore: unnecessary_overrides
  @override
  operator ==(other) => other is PauseExpression;
}

class BooleanExpression extends ValueExpression {
  Boolean value;
  BooleanExpression(
    this.value,
  );
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
  @override
  operator ==(other) => other is BooleanExpression && this.value == other.value;
}

class IntegerExpression extends ValueExpression {
  Integer value;
  IntegerExpression(
    this.value,
  );
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
  @override
  operator ==(other) => other is IntegerExpression && this.value == other.value;
}

class FloaterExpression extends ValueExpression {
  Floater value;
  FloaterExpression(
    this.value,
  );
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
  @override
  operator ==(other) => other is FloaterExpression && this.value == other.value;
}

class SizeExpression extends ValueExpression {
  Floater count;
  Integer exponent;
  SizeExpression(
    this.count,
    this.exponent,
  );
  @override
  get hashCode => super.hashCode ^ this.count.hashCode ^ this.exponent.hashCode;
  @override
  operator ==(other) => other is SizeExpression && this.count == other.count && this.exponent == other.exponent;
}

class StringExpression extends ValueExpression {
  String value;
  StringExpression(
    this.value,
  );
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
  @override
  operator ==(other) => other is StringExpression && this.value == other.value;
}

class PathExpression extends ValueExpression {
  String content;
  PathExpression(
    this.content,
  );
  @override
  get hashCode => super.hashCode ^ this.content.hashCode;
  @override
  operator ==(other) => other is PathExpression && this.content == other.content;
}

class EnumerationExpression extends ValueExpression {
  String item;
  EnumerationExpression(
    this.item,
  );
  @override
  get hashCode => super.hashCode ^ this.item.hashCode;
  @override
  operator ==(other) => other is EnumerationExpression && this.item == other.item;
}

// ----------------

class ValueExpressionHelper {

  // #region utility

  static String makeString(
    ValueExpression value,
  ) {
    return switch (value) {
      PauseExpression       _ => '',
      BooleanExpression     _ => '${ConvertHelper.makeBooleanToStringOfConfirmationCharacter(value.value)}',
      IntegerExpression     _ => '${ConvertHelper.makeIntegerToString(value.value, true)}',
      FloaterExpression     _ => '${ConvertHelper.makeFloaterToString(value.value, true)}',
      SizeExpression        _ => '${ConvertHelper.makeFloaterToString(value.count, false)}${['b', 'k', 'm', 'g'][value.exponent]}',
      StringExpression      _ => '${value.value}',
      PathExpression        _ => '${value.content}',
      EnumerationExpression _ => '${value.item}',
      _                       => throw UnreachableException(),
    };
  }

  // #endregion

}
