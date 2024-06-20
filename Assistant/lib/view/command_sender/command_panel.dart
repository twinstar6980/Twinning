import '/common.dart';
import '/utility/wrapper.dart';
import '/view/command_sender/configuration.dart';
import '/view/command_sender/value_expression.dart';
import '/view/command_sender/argument_bar.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';

// ----------------

class CommandPanel extends StatefulWidget {

  const CommandPanel({
    super.key,
    required this.groupConfiguration,
    required this.itemConfiguration,
    required this.enableBatch,
    required this.argumentValue,
    required this.onRemove,
    required this.onForward,
  });

  @override
  createState() => _CommandPanelState();

  // ----------------

  final MethodGroupConfiguration        groupConfiguration;
  final MethodConfiguration             itemConfiguration;
  final Wrapper<Boolean>                enableBatch;
  final List<Wrapper<ValueExpression?>> argumentValue;
  final Void Function()                 onRemove;
  final Void Function()                 onForward;

}

class _CommandPanelState extends State<CommandPanel> {

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
    var theme = Theme.of(context);
    return Card(
      margin: const EdgeInsets.fromLTRB(1, 0, 1, 0),
      child: Container(
        padding: const EdgeInsets.fromLTRB(16, 8, 16, 12),
        child: Column(
          children: [
            Row(
              children: [
                Expanded(
                  child: Text(
                    '${this.widget.groupConfiguration.name} - ${this.widget.itemConfiguration.name}',
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.titleMedium,
                  ),
                ),
                const SizedBox(width: 8),
                IconButton.filledTonal(
                  tooltip: 'Remove',
                  isSelected: false,
                  icon: const Icon(IconSymbols.remove),
                  onPressed: this.widget.onRemove,
                ),
              ],
            ),
            ...this.widget.itemConfiguration.argument.mapIndexed((argumentIndex, argumentConfiguration) => Container(
              margin: const EdgeInsets.fromLTRB(0, 8, 0, 4),
              child: ArgumentBar(
                key: ObjectKey(this.widget.argumentValue[argumentIndex]),
                name: argumentConfiguration.name,
                type: argumentConfiguration.type,
                option: argumentConfiguration.option?.map((value) => (ConfigurationHelper.parseArgumentValueJson(argumentConfiguration.type, value))).toList(),
                value: this.widget.argumentValue[argumentIndex],
              ),
            )),
            const SizedBox(height: 12),
            Row(
              children: [
                const Expanded(
                  child: SizedBox.shrink(),
                ),
                const SizedBox(width: 8),
                IconButton.filledTonal(
                  tooltip: 'Apply Preset',
                  style: const ButtonStyle(
                    padding: WidgetStatePropertyAll(EdgeInsets.zero),
                  ),
                  isSelected: false,
                  onPressed: () async {
                  },
                  icon: Stack(
                    children: [
                      Container(
                        height: 40,
                        padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
                        child: Row(
                          children: [
                            const SizedBox(width: 8),
                            Text(
                              '${this.widget.itemConfiguration.preset.whereNotNull().length}',
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
                          style: const ButtonStyle(
                            overlayColor: WidgetStatePropertyAll(Colors.transparent),
                          ),
                          position: PopupMenuPosition.under,
                          offset: const Offset(0, 12),
                          icon: const SizedBox(),
                          itemBuilder: (context) => [
                            if (this.widget.itemConfiguration.preset.isEmpty)
                              const PopupMenuItem(
                                height: 16,
                                enabled: false,
                                child: null,
                              ),
                            ...this.widget.itemConfiguration.preset.map((preset) => preset == null
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
                            for (var argument in value.argument.entries) {
                              var argumentIndex = this.widget.itemConfiguration.argument.indexWhere((value) => (value.id == argument.key));
                              assertTest(argumentIndex != -1);
                              this.widget.argumentValue[argumentIndex] = Wrapper(ConfigurationHelper.parseArgumentValueJson(this.widget.itemConfiguration.argument[argumentIndex].type, argument.value));
                            }
                            this.setState(() {});
                          },
                        ),
                      ),
                    ],
                  ),
                ),
                const SizedBox(width: 8),
                IconButton.filledTonal(
                  tooltip: 'Enable Batch',
                  isSelected: this.widget.enableBatch.value,
                  icon: const Icon(IconSymbols.layers),
                  selectedIcon: const Icon(IconSymbols.layers, fill: 1),
                  onPressed: this.widget.itemConfiguration.batchable == null
                    ? null
                    : () async {
                      this.widget.enableBatch.value = !this.widget.enableBatch.value;
                      this.setState(() {});
                    },
                ),
                const SizedBox(width: 8),
                FilledButton.tonalIcon(
                  label: const Text(
                    'Forward',
                    overflow: TextOverflow.clip,
                  ),
                  icon: const Icon(IconSymbols.send),
                  onPressed: this.widget.onForward,
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }

}
