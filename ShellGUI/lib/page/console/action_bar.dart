// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/material.dart';
import '/page/console/messsage_type.dart';

// ----------------

class ActionBar extends StatelessWidget {

  const ActionBar({
    super.key,
    required this.content,
  });

  // ----------------

  final Widget  content;

  // ----------------

  @override
  Widget build(BuildContext context) {
    var theme = Theme.of(context);
    var typeColor = MessageType.input.color(theme);
    return Card(
      margin: const EdgeInsets.fromLTRB(1, 0, 1, 0),
      elevation: 1,
      color: Color.alphaBlend(typeColor.withOpacity(0.025), theme.colorScheme.surface),
      child: Container(
        padding: const EdgeInsets.fromLTRB(8, 4, 8, 4),
        child: SizedBox(
          height: theme.buttonTheme.height + 12,
          child: this.content,
        ),
      ),
    );
  }

}
