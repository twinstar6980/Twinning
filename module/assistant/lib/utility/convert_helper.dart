import '/common.dart';
import 'dart:async' as lib;
import 'dart:typed_data' as lib;
import 'dart:math' as lib;
import 'dart:ui' as lib;
import 'package:decimal/decimal.dart' as lib;
import 'package:stack_trace/stack_trace.dart' as lib;

// ----------------

class ConvertHelper {

  // #region exception

  static List<String> generateExceptionMessage(
    Object      exception,
    StackTrace? stack,
  ) {
    var result = <String>[];
    result.add('# ${exception.runtimeType}');
    result.add('\$ message: ${exception}');
    if (stack != null) {
      for (var frame in lib.Trace.from(stack).frames) {
        var framePart = [
          frame.library.selfLet((it) => it.startsWith('package:assistant') ? it.substring('package:'.length) : it),
          frame.line ?? '?',
          frame.column ?? '?',
          frame.member ?? '?',
        ];
        result.add('@ ${framePart[0]}:${framePart[1]}:${framePart[2]} ${framePart[3]}');
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

  static Boolean parseBooleanFromString(
    String text,
  ) {
    return .parse(text);
  }

  // ----------------

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

  static Integer parseIntegerFromString(
    String text,
  ) {
    return .parse(text);
  }

  // #endregion

  // #region floater

  static String makeFloaterToString(
    Floater value,
  ) {
    var text = '';
    do {
      if (value.isNaN) {
        text += 'NaN';
        break;
      }
      if (value.isInfinite) {
        if (value < 0.0) {
          text += '-';
        }
        text += 'Inf';
        break;
      }
      var valueDecimal = lib.Decimal.parse(value.toString());
      text += '${valueDecimal}';
      if (valueDecimal.isInteger) {
        text += '.0';
      }
    }
    while (false);
    return text;
  }

  static Floater parseFloaterFromString(
    String  text,
  ) {
    var value = 0.0;
    do {
      if (text == 'NaN') {
        value = Floater.nan;
        break;
      }
      if (text == 'Inf' || text == '+Inf' || text == '-Inf') {
        value = text == '-Inf' ? -Floater.infinity : Floater.infinity;
        break;
      }
      value = Floater.parse(text);
    }
    while (false);
    return value;
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

  static String normalizeStringLineFeed(
    String source,
  ) {
    return source.replaceAll('\r\n', '\n');
  }

  // ----------------

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
    return ConvertHelper.splitStringOfCamelCase(source).map((it) => it.toLowerCase()).join('_');
  }

  static String changeStringFromCamelCaseToHeadlineStyle(
    String source,
  ) {
    return ConvertHelper.splitStringOfCamelCase(source).map((it) => '${it.substring(0, 1).toUpperCase()}${it.substring(1).toLowerCase()}').join(' ');
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
    var value = ConvertHelper.normalizeStringLineFeed(text).split('\n');
    if (!value.isEmpty && value.last.isEmpty) {
      value.removeLast();
    }
    return value;
  }

  // #endregion

  // #region datetime

  static String makeDateTimeToString(
    DateTime value,
  ) {
    return '${(value.year % 100).toString().padLeft(2, '0')}-${value.month.toString().padLeft(2, '0')}-${value.day.toString().padLeft(2, '0')}_${value.hour.toString().padLeft(2, '0')}-${value.minute.toString().padLeft(2, '0')}-${value.second.toString().padLeft(2, '0')}_${value.millisecond.toString().padLeft(3, '0')}';
  }

  // #endregion

  // #region color

  static String makeColorToString(
    lib.Color value,
    Boolean   withAlpha,
  ) {
    var text = value.toARGB32().toRadixString(16).padLeft(8, '0');
    if (!withAlpha) {
      text = text.substring(2);
    }
    return text;
  }

  static lib.Color parseColorFromString(
    String  text,
    Boolean withAlpha,
  ) {
    var value = lib.Color(Integer.tryParse(text.substring(text.length - lib.min(8, text.length)), radix: 16) ?? 0x00000000);
    if (!withAlpha) {
      value = value.withValues(alpha: 0.0);
    }
    return value;
  }

  // #endregion

  // #region image

  static Future<lib.Image> parseImageFromData(
    lib.Uint8List data, {
    Integer?      width = null,
    Integer?      height = null,
    Boolean       isRawRgba = false,
    Boolean       isRawBgra = false,
    Boolean       isPng = false,
  }) async {
    assertTest([isRawRgba, isRawBgra, isPng].where((it) => it).length == 1);
    var value = null as lib.Image?;
    if (isPng) {
      var codec = await lib.instantiateImageCodec(data);
      var frame = await codec.getNextFrame();
      value = frame.image;
    }
    if (isRawRgba) {
      var completer = lib.Completer<lib.Image>();
      lib.decodeImageFromPixels(data, width!, height!, .rgba8888, (image) => completer.complete(image));
      value = await completer.future;
    }
    if (isRawBgra) {
      var completer = lib.Completer<lib.Image>();
      lib.decodeImageFromPixels(data, width!, height!, .bgra8888, (image) => completer.complete(image));
      value = await completer.future;
    }
    return value!;
  }

  // #endregion

}
