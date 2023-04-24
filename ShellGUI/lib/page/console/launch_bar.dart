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

  final Future<Void> Function() onLaunch;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Expanded(
          child: OutlinedButton(
            onPressed: () {
              this.onLaunch();
            },
            child: const Text('Launch'),
          ),
        ),
      ],
    );
  }

}
