import '/common.dart';
import '/setting.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/application_platform_method.dart';
import '/widget/export.dart';
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

  // ----------------

  static Future<StoragePath?> resolveAndroidContentUri(
    BuildContext context,
    Uri          uri,
  ) async {
    var result = (await ApplicationPlatformMethod.instance.resolveContentUri(uri.toString(), null)).path;
    if (result == null) {
      var canDuplicate = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
        title: 'Unknown Content Uri',
        contentBuilder: (context, setStateForPanel) => [
          FlexContainer.horizontal([
            StyledText.custom(
              uri.toString(),
              overflow: .clip,
            ).withSelectableArea(
            ).withFlexExpanded(),
          ]),
        ],
        actionBuilder: (context) => [
          StyledButton.text(
            content: StyledText.inherit('Ignore'),
            onPressed: (context) => Navigator.pop(context, false),
          ),
          StyledButton.text(
            content: StyledText.inherit('Duplicate'),
            onPressed: (context) => Navigator.pop(context, true),
          ),
        ],
      )) ?? false;
      if (canDuplicate) {
        var fallback = await StorageHelper.query(.applicationCache);
        result = (await ApplicationPlatformMethod.instance.resolveContentUri(uri.toString(), fallback.emitNative())).path;
      }
    }
    return result == null ? null : .of(result);
  }

  // #endregion

}
