import '/common.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import 'package:vdf/vdf.dart' as lib;

// ----------------

class VdfHelper {

  // #region encoding

  static String encodeText(
    Map<String, Object?> value, {
    Boolean              indented = true,
  }) {
    return lib.vdfEncode(value);
  }

  static Map<String, Object?> decodeText(
    String target,
  ) {
    return lib.vdfDecode(target);
  }

  // ----------------

  static Future<Void> encodeFile(
    StoragePath          target,
    Map<String, Object?> value, {
    Boolean              indented = true,
  }) async {
    return await StorageHelper.writeFileText(target, VdfHelper.encodeText(value, indented: indented));
  }

  static Future<Map<String, Object?>> decodeFile(
    StoragePath target,
  ) async {
    return VdfHelper.decodeText(await StorageHelper.readFileText(target));
  }

  // #endregion

}
