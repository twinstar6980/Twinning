import '/common.dart'; // ignore: unused_import
import '/page/console/messsage_type.dart';
import 'package:flutter/material.dart';

// ----------------

class ActionBar extends StatelessWidget {

  const ActionBar({
    super.key,
    required this.content,
  });

  // ----------------

  final Widget content;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    var cardColor = MessageType.color(MessageType.input, theme);
    return Card(
      margin: const EdgeInsets.fromLTRB(1, 0, 1, 0),
      elevation: 1,
      color: Color.alphaBlend(cardColor.withOpacity(0.025), theme.colorScheme.surface),
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
