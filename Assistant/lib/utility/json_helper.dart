import '/common.dart';
import '/utility/storage_helper.dart';
import 'dart:convert';
import 'package:json5/json5.dart' as lib;

// ----------------

class JsonHelper {

  // #region serialization

  static final JsonEncoder _encoder = .withIndent('\t');

  static final JsonEncoder _encoderCompact = .new();

  // ----------------

  static String serializeText(
    Object? value, {
    Boolean indented = true,
  }) {
    return (indented ? JsonHelper._encoder : JsonHelper._encoderCompact).convert(value);
  }

  static Object? deserializeText(
    String text,
  ) {
    return lib.json5Decode(text);
  }

  // ----------------

  static Future<Void> serializeFile(
    String  path,
    Object? value, {
    Boolean indented = true,
  }) async {
    return await StorageHelper.writeFileText(path, JsonHelper.serializeText(value, indented: indented));
  }

  static Future<Object?> deserializeFile(
    String path,
  ) async {
    return JsonHelper.deserializeText(await StorageHelper.readFileText(path));
  }

  // #endregion

}
