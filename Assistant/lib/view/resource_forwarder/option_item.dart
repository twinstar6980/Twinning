import '/common.dart';
import '/view/resource_forwarder/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/get.dart';

// ----------------

class OptionItemItem extends StatelessWidget {

  const OptionItemItem({
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
    var visible = (!this.enableFilter || (this.match.$3 && (!this.enableBatch ? this.match.$1 : this.match.$2)));
    var enabled = (!this.enableFilter || (this.match.$3)) && (!this.enableBatch ? this.match.$1 : this.match.$2);
    return Visibility(
      visible: visible,
      child: Column(
        children: [
          ListTile(
            enabled: enabled,
            dense: true,
            contentPadding: const EdgeInsets.fromLTRB(32, 0, 12, 0),
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
                    ),
                    onPressed: !enabled
                      ? null
                      : () async {
                      },
                    child: Stack(
                      children: [
                        Container(
                          height: 40,
                          padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
                          child: Row(
                            children: [
                              const SizedBox(width: 8),
                              Text(
                                '${this.configuration.preset.whereNotNull().length}',
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
                            style: const ButtonStyle(
                              overlayColor: WidgetStatePropertyAll(Colors.transparent),
                            ),
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
                                )
                              ),
                            ],
                            onSelected: (value) async {
                              this.onSelect(this.configuration.method, value.argument);
                            },
                          ),
                        ),
                      ],
                    ),
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

// ----------------

class OptionGroupItem extends StatefulWidget {

  const OptionGroupItem({
    super.key,
    required this.configuration,
    required this.match,
    required this.enableFilter,
    required this.enableBatch,
    required this.onSelect,
  });

  @override
  createState() => _OptionGroupItemState();

  // ----------------

  final OptionGroupConfiguration                     configuration;
  final List<(Boolean, Boolean, Boolean)>            match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onSelect;

}

class _OptionGroupItemState extends State<OptionGroupItem> {

  late Boolean _itemListVisible;

  // ----------------

  @override
  initState() {
    super.initState();
    this._itemListVisible = true;
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    var theme = Theme.of(context);
    var visible = !this.widget.enableFilter || this.widget.match.any((value) => value.$3 && (!this.widget.enableBatch ? value.$1 : value.$2));
    return Visibility(
      visible: visible,
      child: Column(
        children: [
          ListTile(
            contentPadding: const EdgeInsets.fromLTRB(16, 0, 20, 0),
            leading: Icon(SymbolsGet.get(this.widget.configuration.icon, SymbolStyle.outlined)),
            title: Text(
              this.widget.configuration.name,
              overflow: TextOverflow.ellipsis,
            ),
            trailing: Row(
              mainAxisSize: MainAxisSize.min,
              children: [
                Text(
                  '${this.widget.configuration.item.length}',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyMedium,
                ),
                const SizedBox(width: 6),
                Icon(!this._itemListVisible ? IconSymbols.keyboard_arrow_down : IconSymbols.keyboard_arrow_left),
              ],
            ),
            onTap: () async {
              this._itemListVisible = !this._itemListVisible;
              this.setState(() {});
            },
          ),
          Visibility(
            visible: this._itemListVisible,
            child: Column(
              children: this.widget.configuration.item.mapIndexed((optionItemIndex, optionItem) => OptionItemItem(
                configuration: optionItem,
                match: this.widget.match[optionItemIndex],
                enableFilter: this.widget.enableFilter,
                enableBatch: this.widget.enableBatch,
                onSelect: this.widget.onSelect,
              )).toList(),
            ),
          ),
        ],
      ),
    );
  }

}
