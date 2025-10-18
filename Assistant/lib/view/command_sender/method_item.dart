import '/common.dart';
import '/utility/icon_helper.dart';
import '/view/command_sender/configuration.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';

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
          contentPadding: EdgeInsets.fromLTRB(40, 0, 24, 0),
          dense: true,
          leading: Icon(IconHelper.queryOutlined(this.configuration.icon)),
          title: Text(
            this.configuration.name,
            overflow: TextOverflow.ellipsis,
          ).withTooltip(
            message: this.configuration.name,
          ),
          trailing: Visibility(
            visible: this.configuration.batch != null,
            child: Icon(IconSymbols.layers, color: theme.colorScheme.primary).withTooltip(message: 'Batch'),
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
    required this.expanded,
    required this.onToggle,
  });

  // ----------------

  final MethodGroupConfiguration configuration;
  final Void Function(String)    onSelect;
  final Boolean                  expanded;
  final Void Function()          onToggle;

  // ----------------

  @override
  build(context) {
    return Column(
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
                count: this.configuration.item.length,
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
