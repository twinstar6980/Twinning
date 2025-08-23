import '/common.dart';
import '/view/modding_worker/message_type.dart';
import '/view/modding_worker/main_page.dart';
import 'package:flutter/material.dart';

// ----------------

class MessageCard extends StatelessWidget {

  const MessageCard({
    super.key,
    required this.type,
    required this.title,
    required this.description,
  });

  // ----------------

  final MessageType  type;
  final String       title;
  final List<String> description;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    var cardColor = this.type.color(theme);
    var titleStyle = theme.textTheme.titleSmall!.copyWith(
      color: theme.colorScheme.onSurface,
    ).selfLet((it) => withSpecialFontTextStyle(context, it));
    var descriptionStyle = theme.textTheme.bodySmall!.copyWith(
      color: theme.colorScheme.onSurfaceVariant,
    ).selfLet((it) => withSpecialFontTextStyle(context, it));
    return Card(
      margin: EdgeInsets.fromLTRB(0, 0, 0, 0),
      color: Color.alphaBlend(cardColor.withValues(alpha: 0.02), theme.colorScheme.surfaceContainerLow),
      child: Container(
        padding: EdgeInsets.fromLTRB(12, 8, 12, 8),
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Icon(
                  IconSymbols.circle,
                  size: 12,
                  fill: 0.6,
                  color: Color.alphaBlend(cardColor.withValues(alpha: 0.50), theme.colorScheme.onSurface),
                ),
                Text(
                  '',
                  overflow: TextOverflow.clip,
                  style: titleStyle,
                ),
              ],
            ),
            SizedBox(width: 8),
            Column(
              crossAxisAlignment: CrossAxisAlignment.stretch,
              children: [
                Text(
                  this.title,
                  overflow: TextOverflow.clip,
                  style: titleStyle,
                ).withSelectionArea(
                ),
                ...this.description.expand((item) => [
                  SizedBox(height: 4),
                  Text(
                    item,
                    overflow: TextOverflow.clip,
                    style: descriptionStyle,
                  ).withSelectionArea(
                  ),
                ]),
              ],
            ).withExpanded(),
          ],
        ),
      ),
    );
  }

}
