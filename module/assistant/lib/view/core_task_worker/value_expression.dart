import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_size.dart';
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
    super.macro,
  );
  @override
  operator ==(other) => other is PauseExpression && this.macro == other.macro;
  @override
  get hashCode => Object.hashAll([super.macro]);
}

// ----------------

class SingleValueExpression<TValue> extends ValueExpression {
  TValue value;
  SingleValueExpression(
    super.macro,
    this.value,
  );
  @override
  operator ==(other) => other is SingleValueExpression<TValue> && super.macro == other.macro && this.value == other.value;
  @override
  get hashCode => Object.hashAll([super.macro, this.value]);
}

typedef BooleanExpression = SingleValueExpression<Boolean>;

typedef IntegerExpression = SingleValueExpression<Integer>;

typedef FloaterExpression = SingleValueExpression<Floater>;

typedef StringExpression = SingleValueExpression<String>;

typedef SizeExpression = SingleValueExpression<StorageSize>;

typedef PathExpression = SingleValueExpression<StoragePath>;

// ----------------

class EnumerationExpression extends ValueExpression {
  String item;
  EnumerationExpression(
    super.macro,
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
      SizeExpression        _ => '${value.value.emit()}',
      PathExpression        _ => '${value.value.emit()}',
      EnumerationExpression _ => '${value.item}',
      _                       => throw UnreachableException(),
    };
  }

  // #endregion

}
