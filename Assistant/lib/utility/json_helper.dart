import '/common.dart';
import '/utility/storage_helper.dart';
import 'dart:convert';
import 'package:json5/json5.dart';

// ----------------

class JsonHelper {

  // #region serialization

  static final JsonEncoder _encoder = JsonEncoder.withIndent('\t');

  static final JsonEncoder _encoderCompact = JsonEncoder();

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
    return json5Decode(text);
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
