import '/common.dart';
import 'package:flutter/material.dart';

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
    return await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(
          title,
          overflow: TextOverflow.ellipsis,
        ),
        content: StatefulBuilder(
          builder: (context, setState) => Column(
            mainAxisSize: MainAxisSize.min,
            children: contentBuilder(context, setState),
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
    required BuildContext                                                  context,
    required String                                                        title,
    required Widget Function(BuildContext, Void Function(Void Function())) contentBuilder,
  }) async {
    await showModalBottomSheet(
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
                  builder: contentBuilder,
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

}
