import '/common.dart';
import '/setting.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class LaunchBarContent extends StatelessWidget {

  const LaunchBarContent({
    super.key,
    required this.additionalArgument,
    required this.onLaunch,
  });

  // ----------------

  final List<String>            additionalArgument;
  final Future<Void> Function() onLaunch;

  // ----------------

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return Row(
      children: [
        Expanded(
          child: FilledButton.tonal(
            onPressed: () async {
              this.onLaunch();
            },
            onLongPress: () async {
              await showDialog<String>(
                context: context,
                builder: (context) => AlertDialog(
                  title: const Text('Additional Argument'),
                  content: Column(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      TextFormField(
                        style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
                        decoration: const InputDecoration(
                          isDense: true,
                        ),
                        maxLines: null,
                        keyboardType: TextInputType.multiline,
                        inputFormatters: const [],
                        initialValue: convertStringListToTextWithLine(this.additionalArgument),
                        onChanged: (value) {
                          this.additionalArgument.clear();
                          this.additionalArgument.addAll(convertStringListFromTextWithLine(value));
                        },
                      ),
                    ],
                  ),
                  actions: [
                    TextButton(
                      onPressed: () => Navigator.pop(context, 'OK'),
                      child: const Text('OK'),
                    ),
                  ],
                ),
              );
            },
            child: const Text('Launch'),
          ),
        ),
      ],
    );
  }

}
