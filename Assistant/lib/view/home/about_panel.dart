import '/common.dart';
import 'package:flutter/material.dart';

// ----------------

class AboutPanel extends StatelessWidget {

  const AboutPanel({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Container(
      padding: const EdgeInsets.fromLTRB(16, 16, 16, 16),
      child: Column(
        children: [
          Row(
            children: [
              Expanded(
                child: Text(
                  '${kApplicationName} - ${kApplicationVersion}',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.titleMedium,
                ),
              ),
            ],
          ),
          const SizedBox(height: 4),
          Row(
            children: [
              Expanded(
                child: Text(
                  '© 2023-2024 TwinStar. All rights reserved.',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyMedium,
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }

}
