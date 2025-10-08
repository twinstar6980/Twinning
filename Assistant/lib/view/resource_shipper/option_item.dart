import '/common.dart';
import '/utility/icon_helper.dart';
import '/view/resource_shipper/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';

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

  final OptionConfiguration                          configuration;
  final (Boolean, Boolean, Boolean, Boolean)         match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onSelect;

  // ----------------

  @override
  build(context) {
    var enabled = !this.enableBatch ? this.match.$1 : this.match.$3;
    return Visibility(
      visible: !this.enableFilter || (!this.enableBatch ? this.match.$1 && this.match.$2 : this.match.$3 && this.match.$4),
      child: Column(
        children: [
          ListTile(
            contentPadding: EdgeInsets.fromLTRB(40, 0, 16, 0),
            dense: true,
            enabled: enabled,
            leading: Icon(IconHelper.queryOutlined(this.configuration.icon)),
            title: Text(
              this.configuration.name,
              overflow: TextOverflow.ellipsis,
            ).withTooltip(
              message: !enabled ? '' : this.configuration.name,
            ),
            trailing: Row(
              mainAxisSize: MainAxisSize.min,
              children: [
                TextButton(
                  style: ButtonStyle(
                    padding: WidgetStatePropertyAll(EdgeInsets.zero),
                    overlayColor: WidgetStatePropertyAll(Colors.transparent),
                  ),
                  child: Stack(
                    children: [
                      Container(
                        padding: EdgeInsets.fromLTRB(8, 0, 8, 0),
                        height: 40,
                        child: Row(
                          children: [
                            SizedBox(width: 8),
                            Text(
                              '${this.configuration.preset.nonNulls.length}',
                              overflow: TextOverflow.ellipsis,
                            ),
                            SizedBox(width: 6),
                            Icon(IconSymbols.flash_on),
                          ],
                        ),
                      ),
                      Positioned.fill(
                        child: PopupMenuButton<PresetConfiguration>(
                          tooltip: '',
                          enabled: enabled,
                          position: PopupMenuPosition.under,
                          offset: Offset(0, 12),
                          icon: SizedBox(),
                          itemBuilder: (context) => [
                            if (this.configuration.preset.isEmpty)
                              PopupMenuItem(
                                height: 16,
                                enabled: false,
                                child: null,
                              ),
                            ...this.configuration.preset.map((preset) => preset == null
                              ? PopupMenuDivider()
                              : PopupMenuItem(
                                value: preset,
                                child: Text(
                                  preset.name,
                                  overflow: TextOverflow.ellipsis,
                                ),
                              )),
                          ],
                          onSelected: (value) async {
                            this.onSelect(this.configuration.method, value.argument);
                          },
                        ),
                      ),
                    ],
                  ),
                  onPressed: !enabled
                    ? null
                    : () async {
                    },
                ).withTooltip(
                  message: !enabled ? '' : 'Preset',
                ),
              ],
            ),
            onTap: () async {
              this.onSelect(this.configuration.method, null);
            },
          ),
        ],
      ),
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

  final OptionGroupConfiguration                     configuration;
  final List<(Boolean, Boolean, Boolean, Boolean)>   match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onSelect;
  final Boolean                                      expanded;
  final Void Function()                              onToggle;

  // ----------------

  @override
  build(context) {
    return Visibility(
      visible: this.match.any((match) => !this.enableFilter || (!this.enableBatch ? match.$1 && match.$2 : match.$3 && match.$4)),
      child: Column(
        children: [
          ListTile(
            contentPadding: EdgeInsets.fromLTRB(24, 0, 24, 0),
            leading: Icon(IconHelper.queryOutlined(this.configuration.icon)),
            title: Text(
              this.configuration.name,
              overflow: TextOverflow.ellipsis,
            ).withTooltip(
              message: this.configuration.name,
            ),
            trailing: Row(
              mainAxisSize: MainAxisSize.min,
              children: [
                Badge.count(
                  count: this.match.where((match) => !this.enableFilter || (!this.enableBatch ? match.$1 && match.$2 : match.$3 && match.$4)).length,
                ),
                SizedBox(width: 6),
                Icon(!this.expanded ? IconSymbols.keyboard_arrow_down : IconSymbols.keyboard_arrow_left),
              ],
            ),
            onTap: this.onToggle,
          ),
          Visibility(
            visible: this.expanded,
            child: Column(
              children: this.configuration.item.mapIndexed((optionItemIndex, optionItem) => OptionItem(
                configuration: optionItem,
                match: this.match[optionItemIndex],
                enableFilter: this.enableFilter,
                enableBatch: this.enableBatch,
                onSelect: this.onSelect,
              )).toList(),
            ),
          ),
        ],
      ),
    );
  }

}
