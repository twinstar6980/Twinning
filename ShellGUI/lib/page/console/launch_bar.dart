// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/material.dart';

// ----------------

class LaunchBarContent extends StatelessWidget {

  const LaunchBarContent({
    super.key,
    required this.onLaunch,
  });

  // ----------------

  final Future<Void> Function(List<String>) onLaunch;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Expanded(
          child: FilledButton.tonal(
            onPressed: () {
              this.onLaunch([]);
            },
            child: const Text('启动'),
          ),
        ),
      ],
    );
  }

}
