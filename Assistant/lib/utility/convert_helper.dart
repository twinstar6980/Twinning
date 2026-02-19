import '/common.dart';
import 'dart:async';
import 'dart:typed_data';
import 'dart:ui';
import 'package:decimal/decimal.dart' as lib;

// ----------------

class ConvertHelper {

  // #region enumeration

  static String makeEnumerationToStringOfSnakeCase<TType extends Enum>(
    TType value,
  ) {
    return changeStringFromCamelCaseToSnakeCase(value.name);
  }

  static TType parseEnumerationFromStringOfSnakeCase<TType extends Enum>(
    String      text,
    List<TType> list,
  ) {
    return list.firstWhere((it) => makeEnumerationToStringOfSnakeCase(it) == text);
  }

  // #endregion

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
    var valueDecimal = lib.Decimal.parse(value.toString());
    return switch (value) {
      0.0   => '0',
      < 0.0 => '${valueDecimal}',
      > 0.0 => '${!showPositiveSign ? '' : '+'}${valueDecimal}',
      _     => throw Exception(),
    } + (!valueDecimal.isInteger ? '' : '.0');
  }

  // #endregion

  // #region string

  static List<String> splitStringOfCamelCase(
    String source,
  ) {
    return RegExp(r'([A-Z][a-z]*)|([a-z]+)|([0-9]+)')
      .allMatches(source)
      .map((it) => it.group(0)!)
      .toList();
  }

  // ----------------

  static String changeStringFromCamelCaseToSnakeCase(
    String source,
  ) {
    return splitStringOfCamelCase(source).join('_').toLowerCase();
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

  // #region image

  static Future<Image> parseImageFromData(
    Uint8List data, {
    Integer?  width = null,
    Integer?  height = null,
    Boolean   isRawRgba = false,
    Boolean   isRawBgra = false,
    Boolean   isPng = false,
  }) async {
    assertTest(isRawRgba || isRawBgra || isPng);
    var value = null as Image?;
    if (isPng) {
      var codec = await instantiateImageCodec(data);
      var frame = await codec.getNextFrame();
      value = frame.image;
    }
    if (isRawRgba) {
      var completer = Completer<Image>();
      decodeImageFromPixels(data, width!, height!, .rgba8888, (image) => completer.complete(image));
      value = await completer.future;
    }
    if (isRawBgra) {
      var completer = Completer<Image>();
      decodeImageFromPixels(data, width!, height!, .bgra8888, (image) => completer.complete(image));
      value = await completer.future;
    }
    return value!;
  }

  // #endregion

}
