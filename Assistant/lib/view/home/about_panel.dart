import '/common.dart';
import '/widget/export.dart';

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
          typography: .titleMedium,
          overflow: .clip,
        ).withFlexExpanded(),
      ]),
      Gap.vertical(8),
      FlexContainer.horizontal([
        StyledText.custom(
          '© ${ApplicationInformation.year} ${ApplicationInformation.developer}. All rights reserved.',
          typography: .bodyMedium,
          overflow: .clip,
        ).withFlexExpanded(),
      ]),
    ]);
  }

}
