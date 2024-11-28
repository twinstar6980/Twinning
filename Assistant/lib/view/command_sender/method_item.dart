import '/common.dart';
import '/view/command_sender/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/get.dart';

// ----------------

class MethodItem extends StatelessWidget {

  const MethodItem({
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
          contentPadding: const EdgeInsets.fromLTRB(40, 0, 24, 0),
          dense: true,
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

class MethodGroupItem extends StatelessWidget {

  const MethodGroupItem({
    super.key,
    required this.configuration,
    required this.onSelect,
    required this.collapse,
    required this.onToggle,
  });

  // ----------------

  final MethodGroupConfiguration configuration;
  final Void Function(String)    onSelect;
  final Boolean                  collapse;
  final Void Function()          onToggle;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Column(
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
            children: this.configuration.item.mapIndexed((optionItemIndex, optionItem) => MethodItem(
              configuration: optionItem,
              onSelect: this.onSelect,
            )).toList(),
          ),
        ),
      ],
    );
  }

}
