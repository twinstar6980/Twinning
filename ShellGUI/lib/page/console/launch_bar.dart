import '/common.dart';
import 'package:flutter/material.dart';

// ----------------

class LaunchBarContent extends StatefulWidget {

  const LaunchBarContent({
    super.key,
    required this.onLaunch,
  });

  @override
  createState() => _LaunchBarContentState();

  // ----------------

  final Future<Void> Function(List<String>) onLaunch;

}

class _LaunchBarContentState extends State<LaunchBarContent> {

  List<String> _additionalArgument = [];

  // ----------------

  @override
  build(context) {
    return Row(
      children: [
        Expanded(
          child: FilledButton.tonal(
            onPressed: () async {
              this.widget.onLaunch([]);
            },
            onLongPress: () async {
              var state = await showDialog<String>(
                context: context,
                builder: (context) => AlertDialog(
                  title: const Text('Additional Argument'),
                  content: Column(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      TextFormField(
                        decoration: const InputDecoration(
                          isDense: true,
                        ),
                        maxLines: null,
                        keyboardType: TextInputType.multiline,
                        inputFormatters: const [],
                        initialValue: '',
                        onChanged: (value) {
                          var parsedValue = value.split('\n');
                          if (parsedValue.isNotEmpty && parsedValue.last.isEmpty) {
                            parsedValue.removeLast();
                          }
                          this._additionalArgument.clear();
                          this._additionalArgument.addAll(parsedValue);
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
              if (state != null) {
                this.widget.onLaunch(this._additionalArgument);
              }
              this._additionalArgument.clear();
            },
            child: const Text('Launch'),
          ),
        ),
      ],
    );
  }

}
