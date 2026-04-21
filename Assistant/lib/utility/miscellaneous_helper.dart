import '/common.dart';
import '/setting.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class MiscellaneousHelper {

  // #region storage

  static Future<StoragePath?> pickStorageItem(
    BuildContext    context,
    StoragePickType type,
    String          locationTag,
    String?         name,
  ) async {
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var location = setting.data.storagePickerHistoryLocation[locationTag];
    var target = await StorageHelper.pick(type, location, name);
    if (target != null) {
      setting.data.storagePickerHistoryLocation[locationTag] = switch (type) {
        .loadFile      => target.parent()!,
        .loadDirectory => target,
        .saveFile      => target.parent()!,
      };
      await setting.save(apply: false);
    }
    return target;
  }

  // #endregion

}
