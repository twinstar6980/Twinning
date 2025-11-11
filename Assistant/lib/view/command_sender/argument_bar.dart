import '/common.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/command_sender/configuration.dart';
import '/view/command_sender/value_expression.dart';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart' as material;

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
      if (!this.expanded && this.value != null)
        FlexContainer.horizontal(textBaseline: material.Theme.of(context).textTheme.labelLarge!.textBaseline!, [
          Gap.horizontal(8),
          StyledText.custom(
            this.name,
            variant: StyledTextVariant.labelLarge,
            color: !this.batch ? StyledColor.primary : StyledColor.tertiary,
            overflow: TextOverflow.clip,
            align: TextAlign.start,
          ).withFlexExpanded(),
          Gap.horizontal(8),
          StyledText.custom(
            ValueExpressionHelper.makeString(this.value!),
            variant: StyledTextVariant.bodyMedium,
            overflow: TextOverflow.clip,
            align: TextAlign.end,
          ).withSelectableArea(
          ).withFlexExpanded(),
          Gap.horizontal(8),
        ]),
      if (this.expanded)
        FlexContainer.horizontal([
          Gap.horizontal(8),
          StyledText.custom(
            this.name,
            variant: StyledTextVariant.labelLarge,
            tooltip: true,
            color: !this.batch ? StyledColor.primary : StyledColor.tertiary,
          ).withFlexExpanded(),
          Gap.horizontal(8),
        ]),
      if (this.expanded)
        this.content,
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
          type: StyledInputType.text,
          format: [],
          hint: 'Boolean',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'No',
              selected: this.value.value == null ? false : this.value.value!.value == false,
              icon: Icon(IconSet.do_not_disturb_on),
              iconOnSelected: Icon(IconSet.do_not_disturb_on, fill: 1),
              onPressed: (context) async {
                this.value.value = this.value.value?.value == false ? null : BooleanExpression(false);
                await refreshState(setState);
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Yes',
              selected: this.value.value == null ? false : this.value.value!.value == true,
              icon: Icon(IconSet.check_circle),
              iconOnSelected: Icon(IconSet.check_circle, fill: 1),
              onPressed: (context) async {
                this.value.value = this.value.value?.value == true ? null : BooleanExpression(true);
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.value.value!.value),
          onChanged: (context, text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              if (text == 'n' || text == 'y') {
                this.value.value = BooleanExpression(text == 'y');
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
          type: StyledInputType.numberWithOptions(signed: true, decimal: false),
          format: [],
          hint: 'Integer',
          prefix: null,
          suffix: null,
          value: this.value.value == null ? '' : ConvertHelper.makeIntegerToString(this.value.value!.value, false),
          onChanged: (context, text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              var value = Integer.tryParse(text);
              if (value != null) {
                this.value.value = IntegerExpression(value);
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
          type: StyledInputType.numberWithOptions(signed: true, decimal: true),
          format: [],
          hint: 'Floater',
          prefix: null,
          suffix: null,
          value: this.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.value.value!.value, false),
          onChanged: (context, text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              var value = Floater.tryParse(text);
              if (value != null && value.isFinite) {
                this.value.value = FloaterExpression(value);
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
          type: StyledInputType.numberWithOptions(signed: false, decimal: true),
          format: [],
          hint: 'Size',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Exponent',
              icon: this.value.value == null
                ? Icon(IconSet.expand_circle_down)
                : BoxContainer.of(
                  width: 24,
                  height: 24,
                  align: BoxContainerAlign.center,
                  child: StyledText.inherit(['B', 'K', 'M', 'G'][this.value.value!.exponent]),
                ),
              onPressed: (context) async {
                var value = await StyledMenuExtension.show<Integer>(context, StyledMenu.standard(
                  position: StyledMenuPosition.under,
                  children: ['B', 'K', 'M', 'G'].mapIndexed((index, value) => StyledMenuItem.standard(
                    value: index,
                    content: StyledText.inherit(value),
                  )),
                ));
                if (value != null) {
                  this.value.value = SizeExpression(this.value.value?.count ?? 1.0, value);
                  await refreshState(setState);
                }
              },
            ),
          ],
          value: this.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.value.value!.count, false),
          onChanged: (context, text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              var count = Floater.tryParse(text);
              if (count != null && count.isFinite && count >= 0.0) {
                this.value.value = SizeExpression(count, this.value.value?.exponent ?? 2);
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
          type: StyledInputType.text,
          format: [],
          hint: 'String',
          prefix: null,
          suffix: null,
          value: this.value.value == null ? '' : this.value.value!.value,
          onChanged: (context, text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = StringExpression(text);
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
        content: StorageDropRegion(
          onDrop: (item) async {
            this.value.value = PathExpression(item.first);
            await refreshState(setState);
          },
          child: StyledInput.underlined(
            type: StyledInputType.text,
            format: [],
            hint: 'Path',
            prefix: null,
            suffix: [
              StyledIconButton.standard(
                tooltip: 'Pick',
                icon: Icon(IconSet.open_in_new),
                onPressed: (context) async {
                  var target = await pickStorageItem(
                    context: context,
                    allowLoadFile: true,
                    allowLoadDirectory: true,
                    allowSaveFile: true,
                    location: '@CommandSender.Generic',
                  );
                  if (target != null) {
                    this.value.value = PathExpression(target);
                    await refreshState(setState);
                  }
                },
              ),
            ],
            value: this.value.value == null ? '' : this.value.value!.content,
            onChanged: (context, text) async {
              if (text.isEmpty) {
                this.value.value = null;
              }
              else {
                this.value.value = PathExpression(StorageHelper.regularize(text));
              }
              await refreshState(setState);
            },
          ),
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
              icon: Icon(IconSet.restart_alt),
              onPressed: (context) async {
                this.value.value = null;
                await refreshState(setState);
              },
            ),
          ],
          option: this.option.map((value) => (value, ValueExpressionHelper.makeString(value))).toList(),
          value: this.value.value == null ? '' : ValueExpressionHelper.makeString(this.value.value!),
          onChanged: (context, value) async {
            this.value.value = value as ValueExpression;
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
        ArgumentType.boolean => _BooleanArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        ArgumentType.integer => _IntegerArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        ArgumentType.floater => _FloaterArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        ArgumentType.size => _SizeArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        ArgumentType.string => _StringArgumentBar(
          name: this.name,
          value: this.value.cast(),
          batch: this.batch,
          expanded: this.expanded,
        ),
        ArgumentType.path => _PathArgumentBar(
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
