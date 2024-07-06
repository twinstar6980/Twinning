import '/common.dart';
import '/view/home/common.dart';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

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

  static Future<TResult?> showCustomModalDialog<TResult>(
    BuildContext      context,
    CustomModalDialog widget,
  ) async {
    return await showDialog<TResult>(
      context: context,
      builder: (context) => widget,
    );
  }

  static Future<Boolean> showCustomConfirmModalDialog(
    BuildContext context,
  ) async {
    return await showCustomModalDialog<Boolean>(context, CustomModalDialog(
      title: 'Confirm ?',
      contentBuilder: (context, setState) => [],
      actionBuilder: (context) => [
        TextButton(
          onPressed: () => Navigator.pop(context, false),
          child: const Text('No'),
        ),
        TextButton(
          onPressed: () => Navigator.pop(context, true),
          child: const Text('Yes'),
        ),
      ],
    )) ?? false;
  }

  // #endregion

  // #region bottom sheet

  static Future<TResult?> showCustomModalBottomSheet<TResult>(
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

  // #region miscellaneous

  static Future<Void> revealStorageDirectoryInNativeManagerOrShowTextDialog(
    BuildContext context,
    String       title,
    String       path,
  ) async {
    if (Platform.isWindows || Platform.isMacOS || Platform.isLinux) {
      await launchUrl(Uri.file(path), mode: LaunchMode.externalNonBrowserApplication);
    }
    if (Platform.isAndroid || Platform.isIOS) {
      assertTest(context.mounted);
      await ControlHelper.showCustomModalDialog<Void>(context, CustomModalDialog(
        title: title,
        contentBuilder: (context, setState) => [
          TextFormField(
            keyboardType: TextInputType.none,
            maxLines: null,
            inputFormatters: const [],
            decoration: const InputDecoration(
              contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
              filled: false,
              border: OutlineInputBorder(),
            ),
            readOnly: true,
            initialValue: path,
          ),
        ],
        actionBuilder: null,
      ));
    }
    return;
  }

  // #endregion

}
