import '/common.dart';
import '/module.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/core_command_sender/configuration.dart';
import '/view/core_command_sender/value_expression.dart';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';

// ----------------

class _BasicArgumentBar extends StatelessWidget {

  const _BasicArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
    required this.content,
  });

  // ----------------

  final String           name;
  final ValueExpression? value;
  final Boolean          batch;
  final Boolean          expanded;
  final Widget           content;

  // ----------------

  @override
  build(context) {
    return FlexContainer.vertical([
      if (!this.expanded && this.value != null) ...[
        FlexContainer.horizontal(textBaseline: StyledTypography.labelLarge.query(context).textBaseline!, [
          Gap.horizontal(8),
          StyledText.custom(
            this.name,
            typography: .labelLarge,
            color: !this.batch ? .primary : .tertiary,
            overflow: .clip,
            align: .start,
          ).withFlexExpanded(),
          Gap.horizontal(8),
          StyledText.custom(
            ValueExpressionHelper.makeString(this.value!),
            typography: .bodyMedium,
            overflow: .clip,
            align: .end,
          ).withSelectableArea(
          ).withFlexExpanded(),
          Gap.horizontal(8),
        ]),
      ],
      if (this.expanded) ...[
        FlexContainer.horizontal([
          Gap.horizontal(8),
          StyledText.custom(
            this.name,
            tooltip: true,
            typography: .labelLarge,
            color: !this.batch ? .primary : .tertiary,
          ).withFlexExpanded(),
          Gap.horizontal(8),
        ]),
        this.content,
      ],
    ]);
  }

}

// ----------------

class _BooleanArgumentBar extends StatelessWidget {

  const _BooleanArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                      name;
  final Wrapper<BooleanExpression?> value;
  final Boolean                     batch;
  final Boolean                     expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInput.underlined(
          type: .text,
          format: null,
          hint: 'Boolean',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'No',
              selected: this.value.value == null ? false : this.value.value!.value == false,
              icon: IconView.of(IconSet.do_not_disturb_on),
              iconOnSelected: IconView.of(IconSet.do_not_disturb_on, fill: 1),
              onPressed: (context) async {
                this.value.value = this.value.value?.value == false ? null : .new(false);
                await refreshState(setState);
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Yes',
              selected: this.value.value == null ? false : this.value.value!.value == true,
              icon: IconView.of(IconSet.check_circle),
              iconOnSelected: IconView.of(IconSet.check_circle, fill: 1),
              onPressed: (context) async {
                this.value.value = this.value.value?.value == true ? null : .new(true);
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.value.value!.value),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              if (value == 'n' || value == 'y') {
                this.value.value = .new(value == 'y');
              }
            }
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

class _IntegerArgumentBar extends StatelessWidget {

  const _IntegerArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                      name;
  final Wrapper<IntegerExpression?> value;
  final Boolean                     batch;
  final Boolean                     expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInput.underlined(
          type: .numberWithOptions(signed: true, decimal: false),
          format: null,
          hint: 'Integer',
          prefix: null,
          suffix: null,
          value: this.value.value == null ? '' : ConvertHelper.makeIntegerToString(this.value.value!.value, false),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              var parsedValue = Integer.tryParse(value);
              if (parsedValue != null) {
                this.value.value = .new(parsedValue);
              }
            }
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

class _FloaterArgumentBar extends StatelessWidget {

  const _FloaterArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                      name;
  final Wrapper<FloaterExpression?> value;
  final Boolean                     batch;
  final Boolean                     expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInput.underlined(
          type: .numberWithOptions(signed: true, decimal: true),
          format: null,
          hint: 'Floater',
          prefix: null,
          suffix: null,
          value: this.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.value.value!.value, false),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              var parsedValue = Floater.tryParse(value);
              if (parsedValue != null && parsedValue.isFinite) {
                this.value.value = .new(parsedValue);
              }
            }
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

class _SizeArgumentBar extends StatelessWidget {

  const _SizeArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                   name;
  final Wrapper<SizeExpression?> value;
  final Boolean                  batch;
  final Boolean                  expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInput.underlined(
          type: .numberWithOptions(signed: false, decimal: true),
          format: null,
          hint: 'Size',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Exponent',
              icon: this.value.value == null
                ? IconView.of(IconSet.expand_circle_down)
                : BoxContainer.of(
                  constraints: .tightFor(width: 24, height: 24),
                  align: .center,
                  child: StyledText.inherit(['B', 'K', 'M', 'G'][this.value.value!.exponent]),
                ),
              onPressed: (context) async {
                var value = await StyledMenuExtension.show<Integer>(context, StyledMenu.standard(
                  position: .under,
                  content: ['B', 'K', 'M', 'G'].mapIndexed((index, value) => StyledMenuItem.standard(
                    value: index,
                    content: StyledText.inherit(value),
                  )),
                ));
                if (value != null) {
                  this.value.value = .new(this.value.value?.count ?? 1.0, value);
                  await refreshState(setState);
                }
              },
            ),
          ],
          value: this.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.value.value!.count, false),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              var parsedCount = Floater.tryParse(value);
              if (parsedCount != null && parsedCount.isFinite && parsedCount >= 0.0) {
                this.value.value = .new(parsedCount, this.value.value?.exponent ?? 2);
              }
            }
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

class _StringArgumentBar extends StatelessWidget {

