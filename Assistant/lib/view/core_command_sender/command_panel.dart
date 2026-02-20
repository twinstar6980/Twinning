import '/common.dart';
import '/utility/wrapper.dart';
import '/widget/export.dart';
import '/view/core_command_sender/configuration.dart';
import '/view/core_command_sender/value_expression.dart';
import '/view/core_command_sender/argument_bar.dart';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';

// ----------------

class CommandPanel extends StatelessWidget {

  const CommandPanel({
    super.key,
    required this.groupConfiguration,
    required this.itemConfiguration,
    required this.batch,
    required this.argument,
    required this.expanded,
    required this.onRemove,
    required this.onForward,
  });

  // ----------------

  final MethodGroupConfiguration        groupConfiguration;
  final MethodConfiguration             itemConfiguration;
  final Wrapper<Boolean>                batch;
  final List<Wrapper<ValueExpression?>> argument;
  final Wrapper<Boolean>                expanded;
  final Void Function()                 onRemove;
  final Void Function()                 onForward;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => StyledCard.outlined(
        padding: .fromLTRB(16, 8, 16, 16),
        content: FlexContainer.vertical([
          FlexContainer.horizontal([
            Gap.horizontal(8),
            StyledText.custom(
              '${this.groupConfiguration.name} - ${this.itemConfiguration.name}',
              tooltip: true,
              typography: .titleMedium,
            ).withFlexExpanded(),
            Gap.horizontal(8),
            StyledIconButton.standard(
              tooltip: !this.expanded.value ? 'Expand' : 'Collapse',
              icon: IconView.of(!this.expanded.value ? IconSet.keyboard_arrow_down : IconSet.keyboard_arrow_up),
              onPressed: (context) async {
                this.expanded.value = !this.expanded.value;
                await refreshState(setState);
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Remove',
              icon: IconView.of(IconSet.remove),
              onPressed: (context) async {
                if (this.argument.every((value) => value.value == null) || await MoreModalDialogExtension.showForConfirm(context)) {
                  this.onRemove();
                }
              },
            ),
            Gap.horizontal(4),
          ]),
          StyledDivider.standard(),
          ...this.itemConfiguration.argument.mapIndexed((argumentIndex, argumentConfiguration) => BoxContainer.of(
            margin: !this.expanded.value && this.argument[argumentIndex].value == null ? .zero : .fromLTRB(0, 8, 0, 8),
            child: ArgumentBar(
              name: argumentConfiguration.name,
              type: argumentConfiguration.type,
              option: argumentConfiguration.option?.map((value) => ConfigurationHelper.parseArgumentValueJson(argumentConfiguration.type, value)).toList(),
              value: this.argument[argumentIndex],
              batch: this.batch.value && (this.itemConfiguration.batch?.contains(argumentConfiguration.identifier) ?? false),
              expanded: this.expanded.value,
            ),
          )),
          if (!this.expanded.value && this.argument.where((value) => value.value != null).isEmpty) ...[
            Gap.vertical(16),
          ],
          if (!this.expanded.value) ...[
            StyledDivider.standard(),
          ],
          Gap.vertical(8),
          FlexContainer.horizontal([
            Gap.expanded(),
            Gap.horizontal(8),
            StyledIconButton.filledTonal(
              tooltip: 'Preset',
              selected: false,
              icon: FlexContainer.horizontal([
                Gap.horizontal(8),
                StyledText.inherit('${this.itemConfiguration.preset.nonNulls.length}'),
                Gap.horizontal(4),
                IconView.of(IconSet.flash_on),
              ]),
              onPressed: (context) async {
                var preset = await StyledMenuExtension.show<PresetConfiguration>(context, StyledMenu.standard(
                  position: .under,
                  content: this.itemConfiguration.preset.map((preset) => preset == null ? null : StyledMenuItem.standard(
                    value: preset,
                    content: StyledText.inherit(preset.name),
                  )),
                ));
                if (preset != null) {
                  for (var argument in preset.argument.entries) {
                    var argumentIndex = this.itemConfiguration.argument.indexWhere((value) => value.identifier == argument.key);
                    assertTest(argumentIndex != -1);
                    this.argument[argumentIndex] = .new(ConfigurationHelper.parseArgumentValueJson(this.itemConfiguration.argument[argumentIndex].type, argument.value));
                  }
                  await refreshState(setState);
                }
              },
            ),
            Gap.horizontal(8),
            StyledIconButton.filledTonal(
              enabled: this.itemConfiguration.batch != null,
              tooltip: 'Batch',
              selected: this.batch.value,
              icon: IconView.of(IconSet.layers),
              iconOnSelected: IconView.of(IconSet.layers, fill: 1),
              onPressed: (context) async {
                this.batch.value = !this.batch.value;
                await refreshState(setState);
              },
            ),
            Gap.horizontal(8),
            StyledButton.filled(
              icon: IconView.of(IconSet.send, fill: 1),
              content: StyledText.inherit('Forward'),
              onPressed: (context) async {
                this.onForward();
              },
            ),
          ]),
        ]),
      ),
    );
  }

}
