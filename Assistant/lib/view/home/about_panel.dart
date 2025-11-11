import '/common.dart';
import '/widget/export.dart';
import 'package:flutter/widgets.dart';

// ----------------

class AboutPanel extends StatelessWidget {

  const AboutPanel({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return FlexContainer.vertical([
      FlexContainer.horizontal([
        StyledText.custom(
          '${ApplicationInformation.name} - v${ApplicationInformation.version}',
          overflow: TextOverflow.clip,
          variant: StyledTextVariant.titleMedium,
        ).withFlexExpanded(),
      ]),
      Gap.vertical(8),
      FlexContainer.horizontal([
        StyledText.custom(
          '© ${ApplicationInformation.year} ${ApplicationInformation.developer}. All rights reserved.',
          overflow: TextOverflow.clip,
          variant: StyledTextVariant.bodyMedium,
        ).withFlexExpanded(),
      ]),
    ]);
  }

}
