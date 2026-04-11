import '/common.dart';
import 'dart:async';
import 'dart:typed_data';
import 'dart:ui';
import 'package:decimal/decimal.dart' as lib;
import 'package:stack_trace/stack_trace.dart' as lib;

// ----------------

class ConvertHelper {

  // #region exception

  static String generateExceptionMessage(
    Object      exception,
    StackTrace? stack,
  ) {
    var result = '${exception}';
    if (stack != null) {
      var trace = lib.Trace.from(stack);
      for (var frame in trace.frames) {
        result += '\n@ ${frame.library.selfLet((it) => it.startsWith('package:assistant') ? it.substring('package:'.length) : it)}:${frame.line ?? '?'}:${frame.column ?? '?'} ${frame.member ?? '?'}';
      }
    }
    return result;
  }

  // #endregion

  // #region enumeration

  static String makeEnumerationToStringOfSnakeCase<TValue extends Enum>(
    TValue value,
  ) {
    return ConvertHelper.changeStringFromCamelCaseToSnakeCase(value.name);
  }

  static TValue parseEnumerationFromStringOfSnakeCase<TValue extends Enum>(
    String       text,
    List<TValue> list,
  ) {
    return list.firstWhere((it) => ConvertHelper.makeEnumerationToStringOfSnakeCase(it) == text);
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
  ) {
    return '${value}';
  }

  // #endregion

  // #region floater

  static String makeFloaterToString(
    Floater value,
  ) {
    var valueDecimal = lib.Decimal.parse(value.toString());
    return '${valueDecimal}${(!valueDecimal.isInteger ? '' : '.0')}';
  }

  // #endregion

  // #region character

  static Boolean isLetter(
    String value,
  ) {
    if (value.length != 1) {
      return false;
    }
    var code = value.codeUnitAt(0) | 0x20;
    return 'a'.codeUnitAt(0) <= code && code <= 'z'.codeUnitAt(0);
  }

  // ----------------

  static Boolean isPathDot(
    String value,
  ) {
    if (value.length != 1) {
      return false;
    }
    return value == '.';
  }

  static Boolean isPathDirectorySeparator(
    String value,
  ) {
    if (value.length != 1) {
      return false;
    }
    return value == '/' || value == '\\';
  }

  static Boolean isPathVolumeSeparator(
    String value,
  ) {
    if (value.length != 1) {
      return false;
    }
    return value == ':';
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
    return ConvertHelper.splitStringOfCamelCase(source).join('_').toLowerCase();
  }

  // #endregion

  // #region string list

  static String makeStringListToStringWithLine(
    List<String> value,
  ) {
    return value.join('\n') + (!value.isEmpty && value.last.isEmpty ? '\n' : '');
  }

  static List<String> parseStringListFromStringWithLine(
    String text,
  ) {
    var value = text.split('\n').map((value) => !value.endsWith('\r') ? value : value.substring(0, value.length - 1)).toList();
    if (!value.isEmpty && value.last.isEmpty) {
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
    assertTest([isRawRgba, isRawBgra, isPng].where((it) => it).length == 1);
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
