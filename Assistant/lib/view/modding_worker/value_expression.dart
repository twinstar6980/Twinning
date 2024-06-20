import '/common.dart';
import '/utility/convert_helper.dart';

// ----------------

class ValueExpression {
  ValueExpression(
  );
}

// ----------------

class PauseExpression extends ValueExpression {
  PauseExpression(
  );
}

class BooleanExpression extends ValueExpression {
  Boolean value;
  BooleanExpression(
    this.value,
  );
}

class IntegerExpression extends ValueExpression {
  Integer value;
  IntegerExpression(
    this.value,
  );
}

class FloaterExpression extends ValueExpression {
  Floater value;
  FloaterExpression(
    this.value,
  );
}

class SizeExpression extends ValueExpression {
  Floater count;
  Integer exponent;
  SizeExpression(
    this.count,
    this.exponent,
  );
}

class StringExpression extends ValueExpression {
  String value;
  StringExpression(
    this.value,
  );
}

class PathExpression extends ValueExpression {
  String content;
  PathExpression(
    this.content,
  );
}

class EnumerationExpression extends ValueExpression {
  String item;
  EnumerationExpression(
    this.item,
  );
}

// ----------------

class ValueExpressionHelper {

  // #region utility

  static String makeString(
    ValueExpression value,
  ) {
    return switch (value) {
      PauseExpression       _ => '',
      BooleanExpression     _ => ConvertHelper.makeBooleanToStringOfConfirmationCharacter(value.value),
      IntegerExpression     _ => ConvertHelper.makeIntegerToString(value.value, true),
      FloaterExpression     _ => ConvertHelper.makeFloaterToString(value.value, true),
      SizeExpression        _ => '${ConvertHelper.makeFloaterToString(value.count, false)}${['b', 'k', 'm', 'g'][value.exponent]}',
      StringExpression      _ => value.value,
      PathExpression        _ => '${value.content}',
      EnumerationExpression _ => '${value.item}',
      _                       => throw Exception(),
    };
  }

  // #endregion

}
