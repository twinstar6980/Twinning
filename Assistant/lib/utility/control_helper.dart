import '/common.dart';
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

  static Future<TResult?> showCustomModalDialog<TResult>({
    required BuildContext                                                         context,
    required String                                                               title,
    required List<Widget> Function(BuildContext, Void Function(Void Function()))? contentBuilder,
    required List<Widget> Function(BuildContext)?                                 actionBuilder,
  }) async {
    return await showDialog<TResult>(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(
          title,
          overflow: TextOverflow.ellipsis,
        ),
        content: contentBuilder == null
          ? const SizedBox.shrink()
          : SingleChildScrollView(
            child: StatefulBuilder(
              builder: (context, setState) => Column(
                mainAxisSize: MainAxisSize.min,
                children: contentBuilder(context, setState),
              ),
            ),
          ),
        actions: actionBuilder == null
          ? [
            TextButton(
              onPressed: () => Navigator.pop(context, null),
              child: const Text('Okay'),
            ),
          ]
          : actionBuilder(context),
      ),
    );
  }

  static Future<Boolean> showCustomConfirmDialog({
    required BuildContext context,
  }) async {
    return await showCustomModalDialog<Boolean>(
      context: context,
      title: 'Confirm ?',
      contentBuilder: null,
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
        actionBuilder: null,
      );
    }
    return;
  }

  // #endregion

}
