import '/common.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import 'dart:convert';
import 'package:json5/json5.dart' as lib;

// ----------------

class JsonHelper {

  // #region encoding

  static final JsonEncoder _encoder = .withIndent('\t');

  static final JsonEncoder _encoderCompact = .new();

  // ----------------

  static String encodeText(
    Object? value, {
    Boolean indented = true,
  }) {
    return (indented ? JsonHelper._encoder : JsonHelper._encoderCompact).convert(value);
  }

  static Object? decodeText(
    String target,
  ) {
    return lib.json5Decode(target);
  }

  // ----------------

  static Future<Void> encodeFile(
    StoragePath target,
    Object?     value, {
    Boolean     indented = true,
  }) async {
    return await StorageHelper.writeFileText(target, JsonHelper.encodeText(value, indented: indented));
  }

  static Future<Object?> decodeFile(
    StoragePath target,
  ) async {
    return JsonHelper.decodeText(await StorageHelper.readFileText(target));
  }

  // #endregion

}
