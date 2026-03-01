import '/common.dart';
import '/widget/export.dart';
import '/view/core_resource_shipper/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';

// ----------------

class OptionItem extends StatelessWidget {

  const OptionItem({
    super.key,
    required this.configuration,
    required this.match,
    required this.enableFilter,
    required this.enableBatch,
    required this.onSelect,
  });

  // ----------------

  final OptionConfiguration                                                                            configuration;
  final ({Boolean singleEnabled, Boolean singleFiltered, Boolean batchEnabled, Boolean batchFiltered}) match;
  final Boolean                                                                                        enableFilter;
  final Boolean                                                                                        enableBatch;
  final Void Function(String? method, Map<String, Object>? argument)                                   onSelect;

  // ----------------

  @override
  build(context) {
    var enabled = !this.enableBatch ? this.match.singleEnabled : this.match.batchEnabled;
    return VisibilityArea.of(
      enabled: !this.enableFilter || (!this.enableBatch ? this.match.singleEnabled && this.match.singleFiltered : this.match.batchEnabled && this.match.batchFiltered),
      child: FlexContainer.vertical([
        StyledListTile.standardCustom(
          enabled: enabled,
          dense: true,
          padding: .fromLTRB(40, 0, 16, 0),
          leading: IconView.of(IconSetExtension.queryOutlined(this.configuration.icon)),
          content: StyledText.custom(tooltip: enabled, this.configuration.name),
          trailing: FlexContainer.horizontal(mainStretch: false, [
            StyledButton.text(
              tooltip: 'Preset',
              content: FlexContainer.horizontal([
                Gap.horizontal(8),
                StyledText.inherit('${this.configuration.preset.nonNulls.length}'),
                Gap.horizontal(6),
                IconView.of(IconSet.flash_on),
              ]),
              onPressed: (context) async {
                var preset = await StyledMenuExtension.show<PresetConfiguration>(context, StyledMenu.standard(
                  position: .under,
                  content: this.configuration.preset.map((preset) => preset == null ? null : StyledMenuItem.standard(
                    value: preset,
                    content: StyledText.inherit(preset.name),
                  )),
                ));
                if (preset != null) {
                  this.onSelect(this.configuration.method, preset.argument);
                }
              },
            ),
          ]),
          onPressed: (context) async {
            this.onSelect(this.configuration.method, null);
          },
        ),
      ]),
    );
  }

}

class OptionGroupItem extends StatelessWidget {

  const OptionGroupItem({
    super.key,
    required this.configuration,
    required this.match,
    required this.enableFilter,
    required this.enableBatch,
    required this.onSelect,
    required this.expanded,
    required this.onToggle,
  });

  // ----------------

  final OptionGroupConfiguration                                                                             configuration;
  final List<({Boolean singleEnabled, Boolean singleFiltered, Boolean batchEnabled, Boolean batchFiltered})> match;
  final Boolean                                                                                              enableFilter;
  final Boolean                                                                                              enableBatch;
  final Void Function(String? method, Map<String, Object>? argument)                                         onSelect;
  final Boolean                                                                                              expanded;
  final Void Function()                                                                                      onToggle;

  // ----------------

  @override
  build(context) {
    return VisibilityArea.of(
      enabled: this.match.any((match) => !this.enableFilter || (!this.enableBatch ? match.singleEnabled && match.singleFiltered : match.batchEnabled && match.batchFiltered)),
      child: FlexContainer.vertical([
        StyledListTile.standardCustom(
          padding: .fromLTRB(24, 0, 24, 0),
          leading: IconView.of(IconSetExtension.queryOutlined(this.configuration.icon)),
          content: StyledText.inherit(tooltip: true, this.configuration.name),
          trailing: FlexContainer.horizontal(mainStretch: false, [
            StyledBadge.standard(
              label: StyledText.inherit('${this.match.where((match) => !this.enableFilter || (!this.enableBatch ? match.singleEnabled && match.singleFiltered : match.batchEnabled && match.batchFiltered)).length}'),
            ),
            Gap.horizontal(6),
            IconView.of(!this.expanded ? IconSet.keyboard_arrow_down : IconSet.keyboard_arrow_left),
          ]),
          onPressed: (context) async {
            this.onToggle();
          },
        ),
        VisibilityArea.of(
          enabled: this.expanded,
          child: FlexContainer.vertical(this.configuration.item.mapIndexed((optionItemIndex, optionItem) => OptionItem(
            configuration: optionItem,
            match: this.match[optionItemIndex],
            enableFilter: this.enableFilter,
            enableBatch: this.enableBatch,
            onSelect: this.onSelect,
          ))),
        ),
      ]),
    );
  }

}
