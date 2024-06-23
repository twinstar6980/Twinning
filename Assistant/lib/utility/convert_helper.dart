import '/common.dart';
import 'package:decimal/decimal.dart';

// ----------------

class ConvertHelper {

  // #region boolean

  static String makeBooleanToString(
    Boolean value,
  ) {
    return switch (value) {
      false => 'false',
      true  => 'true',
    };
  }

  static String makeBooleanToStringOfConfirmationCharacter(
    Boolean value,
  ) {
    return switch (value) {
      false => 'n',
      true  => 'y',
    };
  }

  // #endregion

  // #region integer

  static String makeIntegerToString(
    Integer value,
    Boolean showPositiveSign,
  ) {
    return switch (value) {
      0   => '0',
      < 0 => '${value}',
      > 0 => '${!showPositiveSign ? '' : '+'}${value}',
      _   => throw Exception(),
    };
  }

  // #endregion

  // #region floater

  static String makeFloaterToString(
    Floater value,
    Boolean showPositiveSign,
  ) {
    var valueDecimal = Decimal.parse(value.toString());
    return switch (value) {
      0.0   => '0',
      < 0.0 => '${valueDecimal}',
      > 0.0 => '${!showPositiveSign ? '' : '+'}${valueDecimal}',
      _     => throw Exception(),
    } + (!valueDecimal.isInteger ? '' : '.0');
  }

  // #endregion

  // #region string list

  static String makeStringListToStringWithLine(
    List<String> value,
  ) {
    return value.join('\n') + (value.isNotEmpty && value.last.isEmpty ? '\n' : '');
  }

  static List<String> parseStringListFromStringWithLine(
    String text,
  ) {
    var value = text.split('\n').map((value) => !value.endsWith('\r') ? value : value.substring(0, value.length - 1)).toList();
    if (value.isNotEmpty && value.last.isEmpty) {
      value.removeLast();
    }
    return value;
  }

  // #endregion

}
