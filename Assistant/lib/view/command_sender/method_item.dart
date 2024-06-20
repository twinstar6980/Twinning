import '/common.dart';
import '/view/command_sender/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/get.dart';

// ----------------

class MethodItemItem extends StatelessWidget {

  const MethodItemItem({
    super.key,
    required this.configuration,
    required this.onSelect,
  });

  // ----------------

  final MethodConfiguration   configuration;
  final Void Function(String) onSelect;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Column(
      children: [
        ListTile(
          dense: true,
          contentPadding: const EdgeInsets.fromLTRB(32, 0, 20, 0),
          leading: Icon(SymbolsGet.get(this.configuration.icon, SymbolStyle.outlined)),
          title: Text(
            this.configuration.name,
            overflow: TextOverflow.ellipsis,
          ),
          trailing: Visibility(
            visible: this.configuration.batchable != null,
            child: Tooltip(
              message: 'Batchable',
              child: Icon(IconSymbols.layers, color: theme.colorScheme.primary),
            ),
          ),
          onTap: () async {
            this.onSelect(this.configuration.id);
          },
        ),
      ],
    );
  }

}

// ----------------

class MethodGroupItem extends StatefulWidget {

  const MethodGroupItem({
    super.key,
    required this.configuration,
    required this.onSelect,
  });

  @override
  createState() => _MethodGroupItemState();

  // ----------------

  final MethodGroupConfiguration configuration;
  final Void Function(String)    onSelect;

}

class _MethodGroupItemState extends State<MethodGroupItem> {

  late Boolean _itemListVisible;

  // ----------------

  @override
  initState() {
    super.initState();
    this._itemListVisible = false;
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
    return Column(
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
            children: this.widget.configuration.item.mapIndexed((optionItemIndex, optionItem) => MethodItemItem(
              configuration: optionItem,
              onSelect: this.widget.onSelect,
            )).toList(),
          ),
        ),
      ],
    );
  }

}
