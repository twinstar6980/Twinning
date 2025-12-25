import '/common.dart';
import '/utility/storage_helper.dart';
import 'package:vdf/vdf.dart' as lib;

// ----------------

class VdfHelper {

  // #region serialization

  static String serializeText(
    Map<String, Object?> value, {
    Boolean              indented = true,
  }) {
    return lib.vdfEncode(value);
  }

  static Map<String, Object?> deserializeText(
    String text,
  ) {
    return lib.vdfDecode(text);
  }

  // ----------------

  static Future<Void> serializeFile(
    String               path,
    Map<String, Object?> value, {
    Boolean              indented = true,
  }) async {
    return await StorageHelper.writeFileText(path, serializeText(value, indented: indented));
  }

  static Future<Map<String, Object?>> deserializeFile(
    String path,
  ) async {
    return deserializeText(await StorageHelper.readFileText(path));
  }

  // #endregion

}
