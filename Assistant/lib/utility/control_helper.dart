import '/common.dart';
import '/view/home/common.dart';
import '/utility/storage_helper.dart';
import 'package:flutter/material.dart';

// ----------------

class ControlHelper {

  // #region common

  static Void postTask(
    Void Function() action,
  ) async {
    WidgetsBinding.instance.addPostFrameCallback((_) { action(); });
    return;
  }

  // #endregion

  // #region dialog

  static Future<TResult?> showDialogAsModal<TResult>(
    BuildContext      context,
    CustomModalDialog widget,
  ) async {
    return await showDialog<TResult>(
      context: context,
      builder: (context) => widget,
    );
  }

  // ----------------

  static Future<Boolean> showDialogForConfirm(
    BuildContext context,
  ) async {
    return await showDialogAsModal<Boolean>(context, CustomModalDialog(
      title: 'Confirm ?',
      contentBuilder: (context, setState) => [],
      actionBuilder: (context) => [
        TextButton(
          child: const Text('Cancel'),
          onPressed: () => Navigator.pop(context, false),
        ),
        TextButton(
          child: const Text('Continue'),
          onPressed: () => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
  }

  static Future<Void> showDialogForRevealStoragePath(
    BuildContext context,
    String       title,
    String       path,
  ) async {
    var canContinue = await showDialogAsModal<Boolean>(context, CustomModalDialog(
      title: title,
      contentBuilder: (context, setState) => [
        CustomTextField(
          keyboardType: TextInputType.none,
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
            filled: false,
            border: OutlineInputBorder(),
          ),
          value: path,
          onChanged: null,
        ),
      ],
      actionBuilder: (context) => [
        TextButton(
          child: const Text('Cancel'),
          onPressed: () => Navigator.pop(context, false),
        ),
        TextButton(
          child: const Text('Reveal'),
          onPressed: () => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
    if (canContinue) {
      await StorageHelper.reveal(path);
    }
    return;
  }

  // #endregion

  // #region bottom sheet

  static Future<TResult?> showBottomSheetAsModal<TResult>(
    BuildContext           context,
    CustomModalBottomSheet widget,
  ) async {
    return await showModalBottomSheet<TResult>(
      context: context,
      elevation: 3,
      builder: (context) => widget,
    );
  }

  // #endregion

}
