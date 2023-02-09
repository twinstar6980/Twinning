// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '/page/console/messsage_type.dart';
import '/setting.dart';

// ----------------

class NotifyOutputBar extends StatelessWidget {

  const NotifyOutputBar({
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
  Widget build(BuildContext context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    var typeColor = this.type.color(theme);
    var titleTextStyle = !setting.data.mUseLargerFontInConsole ? theme.textTheme.titleSmall! : theme.textTheme.titleMedium!;
    var descriptionTextStyle = !setting.data.mUseLargerFontInConsole ? theme.textTheme.bodySmall! : theme.textTheme.bodyMedium!.copyWith(color: theme.colorScheme.secondary);
    return Card(
      elevation: 1.0,
      color: Color.alphaBlend(typeColor.withOpacity(0.025), theme.colorScheme.surface),
      child: Container(
          padding: const EdgeInsets.all(8),
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.baseline,
            textBaseline: titleTextStyle.textBaseline,
            children: [
              Icon(
                Icons.circle,
                color: Color.alphaBlend(typeColor.withOpacity(0.50), theme.colorScheme.onSurface),
                size: 12,
              ),
              const SizedBox(width: 8),
              Expanded(
                child: SelectionArea(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        this.title,
                        style: titleTextStyle,
                      ),
                      ...this.description.map((e) => (Container(
                            padding: const EdgeInsets.fromLTRB(0, 4, 0, 0),
                            child: Text(
                              e,
                              style: descriptionTextStyle,
                            ),
                          ))),
                    ],
                  ),
                ),
              ),
            ],
          )),
    );
  }

}
