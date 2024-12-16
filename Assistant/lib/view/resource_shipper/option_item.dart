import '/common.dart';
import '/view/resource_shipper/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/get.dart';

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
  final (Boolean, Boolean, Boolean)                  match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onSelect;

  // ----------------

  @override
  build(context) { 
    var visible = !this.enableFilter || (this.enableBatch ? this.match.$2 : this.match.$1 && this.match.$3);
    var enabled = (this.enableBatch ? this.match.$2 : this.match.$1 && (!this.enableFilter || this.match.$3));
    return Visibility(
      visible: visible,
      child: Column(
        children: [
          ListTile(
            contentPadding: const EdgeInsets.fromLTRB(40, 0, 16, 0),
            dense: true,
            enabled: enabled,
            leading: Icon(SymbolsGet.get(this.configuration.icon, SymbolStyle.outlined)),
            title: Text(
              this.configuration.name,
              overflow: TextOverflow.ellipsis,
            ),
            trailing: Row(
              mainAxisSize: MainAxisSize.min,
              children: [
                Tooltip(
                  message: !enabled ? '' : 'Preset',
                  child: TextButton(
                    style: const ButtonStyle(
                      padding: WidgetStatePropertyAll(EdgeInsets.zero),
                      overlayColor: WidgetStatePropertyAll(Colors.transparent),
                    ),
                    child: Stack(
                      children: [
                        Container(
                          padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
                          height: 40,
                          child: Row(
                            children: [
                              const SizedBox(width: 8),
                              Text(
                                '${this.configuration.preset.nonNulls.length}',
                                overflow: TextOverflow.ellipsis,
                              ),
                              const SizedBox(width: 4),
                              const Icon(IconSymbols.flash_on),
                            ],
                          ),
                        ),
                        Positioned.fill(
                          child: PopupMenuButton<PresetConfiguration>(
                            tooltip: '',
                            enabled: enabled,
                            position: PopupMenuPosition.under,
                            offset: const Offset(0, 12),
                            icon: const SizedBox(),
                            itemBuilder: (context) => [
                              if (this.configuration.preset.isEmpty)
                                const PopupMenuItem(
                                  height: 16,
                                  enabled: false,
                                  child: null,
                                ),
                              ...this.configuration.preset.map((preset) => preset == null
                                ? const PopupMenuDivider()
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
                  ),
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
    required this.collapse,
    required this.onToggle,
  });

  // ----------------

  final OptionGroupConfiguration                     configuration;
  final List<(Boolean, Boolean, Boolean)>            match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onSelect;
  final Boolean                                      collapse;
  final Void Function()                              onToggle;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    var visible = !this.enableFilter || this.match.any((value) => this.enableBatch ? value.$2 : value.$1 && value.$3);
    return Visibility(
      visible: visible,
      child: Column(
        children: [
          ListTile(
            contentPadding: const EdgeInsets.fromLTRB(24, 0, 24, 0),
            leading: Icon(SymbolsGet.get(this.configuration.icon, SymbolStyle.outlined)),
            title: Text(
              this.configuration.name,
              overflow: TextOverflow.ellipsis,
            ),
            trailing: Row(
              mainAxisSize: MainAxisSize.min,
              children: [
                Text(
                  '${this.configuration.item.length}',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyMedium,
                ),
                const SizedBox(width: 6),
                Icon(!this.collapse ? IconSymbols.keyboard_arrow_left : IconSymbols.keyboard_arrow_down),
              ],
            ),
            onTap: this.onToggle,
          ),
          Visibility(
            visible: !this.collapse,
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
