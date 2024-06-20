import '/common.dart';
import '/setting.dart';
import '/view/modding_worker/message_type.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    var cardColor = this.type.color(theme);
    var titleStyle = theme.textTheme.titleSmall?.copyWith(
      fontFamily: '',
      fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
    );
    var descriptionStyle = theme.textTheme.bodySmall?.copyWith(
      fontFamily: '',
      fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
      color: theme.colorScheme.secondary,
    );
    return Card(
      margin: const EdgeInsets.fromLTRB(1, 0, 1, 0),
      color: Color.alphaBlend(cardColor.withOpacity(0.025), theme.colorScheme.surfaceContainerLow),
      child: Container(
        padding: const EdgeInsets.fromLTRB(8, 8, 8, 8),
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Icon(
                  IconSymbols.circle,
                  color: Color.alphaBlend(cardColor.withOpacity(0.50), theme.colorScheme.onSurface),
                  fill: 0.6,
                  size: 12,
                ),
                Text(
                  '',
                  overflow: TextOverflow.clip,
                  style: titleStyle,
                ),
              ],
            ),
            const SizedBox(width: 8),
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    this.title,
                    overflow: TextOverflow.clip,
                    style: titleStyle,
                  ),
                  ...this.description.map(
                    (value) => Container(
                      padding: const EdgeInsets.fromLTRB(0, 4, 0, 0),
                      child: Text(
                        value,
                        overflow: TextOverflow.clip,
                        style: descriptionStyle,
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

}
