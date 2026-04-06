import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';

// ----------------

class ValueExpression {
  String? macro;
  ValueExpression(
    this.macro,
  );
  @override
  operator ==(other) => other is ValueExpression && this.macro == other.macro;
  @override
  get hashCode => Object.hashAll([this.macro]);
}

// ----------------

class PauseExpression extends ValueExpression {
  PauseExpression(
    super.command,
  );
  @override
  operator ==(other) => other is PauseExpression;
  @override
  get hashCode => Object.hashAll([super.macro]);
}

class BooleanExpression extends ValueExpression {
  Boolean value;
  BooleanExpression(
    super.command,
    this.value,
  );
  @override
  operator ==(other) => other is BooleanExpression && super.macro == other.macro && this.value == other.value;
  @override
  get hashCode => Object.hashAll([super.macro, this.value]);
}

class IntegerExpression extends ValueExpression {
  Integer value;
  IntegerExpression(
    super.command,
    this.value,
  );
  @override
  operator ==(other) => other is IntegerExpression && super.macro == other.macro && this.value == other.value;
  @override
  get hashCode => Object.hashAll([super.macro, this.value]);
}

class FloaterExpression extends ValueExpression {
  Floater value;
  FloaterExpression(
    super.command,
    this.value,
  );
  @override
  operator ==(other) => other is FloaterExpression && super.macro == other.macro && this.value == other.value;
  @override
  get hashCode => Object.hashAll([super.macro, this.value]);
}

class StringExpression extends ValueExpression {
  String value;
  StringExpression(
    super.command,
    this.value,
  );
  @override
  operator ==(other) => other is StringExpression && super.macro == other.macro && this.value == other.value;
  @override
  get hashCode => Object.hashAll([super.macro, this.value]);
}

class SizeExpression extends ValueExpression {
  Floater count;
  Integer exponent;
  SizeExpression(
    super.command,
    this.count,
    this.exponent,
  );
  @override
  operator ==(other) => other is SizeExpression && super.macro == other.macro && this.count == other.count && this.exponent == other.exponent;
  @override
  get hashCode => Object.hashAll([super.macro, this.count, this.exponent]);
}

class PathExpression extends ValueExpression {
  StoragePath content;
  PathExpression(
    super.command,
    this.content,
  );
  @override
  operator ==(other) => other is PathExpression && super.macro == other.macro && this.content == other.content;
  @override
  get hashCode => Object.hashAll([super.macro, this.content]);
}

class EnumerationExpression extends ValueExpression {
  String item;
  EnumerationExpression(
    super.command,
    this.item,
  );
  @override
  operator ==(other) => other is EnumerationExpression && super.macro == other.macro && this.item == other.item;
  @override
  get hashCode => Object.hashAll([super.macro, this.item]);
}

// ----------------

class ValueExpressionHelper {

  // #region utility

  static String makeString(
    ValueExpression value,
  ) {
    return value.macro != null ? '?${value.macro}' : switch (value) {
      PauseExpression       _ => '',
      BooleanExpression     _ => '${ConvertHelper.makeBooleanToStringOfConfirmationCharacter(value.value)}',
      IntegerExpression     _ => '${ConvertHelper.makeIntegerToString(value.value)}',
      FloaterExpression     _ => '${ConvertHelper.makeFloaterToString(value.value)}',
      StringExpression      _ => '${value.value}',
      SizeExpression        _ => '${ConvertHelper.makeFloaterToString(value.count)}${['b', 'k', 'm', 'g'][value.exponent]}',
      PathExpression        _ => '${value.content.emit()}',
      EnumerationExpression _ => '${value.item}',
      _                       => throw UnreachableException(),
    };
  }

  // #endregion

}
