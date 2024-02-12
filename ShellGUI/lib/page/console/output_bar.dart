import '/common.dart';
import '/page/console/messsage_type.dart';
import '/setting.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class MessageOutputBar extends StatelessWidget {

  const MessageOutputBar({
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
    var cardColor = MessageType.color(this.type, theme);
    var titleStyle = (!setting.data.mConsoleFontUseLargerSize ? theme.textTheme.titleSmall : theme.textTheme.titleMedium)?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]);
    var descriptionStyle = (!setting.data.mConsoleFontUseLargerSize ? theme.textTheme.bodySmall : theme.textTheme.bodyMedium)?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy], color: theme.colorScheme.secondary);
    return Card(
      margin: const EdgeInsets.fromLTRB(1, 0, 1, 0),
      elevation: 1,
      color: Color.alphaBlend(cardColor.withOpacity(0.025), theme.colorScheme.surface),
      child: Container(
        padding: const EdgeInsets.all(8),
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Icon(
                  Icons.circle,
                  color: Color.alphaBlend(cardColor.withOpacity(0.50), theme.colorScheme.onSurface),
                  size: 12,
                ),
                Text(
                  '',
                  style: titleStyle,
                ),
              ],
            ),
            const SizedBox(width: 8),
            Expanded(
              child: SelectionArea(
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text(
                      this.title,
                      style: titleStyle,
                    ),
                    ...this.description.map(
                      (value) => Container(
                        padding: const EdgeInsets.fromLTRB(0, 4, 0, 0),
                        child: Text(
                          value,
                          style: descriptionStyle,
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

}
