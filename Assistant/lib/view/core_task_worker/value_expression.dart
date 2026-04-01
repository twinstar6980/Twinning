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
  get hashCode => super.hashCode; // ignore: unnecessary_overrides
}

// ----------------

class PauseExpression extends ValueExpression {
  PauseExpression(
  );
  @override
  operator ==(other) => other is PauseExpression;
  @override
  get hashCode => super.hashCode; // ignore: unnecessary_overrides
}

class BooleanExpression extends ValueExpression {
  Boolean value;
  BooleanExpression(
    this.value,
  );
  @override
  operator ==(other) => other is BooleanExpression && this.value == other.value;
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
}

class IntegerExpression extends ValueExpression {
  Integer value;
  IntegerExpression(
    this.value,
  );
  @override
  operator ==(other) => other is IntegerExpression && this.value == other.value;
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
}

class FloaterExpression extends ValueExpression {
  Floater value;
  FloaterExpression(
    this.value,
  );
  @override
  operator ==(other) => other is FloaterExpression && this.value == other.value;
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
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
  get hashCode => super.hashCode ^ this.count.hashCode ^ this.exponent.hashCode;
}

class StringExpression extends ValueExpression {
  String value;
  StringExpression(
    this.value,
  );
  @override
  operator ==(other) => other is StringExpression && this.value == other.value;
  @override
  get hashCode => super.hashCode ^ this.value.hashCode;
}

class PathExpression extends ValueExpression {
  StoragePath content;
  PathExpression(
    this.content,
  );
  @override
  operator ==(other) => other is PathExpression && this.content == other.content;
  @override
  get hashCode => super.hashCode ^ this.content.hashCode;
}

class EnumerationExpression extends ValueExpression {
  String item;
  EnumerationExpression(
    this.item,
  );
  @override
  operator ==(other) => other is EnumerationExpression && this.item == other.item;
  @override
  get hashCode => super.hashCode ^ this.item.hashCode;
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
      PathExpression        _ => '${value.content.emitGeneric()}',
      EnumerationExpression _ => '${value.item}',
      _                       => throw UnreachableException(),
    };
  }

  // #endregion

}
