import '/common.dart';
import '/view/resource_forwarder/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/get.dart';

// ----------------

class OptionItemItem extends StatefulWidget {

  const OptionItemItem({
    super.key,
    required this.configuration,
    required this.match,
    required this.enableFilter,
    required this.enableBatch,
    required this.onForward,
  });

  @override
  createState() => _OptionItemItemState();

  // ----------------

  final OptionConfiguration                          configuration;
  final (Boolean, Boolean, Boolean)                  match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onForward;

}

class _OptionItemItemState extends State<OptionItemItem> {

  // ----------------

  @override
  initState() {
    super.initState();
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) { 
    var visible = (!this.widget.enableFilter || (this.widget.match.$3 && (!this.widget.enableBatch ? this.widget.match.$1 : this.widget.match.$2)));
    var enabled = (!this.widget.enableFilter || (this.widget.match.$3)) && (!this.widget.enableBatch ? this.widget.match.$1 : this.widget.match.$2);
    return Visibility(
      visible: visible,
      child: Column(
        children: [
          ListTile(
            enabled: enabled,
            dense: true,
            contentPadding: const EdgeInsets.fromLTRB(32, 0, 8, 0),
            leading: Icon(SymbolsGet.get(this.widget.configuration.icon, SymbolStyle.outlined)),
            title: Text(
              this.widget.configuration.name,
              overflow: TextOverflow.ellipsis,
            ),
            trailing: SizedBox(
              width: 76,
              height: 32,
              child: Tooltip(
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
                      SizedBox.expand(
                        child: Row(
                          children: [
                            const SizedBox(width: 12),
                            const SizedBox(width: 4),
                            Expanded(
                              child: Text(
                                this.widget.configuration.preset.whereNotNull().length.toString(),
                                overflow: TextOverflow.ellipsis,
                                textAlign: TextAlign.end,
                              ),
                            ),
                            const SizedBox(width: 4),
                            const Icon(IconSymbols.flash_on),
                            const SizedBox(width: 12),
                          ],
                        ),
                      ),
                      SizedBox.expand(
                        child: PopupMenuButton<OptionPresetConfiguration>(
                          tooltip: '',
                          enabled: enabled,
                          style: const ButtonStyle(
                            overlayColor: WidgetStatePropertyAll(Colors.transparent),
                          ),
                          position: PopupMenuPosition.under,
                          offset: const Offset(0, 12),
                          icon: const SizedBox(),
                          itemBuilder: (context) => [
                            if (this.widget.configuration.preset.isEmpty)
                              const PopupMenuItem(
                                height: 16,
                                enabled: false,
                                child: null,
                              ),
                            ...this.widget.configuration.preset.map((preset) => preset == null
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
                            this.widget.onForward(this.widget.configuration.method, value.argument);
                          },
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ),
            onTap: () async {
              this.widget.onForward(this.widget.configuration.method, null);
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
    required this.onForward,
  });

  @override
  createState() => _OptionGroupItemState();

  // ----------------

  final OptionGroupConfiguration                     configuration;
  final List<(Boolean, Boolean, Boolean)>            match;
  final Boolean                                      enableFilter;
  final Boolean                                      enableBatch;
  final Void Function(String?, Map<String, Object>?) onForward;

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
    var visible = !this.widget.enableFilter || this.widget.match.any((value) => (value.$3 && (!this.widget.enableBatch ? value.$1 : value.$2)));
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
            trailing: Icon(!this._itemListVisible ? IconSymbols.keyboard_arrow_down : IconSymbols.keyboard_arrow_left),
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
                onForward: this.widget.onForward,
              )).toList(),
            ),
          ),
        ],
      ),
    );
  }

}
