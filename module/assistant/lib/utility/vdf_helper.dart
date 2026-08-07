import '/common.dart';
import '/utility/vdf_type.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import 'package:vdf/vdf.dart' as lib;

// ----------------

class VdfHelper {

  // #region encoding

  static String encodeText(
    VdfNode value,
  ) {
    return lib.vdfEncode(value);
  }

  static VdfNode decodeText(
    String target,
  ) {
    return lib.vdfDecode(target);
  }

  // ----------------

  static Future<Void> encodeFile(
    StoragePath target,
    VdfNode     value,
  ) async {
    return await StorageHelper.writeFileText(target, VdfHelper.encodeText(value));
  }

  static Future<VdfNode> decodeFile(
    StoragePath target,
  ) async {
    return VdfHelper.decodeText(await StorageHelper.readFileText(target));
  }

  // #endregion

}
