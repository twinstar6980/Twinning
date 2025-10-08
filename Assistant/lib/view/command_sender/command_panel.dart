import '/common.dart';
import '/utility/wrapper.dart';
import '/utility/control_helper.dart';
import '/view/command_sender/configuration.dart';
import '/view/command_sender/value_expression.dart';
import '/view/command_sender/argument_bar.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';

// ----------------

class CommandPanel extends StatelessWidget {

  const CommandPanel({
    super.key,
    required this.groupConfiguration,
    required this.itemConfiguration,
    required this.enableBatch,
    required this.argumentValue,
    required this.expanded,
    required this.onRemove,
    required this.onForward,
  });

  // ----------------

  final MethodGroupConfiguration        groupConfiguration;
  final MethodConfiguration             itemConfiguration;
  final Wrapper<Boolean>                enableBatch;
  final List<Wrapper<ValueExpression?>> argumentValue;
  final Wrapper<Boolean>                expanded;
  final Void Function()                 onRemove;
  final Void Function()                 onForward;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => Card.outlined(
        margin: EdgeInsets.fromLTRB(0, 0, 0, 0),
        child: Container(
          padding: EdgeInsets.fromLTRB(16, 8, 16, 16),
          child: Column(
            children: [
              Row(
                children: [
                  SizedBox(width: 8),
                  Text(
                    '${this.groupConfiguration.name} - ${this.itemConfiguration.name}',
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.titleMedium!,
                  ).withTooltip(
                    message: '${this.groupConfiguration.name} - ${this.itemConfiguration.name}',
                  ).withExpanded(),
                  SizedBox(width: 8),
                  IconButton(
                    tooltip: !this.expanded.value ? 'Expand' : 'Collapse',
                    isSelected: false,
                    icon: Icon(!this.expanded.value ? IconSymbols.keyboard_arrow_down : IconSymbols.keyboard_arrow_up),
                    onPressed: () async {
                      this.expanded.value = !this.expanded.value;
                      await refreshState(setState);
                    },
                  ),
                  SizedBox(width: 4),
                  IconButton(
                    tooltip: 'Remove',
                    isSelected: false,
                    icon: Icon(IconSymbols.remove),
                    onPressed: () async {
                      if (this.argumentValue.every((value) => value.value == null) || await ControlHelper.showDialogForConfirm(context)) {
                        this.onRemove();
                      }
                    },
                  ),
                  SizedBox(width: 4),
                ],
              ),
              Divider(),
              if (!this.expanded.value)
                ...this.itemConfiguration.argument.mapIndexed((argumentIndex, argumentConfiguration) => this.argumentValue[argumentIndex].value == null
                  ? SizedBox()
                  : Container(
                    margin: EdgeInsets.fromLTRB(8, 8, 8, 8),
                    child: Row(
                      crossAxisAlignment: CrossAxisAlignment.baseline,
                      textBaseline: theme.textTheme.labelLarge!.textBaseline,
                      children: [
                        Text(
                          argumentConfiguration.name,
                          overflow: TextOverflow.clip,
                          textAlign: TextAlign.start,
                          style: theme.textTheme.labelLarge!.copyWith(
                            color: theme.colorScheme.primary,
                          ),
                        ).withExpanded(),
                        SizedBox(width: 8),
                        Text(
                          ValueExpressionHelper.makeString(this.argumentValue[argumentIndex].value!),
                          overflow: TextOverflow.clip,
                          textAlign: TextAlign.end,
                          style: theme.textTheme.bodyMedium!,
                        ).withSelectionArea(
                        ).withExpanded(),
                      ],
                    ),
                  ),
                ),
              if (!this.expanded.value && this.argumentValue.where((value) => value.value != null).isEmpty)
                SizedBox(height: 16),
              if (!this.expanded.value)
                Divider(),
              if (this.expanded.value)
                ...this.itemConfiguration.argument.mapIndexed((argumentIndex, argumentConfiguration) => Container(
                  margin: EdgeInsets.fromLTRB(0, 8, 0, 8),
                  child: ArgumentBar(
                    name: argumentConfiguration.name,
                    type: argumentConfiguration.type,
                    option: argumentConfiguration.option?.map((value) => ConfigurationHelper.parseArgumentValueJson(argumentConfiguration.type, value)).toList(),
                    value: this.argumentValue[argumentIndex],
                  ),
                )),
              SizedBox(height: 8),
              Row(
                children: [
                  SizedBox().withExpanded(),
                  SizedBox(width: 8),
                  IconButton.filledTonal(
                    style: ButtonStyle(
                      padding: WidgetStatePropertyAll(EdgeInsets.zero),
                      overlayColor: WidgetStatePropertyAll(Colors.transparent),
                    ),
                    tooltip: 'Apply Preset',
                    isSelected: false,
                    icon: Stack(
                      children: [
                        Container(
                          padding: EdgeInsets.fromLTRB(8, 0, 8, 0),
                          height: 40,
                          child: Row(
                            children: [
                              SizedBox(width: 8),
                              Text(
                                '${this.itemConfiguration.preset.nonNulls.length}',
                                overflow: TextOverflow.ellipsis,
                              ),
                              SizedBox(width: 4),
                              Icon(IconSymbols.flash_on),
                            ],
                          ),
                        ),
                        Positioned.fill(
                          child: PopupMenuButton<PresetConfiguration>(
                            tooltip: '',
                            position: PopupMenuPosition.under,
                            offset: Offset(0, 12),
                            icon: SizedBox(),
                            itemBuilder: (context) => [
                              if (this.itemConfiguration.preset.isEmpty)
                                PopupMenuItem(
                                  height: 16,
                                  enabled: false,
                                  child: null,
                                ),
                              ...this.itemConfiguration.preset.map((preset) => preset == null
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
                              for (var argument in value.argument.entries) {
                                var argumentIndex = this.itemConfiguration.argument.indexWhere((value) => value.id == argument.key);
                                assertTest(argumentIndex != -1);
                                this.argumentValue[argumentIndex] = Wrapper(ConfigurationHelper.parseArgumentValueJson(this.itemConfiguration.argument[argumentIndex].type, argument.value));
                              }
                              await refreshState(setState);
                            },
                          ),
                        ),
                      ],
                    ),
                    onPressed: () async {
                    },
                  ),
                  SizedBox(width: 8),
                  IconButton.filledTonal(
                    tooltip: this.itemConfiguration.batchable == null ? '' : 'Enable Batch',
                    isSelected: this.enableBatch.value,
                    icon: Icon(IconSymbols.layers),
                    selectedIcon: Icon(IconSymbols.layers, fill: 1),
                    onPressed: this.itemConfiguration.batchable == null
                      ? null
                      : () async {
                        this.enableBatch.value = !this.enableBatch.value;
                        await refreshState(setState);
                      },
                  ),
                  SizedBox(width: 8),
                  FilledButton.icon(
                    label: Text(
                      'Forward',
                      overflow: TextOverflow.ellipsis,
                    ),
                    icon: Icon(IconSymbols.send, fill: 1),
                    onPressed: this.onForward,
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

}
