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
    required this.collapse,
    required this.onRemove,
    required this.onForward,
  });

  // ----------------

  final MethodGroupConfiguration        groupConfiguration;
  final MethodConfiguration             itemConfiguration;
  final Wrapper<Boolean>                enableBatch;
  final List<Wrapper<ValueExpression?>> argumentValue;
  final Wrapper<Boolean>                collapse;
  final Void Function()                 onRemove;
  final Void Function()                 onForward;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => Card.outlined(
        margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
        child: Container(
          padding: const EdgeInsets.fromLTRB(16, 8, 16, 16),
          child: Column(
            children: [
              Row(
                children: [
                  const SizedBox(width: 8),
                  Expanded(
                    child: Text(
                      '${this.groupConfiguration.name} - ${this.itemConfiguration.name}',
                      overflow: TextOverflow.ellipsis,
                      style: theme.textTheme.titleMedium,
                    ),
                  ),
                  const SizedBox(width: 8),
                  IconButton(
                    tooltip: !this.collapse.value ? 'Collapse' : 'Expand',
                    isSelected: false,
                    icon: Icon(!this.collapse.value ? IconSymbols.keyboard_arrow_up : IconSymbols.keyboard_arrow_down),
                    onPressed: () async {
                      this.collapse.value = !this.collapse.value;
                      setState(() {});
                    },
                  ),
                  const SizedBox(width: 4),
                  IconButton(
                    tooltip: 'Remove',
                    isSelected: false,
                    icon: const Icon(IconSymbols.remove),
                    onPressed: () async {
                      if (this.argumentValue.where((value) => value.value != null).isEmpty || await ControlHelper.showDialogForConfirm(context)) {
                        this.onRemove();
                      }
                    },
                  ),
                  const SizedBox(width: 4),
                ],
              ),
              const Divider(),
              if (!this.collapse.value)
                ...this.itemConfiguration.argument.mapIndexed((argumentIndex, argumentConfiguration) => Container(
                  margin: const EdgeInsets.fromLTRB(0, 8, 0, 8),
                  child: ArgumentBar(
                    name: argumentConfiguration.name,
                    type: argumentConfiguration.type,
                    option: argumentConfiguration.option?.map((value) => ConfigurationHelper.parseArgumentValueJson(argumentConfiguration.type, value)).toList(),
                    value: this.argumentValue[argumentIndex],
                  ),
                )),
              if (this.collapse.value)
                ...this.itemConfiguration.argument.mapIndexed((argumentIndex, argumentConfiguration) => this.argumentValue[argumentIndex].value == null
                  ? const SizedBox()
                  : Container(
                    margin: const EdgeInsets.fromLTRB(8, 8, 8, 8),
                    child: Row(
                      crossAxisAlignment: CrossAxisAlignment.baseline,
                      textBaseline: theme.textTheme.labelLarge?.textBaseline,
                      children: [
                        Expanded(
                          child: Text(
                            argumentConfiguration.name,
                            overflow: TextOverflow.clip,
                            textAlign: TextAlign.start,
                            style: theme.textTheme.labelLarge?.copyWith(
                              color: theme.colorScheme.primary,
                            ),
                          ),
                        ),
                        const SizedBox(width: 8),
                        Expanded(
                          child: SelectionArea(
                            child: Text(
                              ValueExpressionHelper.makeString(this.argumentValue[argumentIndex].value!),
                              overflow: TextOverflow.clip,
                              textAlign: TextAlign.end,
                              style: theme.textTheme.bodyMedium,
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ),
              if (this.collapse.value && this.argumentValue.where((value) => value.value != null).isEmpty)
                const SizedBox(height: 16),
              if (this.collapse.value)
                const Divider(),
              const SizedBox(height: 8),
              Row(
                children: [
                  const Expanded(
                    child: SizedBox(),
                  ),
                  const SizedBox(width: 8),
                  IconButton.filledTonal(
                    style: const ButtonStyle(
                      padding: WidgetStatePropertyAll(EdgeInsets.zero),
                      overlayColor: WidgetStatePropertyAll(Colors.transparent),
                    ),
                    tooltip: 'Apply Preset',
                    isSelected: false,
                    icon: Stack(
                      children: [
                        Container(
                          padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
                          height: 40,
                          child: Row(
                            children: [
                              const SizedBox(width: 8),
                              Text(
                                '${this.itemConfiguration.preset.nonNulls.length}',
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
                            position: PopupMenuPosition.under,
                            offset: const Offset(0, 12),
                            icon: const SizedBox(),
                            itemBuilder: (context) => [
                              if (this.itemConfiguration.preset.isEmpty)
                                const PopupMenuItem(
                                  height: 16,
                                  enabled: false,
                                  child: null,
                                ),
                              ...this.itemConfiguration.preset.map((preset) => preset == null
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
                              for (var argument in value.argument.entries) {
                                var argumentIndex = this.itemConfiguration.argument.indexWhere((value) => value.id == argument.key);
                                assertTest(argumentIndex != -1);
                                this.argumentValue[argumentIndex] = Wrapper(ConfigurationHelper.parseArgumentValueJson(this.itemConfiguration.argument[argumentIndex].type, argument.value));
                              }
                              setState(() {});
                            },
                          ),
                        ),
                      ],
                    ),
                    onPressed: () async {
                    },
                  ),
                  const SizedBox(width: 8),
                  IconButton.filledTonal(
                    tooltip: 'Enable Batch',
                    isSelected: this.enableBatch.value,
                    icon: const Icon(IconSymbols.layers),
                    selectedIcon: const Icon(IconSymbols.layers, fill: 1),
                    onPressed: this.itemConfiguration.batchable == null
                      ? null
                      : () async {
                        this.enableBatch.value = !this.enableBatch.value;
                        setState(() {});
                      },
                  ),
                  const SizedBox(width: 8),
                  FilledButton.tonalIcon(
                    label: const Text(
                      'Forward',
                      overflow: TextOverflow.ellipsis,
                    ),
                    icon: const Icon(IconSymbols.send, fill: 1),
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
