// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/material.dart';

// ----------------

class LaunchBarContent extends StatefulWidget {

  const LaunchBarContent({
    super.key,
    required this.onLaunch,
  });

  @override
  State<LaunchBarContent> createState() => _LaunchBarContentState();

  // ----------------

  final Future<Void> Function(List<String>) onLaunch;

}

class _LaunchBarContentState extends State<LaunchBarContent> {

  final List<String> additionalArgument = [];

  // ----------------

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Expanded(
          child: FilledButton.tonal(
            onPressed: () async {
              this.widget.onLaunch(additionalArgument);
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
                        maxLines: null,
                        decoration: const InputDecoration(
                          isDense: true,
                        ),
                        initialValue: '',
                        onChanged: (value) {
                          var valueList = value.split('\n');
                          if (valueList.isNotEmpty && valueList.last.isEmpty) {
                            valueList.removeLast();
                          }
                          additionalArgument.clear();
                          additionalArgument.addAll(valueList);
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
              if (state == null) {
                additionalArgument.clear();
              } else {
                this.widget.onLaunch(additionalArgument);
              }
            },
            child: const Text('Launch'),
          ),
        ),
      ],
    );
  }

}
