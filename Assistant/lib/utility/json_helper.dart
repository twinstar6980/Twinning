import '/common.dart';
import '/utility/storage_helper.dart';
import 'dart:convert';

// ----------------

class JsonHelper {

  // #region serialization

  static JsonEncoder _encoder = const JsonEncoder.withIndent('\t');

  static JsonEncoder _encoderCompact = const JsonEncoder();

  static JsonDecoder _decoder = const JsonDecoder();

  // ----------------

  static String serializeText(
    Object? value, {
    Boolean indented = true,
  }) {
    return (indented ? _encoder : _encoderCompact).convert(value);
  }

  static Object? deserializeText(
    String text,
  ) {
    return _decoder.convert(text);
  }

  // ----------------

  static Future<Void> serializeFile(
    String  path,
    Object? value, {
    Boolean indented = true,
  }) async {
    return await StorageHelper.writeFileText(path, serializeText(value, indented: indented));
  }

  static Future<Object?> deserializeFile(
    String path,
  ) async {
    return deserializeText(await StorageHelper.readFileText(path));
  }

  // #endregion

}
