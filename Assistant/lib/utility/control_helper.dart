import '/common.dart';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

// ----------------

class ControlHelper {

  // #region common

  static Future<Void> runAfterNextFrame(
    Future<Void> Function() action,
  ) async {
    await WidgetsBinding.instance.endOfFrame;
    await action();
    return;
  }

  // #endregion

  // #region dialog

  static Future<Void> showCustomModalDialog({
    required BuildContext                                                        context,
    required String                                                              title,
    required List<Widget> Function(BuildContext, Void Function(Void Function())) contentBuilder,
  }) async {
    return await showDialog<Void>(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(
          title,
          overflow: TextOverflow.ellipsis,
        ),
        content: SingleChildScrollView(
          child: StatefulBuilder(
            builder: (context, setState) => Column(
              mainAxisSize: MainAxisSize.min,
              children: contentBuilder(context, setState),
            ),
          ),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, 'OK'),
            child: const Text('OK'),
          ),
        ],
      ),
    );
  }

  static Future<Void> showCustomFullDialog({
    required BuildContext                                                        context,
    required String                                                              title,
    required List<Widget> Function(BuildContext, Void Function(Void Function())) contentBuilder,
  }) async {
    return await showDialog<Void>(
      context: context,
      builder: (context) => Dialog.fullscreen(
        child: Column(
          children: [
            Row(
              children: [
                Text(
                  title,
                  overflow: TextOverflow.ellipsis,
                ),
                TextButton(
                  onPressed: () => Navigator.pop(context, 'OK'),
                  child: const Text('OK'),
                ),
              ],
            ),
            Expanded(
              child: SingleChildScrollView(
                child: StatefulBuilder(
                  builder: (context, setState) => Column(
                    mainAxisSize: MainAxisSize.min,
                    children: contentBuilder(context, setState),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  // ----------------

  static Future<Boolean> showCustomConfirmDialog({
    required BuildContext context,
  }) async {
    return await showDialog<Boolean>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text(
          'Confirm ?',
          overflow: TextOverflow.ellipsis,
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('No'),
          ),
          TextButton(
            onPressed: () => Navigator.pop(context, true),
            child: const Text('Yes'),
          ),
        ],
      ),
    ) ?? false;
  }

  // #endregion

  // #region bottom sheet

  static Future<Void> showCustomModalBottomSheet({
    required BuildContext                                                        context,
    required String                                                              title,
    required List<Widget> Function(BuildContext, Void Function(Void Function())) contentBuilder,
  }) async {
    await showModalBottomSheet<Void>(
      context: context,
      elevation: 3,
      builder: (context) {
        var theme = Theme.of(context);
        return Column(
          children: [
            Container(
              padding: const EdgeInsets.fromLTRB(16, 12, 16, 12),
              child: Row(
                children: [
                  Expanded(
                    child: Text(
                      title,
                      overflow: TextOverflow.ellipsis,
                      textAlign: TextAlign.left,
                      style: theme.textTheme.titleMedium?.copyWith(
                        color: theme.colorScheme.primary,
                      ),
                    ),
                  ),
                ],
              ),
            ),
            const Divider(height: 1),
            Expanded(
              child: SingleChildScrollView(
                child: StatefulBuilder(
                  builder: (context, setState) => Column(
                    mainAxisSize: MainAxisSize.min,
                    children: contentBuilder(context, setState),
                  ),
                ),
              ),
            ),
          ],
        );
      },
    );
    return;
  }

  // #endregion

  // #region miscellaneous

  static Future<Void> revealStorageDirectoryInNativeManagerOrShowTextDialog({
    required BuildContext context,
    required String       title,
    required String       path,
  }) async {
    if (Platform.isWindows || Platform.isMacOS || Platform.isLinux) {
      await launchUrl(Uri.file(path), mode: LaunchMode.externalNonBrowserApplication);
    }
    if (Platform.isAndroid || Platform.isIOS) {
      assertTest(context.mounted);
      await ControlHelper.showCustomModalDialog(
        context: context,
        title: title,
        contentBuilder: (context, setState) => [
          TextFormField(
            keyboardType: TextInputType.none,
            maxLines: null,
            inputFormatters: const [],
            decoration: const InputDecoration(
              isDense: true,
              border: OutlineInputBorder(),
            ),
            readOnly: true,
            initialValue: path,
          ),
        ],
      );
    }
    return;
  }

  // #endregion

}
