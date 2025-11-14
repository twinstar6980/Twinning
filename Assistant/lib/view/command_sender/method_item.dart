import '/common.dart';
import '/widget/export.dart';
import '/view/command_sender/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';

// ----------------

class MethodItem extends StatelessWidget {

  const MethodItem({
    super.key,
    required this.configuration,
    required this.onSelect,
  });

  // ----------------

  final MethodConfiguration          configuration;
  final Void Function(String method) onSelect;

  // ----------------

  @override
  build(context) {
    return FlexContainer.vertical([
      StyledListTile.standardCustom(
        dense: true,
        padding: .fromLTRB(40, 0, 24, 0),
        leading: Icon(IconSetExtension.queryOutlined(this.configuration.icon)),
        content: StyledText.inherit(tooltip: true, this.configuration.name),
        trailing: VisibilityArea.of(
          enabled: this.configuration.batch != null,
          child: Icon(IconSet.layers, color: StyledColor.primary.query(context)).withStyledTooltip(message: 'Batch'),
        ),
        onPressed: (context) async {
          this.onSelect(this.configuration.identifier);
        },
      ),
    ]);
  }

}

class MethodGroupItem extends StatelessWidget {

  const MethodGroupItem({
    super.key,
    required this.configuration,
    required this.onSelect,
    required this.expanded,
    required this.onToggle,
  });

  // ----------------

  final MethodGroupConfiguration     configuration;
  final Void Function(String method) onSelect;
  final Boolean                      expanded;
  final Void Function()              onToggle;

  // ----------------

  @override
  build(context) {
    return FlexContainer.vertical([
      StyledListTile.standardCustom(
        padding: .fromLTRB(24, 0, 24, 0),
        leading: Icon(IconSetExtension.queryOutlined(this.configuration.icon)),
        content: StyledText.inherit(tooltip: true, this.configuration.name),
        trailing: FlexContainer.horizontal(mainStretch: false, [
          StyledBadge.standard(
            label: StyledText.inherit('${this.configuration.item.length}'),
          ),
          Gap.horizontal(6),
          Icon(!this.expanded ? IconSet.keyboard_arrow_down : IconSet.keyboard_arrow_left),
        ]),
        onPressed: (context) async {
          this.onToggle();
        },
      ),
      VisibilityArea.of(
        enabled: this.expanded,
        child: FlexContainer.vertical(this.configuration.item.mapIndexed((optionItemIndex, optionItem) => MethodItem(
          configuration: optionItem,
          onSelect: this.onSelect,
        ))),
      ),
    ]);
  }

}
