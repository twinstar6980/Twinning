import '/common.dart';
import '/widget/export.dart';
import '/view/modding_worker/message_type.dart';
import '/view/modding_worker/main_page.dart';
import 'package:flutter/widgets.dart';

// ----------------

class MessageCard extends StatelessWidget {

  const MessageCard({
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
  build(context) {
    var cardColor = this.type.color(context);
    var textStyle = TextStyle(inherit: true).selfLet((it) => withSpecialFontTextStyle(context, it));
    return StyledCard.elevated(
      color: Color.alphaBlend(cardColor.withValues(alpha: 0.02), StyledColorExtension.value(context, StyledColor.surfaceContainerLow)),
      content: BoxContainer.of(
        padding: EdgeInsets.fromLTRB(12, 8, 12, 8),
        child: FlexContainer.horizontal(crossAlign: FlexContainerCrossAlign.start, [
          FlexContainer.horizontal([
            Icon(
              IconSet.circle,
              size: 12,
              fill: 0.6,
              color: Color.alphaBlend(cardColor.withValues(alpha: 0.50), StyledColorExtension.value(context, StyledColor.onSurface)),
            ),
            StyledText.custom(
              '',
              variant: StyledTextVariant.titleSmall,
              overflow: TextOverflow.clip,
              style: textStyle,
            ),
          ]),
          Gap.horizontal(8),
          FlexContainer.vertical(crossAlign: FlexContainerCrossAlign.stretch, [
            StyledText.custom(
              this.title,
              variant: StyledTextVariant.titleSmall,
                color: StyledColor.onSurface,
              overflow: TextOverflow.clip,
              style: textStyle,
            ).withSelectableArea(
            ),
            ...this.description.expand((item) => [
              Gap.vertical(4),
              StyledText.custom(
                item,
                variant: StyledTextVariant.bodySmall,
                color: StyledColor.onSurfaceVariant,
                overflow: TextOverflow.clip,
                style: textStyle,
              ).withSelectableArea(
              ),
            ]),
          ]).withFlexExpanded(),
        ]),
      ),
    );
  }

}
