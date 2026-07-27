import '/common.dart';
import '/utility/json_type.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import 'dart:convert' as lib;
import 'package:json5/json5.dart' as lib;

// ----------------

class JsonHelper {

  // #region encoding

  static final lib.JsonEncoder _encoder = .withIndent('\t');

  static final lib.JsonEncoder _encoderCompact = .new();

  // ----------------

  static String encodeText(
    JsonNode value, {
    Boolean  indented = true,
  }) {
    return (indented ? JsonHelper._encoder : JsonHelper._encoderCompact).convert(value);
  }

  static JsonNode decodeText(
    String target,
  ) {
    return lib.json5Decode(target);
  }

  // ----------------

  static Future<Void> encodeFile(
    StoragePath target,
    JsonNode    value, {
    Boolean     indented = true,
  }) async {
    return await StorageHelper.writeFileText(target, JsonHelper.encodeText(value, indented: indented));
  }

  static Future<JsonNode> decodeFile(
    StoragePath target,
  ) async {
    return JsonHelper.decodeText(await StorageHelper.readFileText(target));
  }

  // #endregion

}
