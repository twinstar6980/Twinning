// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/material.dart';
import '/page/console/messsage_type.dart';

// ----------------

class ActionBar extends StatelessWidget {

  const ActionBar({
    super.key,
    required this.running,
    required this.content,
  });

  // ----------------

  final Boolean running;
  final Widget  content;

  // ----------------

  @override
  Widget build(BuildContext context) {
    var theme = Theme.of(context);
    var typeColor = MessageType.input.color(theme);
    return Card(
      elevation: 1.0,
      color: Color.alphaBlend(typeColor.withOpacity(0.025), theme.colorScheme.surface),
      child: Container(
        padding: const EdgeInsets.fromLTRB(8, 4, 8, 4),
        child: Column(
          children: [
            SizedBox(
              height: theme.buttonTheme.height + 12,
              child: this.content,
            ),
            LinearProgressIndicator(
              value: this.running ? null : 0.0,
            ),
          ],
        ),
      ),
    );
  }

}