  const _StringArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                     name;
  final Wrapper<StringExpression?> value;
  final Boolean                    batch;
  final Boolean                    expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInput.underlined(
          type: .text,
          format: null,
          hint: 'String',
          prefix: null,
          suffix: null,
          value: this.value.value == null ? '' : this.value.value!.value,
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = .new(value);
            }
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

class _PathArgumentBar extends StatelessWidget {

  const _PathArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                   name;
  final Wrapper<PathExpression?> value;
  final Boolean                  batch;
  final Boolean                  expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInput.underlined(
          type: .text,
          format: null,
          hint: 'Path',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Pick',
              icon: IconView.of(IconSet.open_in_new),
              onPressed: (context) async {
                var target = await StorageDropRegionExtension.pick(
                  context: context,
                  allowLoadFile: true,
                  allowLoadDirectory: true,
                  allowSaveFile: true,
                  location: '@${ModuleHelper.query(.core_command_sender).identifier}.generic',
                );
                if (target != null) {
                  this.value.value = .new(target);
                  await refreshState(setState);
                }
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.content,
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = .new(StorageHelper.regularize(value));
            }
            await refreshState(setState);
          },
        ).withStorageDropRegion(
          onDrop: (item) async {
            this.value.value = .new(item.first);
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

class _EnumerationArgumentBar extends StatelessWidget {

  const _EnumerationArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.option,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                    name;
  final List<ValueExpression>     option;
  final Wrapper<ValueExpression?> value;
  final Boolean                   batch;
  final Boolean                   expanded;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        value: this.value.value,
        batch: this.batch,
        expanded: this.expanded,
        content: StyledInputCombo.underlined(
          hint: 'Enumeration',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Reset',
              icon: IconView.of(IconSet.restart_alt),
              onPressed: (context) async {
                this.value.value = null;
                await refreshState(setState);
              },
            ),
          ],
          option: this.option.map((value) => (value, ValueExpressionHelper.makeString(value))).toList(),
          value: value.value,
          onChanged: (context, value) async {
            value as ValueExpression;
            this.value.value = value;
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

// ----------------

class ArgumentBar extends StatelessWidget {

  const ArgumentBar({
    super.key,
    required this.name,
    required this.type,
    required this.option,
    required this.value,
    required this.batch,
    required this.expanded,
  });

  // ----------------

  final String                    name;
  final ArgumentType              type;
  final List<ValueExpression>?    option;
  final Wrapper<ValueExpression?> value;
  final Boolean                   batch;
  final Boolean                   expanded;

  // ----------------

  @override
  build(context) {
    if (this.option == null) {
      return switch (this.type) {
        .boolean => _BooleanArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        .integer => _IntegerArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        .floater => _FloaterArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        .size => _SizeArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        .string => _StringArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        .path => _PathArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
      };
    }
    else {
      return _EnumerationArgumentBar(
        name: this.name,
        option: this.option!,
        value: this.value,
        batch: this.batch,
        expanded: this.expanded,
      );
    }
  }

}
