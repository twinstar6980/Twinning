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
    return Column(
      children: [
        Row(
          children: [
            Expanded(
              child: Text(
                '${ApplicationInformation.name} - v${ApplicationInformation.version}',
                overflow: TextOverflow.clip,
                style: theme.textTheme.titleMedium,
              ),
            ),
          ],
        ),
        SizedBox(height: 8),
        Row(
          children: [
            Expanded(
              child: Text(
                '© ${ApplicationInformation.year} ${ApplicationInformation.developer}. All rights reserved.',
                overflow: TextOverflow.clip,
                style: theme.textTheme.bodyMedium,
              ),
            ),
          ],
        ),
      ],
    );
  }

}
