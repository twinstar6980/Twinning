import '/common.dart';
import '/setting.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/widget/control.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class MiscellaneousHelper {

  // #region storage

  static Future<List<StoragePath>> pickStorageItem(
    BuildContext          context,
    String                tag,
    List<StoragePickType> type,
    Boolean               multiply,
    StoragePath?          location,
    String?               name, {
    TextStyle?            textStyle = null, // TODO: remove?
  }) async {
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var actualType = null as StoragePickType?;
    assertTest(!type.isEmpty);
    if (type.length == 1) {
      actualType = type.first;
    }
    else {
      actualType = await StyledMenuExtension.show<StoragePickType>(context, StyledMenu.standard(
        position: .under,
        content: <({StoragePickType value, String text})>[
          if (type.contains(StoragePickType.loadFile)) ...[
            (value: .loadFile, text: 'Load File'),
          ],
          if (type.contains(StoragePickType.loadDirectory)) ...[
            (value: .loadDirectory, text: 'Load Directory'),
          ],
          if (type.contains(StoragePickType.saveFile)) ...[
            (value: .saveFile, text: 'Save File'),
          ],
        ].map((value) => StyledMenuItem.standard(
          value: value.value,
          content: StyledText.custom(
            value.text,
            style: textStyle,
          ),
        )),
      ));
    }
    if (location == null) {
      location = setting.data.storagePickerHistoryLocation[tag];
    }
    var target = actualType == null ? <StoragePath>[] : await StorageHelper.pick(actualType, multiply, location, name);
    if (!target.isEmpty) {
      setting.data.storagePickerHistoryLocation[tag] = switch (actualType!) {
        .loadFile      => target.first.parent()!,
        .loadDirectory => target.first,
        .saveFile      => target.first.parent()!,
      };
      await setting.save(apply: false);
    }
    return target;
  }

  // #endregion

}
