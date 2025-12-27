import '/common.dart';
import '/module.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/core_task_worker/submission_type.dart';
import '/view/core_task_worker/value_expression.dart';
import '/view/core_task_worker/main_page.dart';
import 'dart:async';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';

// ----------------

class _BasicSubmissionBar extends StatelessWidget {

  const _BasicSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.onSelect,
    required this.icon,
    required this.content,
  });

  // ----------------

  final Completer<Void>?                      completer;
  final List<(ValueExpression, Boolean)>?     history;
  final Void Function(ValueExpression value)? onSelect;
  final IconData                              icon;
  final Widget                                content;

  // ----------------

  @override
  build(context) {
    return StyledBottomBar.standard(
      primary: StyledFloatingButton.standard(
        enabled: this.completer != null,
        tooltip: 'Submit',
        icon: this.completer == null
          ? StyledProgress.circular()
          : IconView.of(IconSet.send),
        onPressed: (context) async {
          this.completer!.complete();
        },
      ),
      secondary: [
        StyledBadge.standard(
          visible: this.history != null,
          label: StyledText.custom(
            '${this.history == null ? 0 : this.history!.length}',
            style: getSpecialFontTextStyle(context),
          ),
          child: StyledIconButton.filledTonal(
            enabled: this.history != null,
            tooltip: 'History',
            icon: BoxContainer.of(
              constraints: .tightFor(width: 40),
              child: IconView.of(this.icon, fill: 1),
            ),
            onPressed: (context) async {
              var value = await StyledMenuExtension.show<ValueExpression>(context, StyledMenu.standard(
                position: .under,
                content: (this.history ?? []).mapIndexed((index, value) => StyledMenuItem.standard(
                  enabled: value.$2,
                  value: value.$1,
                  content: StyledText.custom(
                    ValueExpressionHelper.makeString(value.$1),
                    overflow: .clip,
                    style: getSpecialFontTextStyle(context, listen: false),
                  ),
                )),
              ));
              if (value != null) {
                this.onSelect!(value);
              }
            },
          ),
        ),
        Gap.horizontal(16),
        this.content.withFlexExpanded(),
      ],
    );
  }

}

// ----------------

class _IdleSubmissionBar extends StatelessWidget {

  const _IdleSubmissionBar({
    super.key, // ignore: unused_element_parameter
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return _BasicSubmissionBar(
      completer: null,
      history: null,
      onSelect: null,
      icon: IconSet.more_horiz,
      content: StyledInput.underlined(
        enabled: false,
        type: .none,
        format: null,
        hint: null,
        prefix: null,
        suffix: null,
        value: '',
        onChanged: (context, value) async {
        },
      ),
    );
  }

}

class _PauseSubmissionBar extends StatelessWidget {

  const _PauseSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>           completer;
  final List<PauseExpression>     history;
  final Wrapper<PauseExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as PauseExpression;
          await refreshState(setState);
        },
        icon: IconSet.pause,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
          type: .none,
          format: null,
          hint: 'Pause',
          prefix: null,
          suffix: null,
          value: '',
          onChanged: (context, value) async {
          },
        ),
      ),
    );
  }

}

class _BooleanSubmissionBar extends StatelessWidget {

  const _BooleanSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>             completer;
  final List<BooleanExpression>     history;
  final Wrapper<BooleanExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as BooleanExpression;
          await refreshState(setState);
        },
        icon: IconSet.check_box,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
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

class _IntegerSubmissionBar extends StatelessWidget {

  const _IntegerSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>             completer;
  final List<IntegerExpression>     history;
  final Wrapper<IntegerExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as IntegerExpression;
          await refreshState(setState);
        },
        icon: IconSet.speed_1_2,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
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

class _FloaterSubmissionBar extends StatelessWidget {

  const _FloaterSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>             completer;
  final List<FloaterExpression>     history;
  final Wrapper<FloaterExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as FloaterExpression;
          await refreshState(setState);
        },
        icon: IconSet.speed_1_2,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
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

class _SizeSubmissionBar extends StatelessWidget {

  const _SizeSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>          completer;
  final List<SizeExpression>     history;
  final Wrapper<SizeExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as SizeExpression;
          await refreshState(setState);
        },
        icon: IconSet.memory,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
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
                  child: StyledText.custom(
                    ['B', 'K', 'M', 'G'][this.value.value!.exponent],
                    style: getSpecialFontTextStyle(context),
                  ),
                ),
              onPressed: (context) async {
                var value = await StyledMenuExtension.show<Integer>(context, StyledMenu.standard(
                  position: .under,
                  content: ['B', 'K', 'M', 'G'].mapIndexed((index, value) => StyledMenuItem.standard(
                    value: index,
                    content: StyledText.custom(
                      value,
                      style: getSpecialFontTextStyle(context, listen: false),
                    ),
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

class _StringSubmissionBar extends StatelessWidget {

  const _StringSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>            completer;
  final List<StringExpression>     history;
  final Wrapper<StringExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as StringExpression;
          await refreshState(setState);
        },
        icon: IconSet.text_fields,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
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

class _PathSubmissionBar extends StatelessWidget {

  const _PathSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>          completer;
  final List<PathExpression>     history;
  final Wrapper<PathExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as PathExpression;
          await refreshState(setState);
        },
        icon: IconSet.link,
        content: StyledInput.underlined(
          style: getSpecialFontTextStyle(context),
          type: .text,
          format: null,
          hint: 'Path',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Command',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                var value = await StyledMenuExtension.show<String>(context, StyledMenu.standard(
                  position: .under,
                  content: [
                    ('?generate', 'Generate'),
                    ('?move', 'Move'),
                    ('?delete', 'Delete'),
                    ('?overwrite', 'Overwrite'),
                  ].mapIndexed((index, value) => StyledMenuItem.standard(
                    value: value.$1,
                    content: StyledText.custom(
                      value.$2,
                      style: getSpecialFontTextStyle(context, listen: false),
                    ),
                  )),
                ));
                if (value != null) {
                  this.value.value = .new(value);
                  await refreshState(setState);
                }
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Pick',
              icon: IconView.of(IconSet.open_in_new),
              onPressed: (context) async {
                var target = await pickStorageItem(
                  context: context,
                  allowLoadFile: true,
                  allowLoadDirectory: true,
                  allowSaveFile: true,
                  location: '@${ModuleType.core_task_worker.name}.generic',
                  textStyle: getSpecialFontTextStyle(context, listen: false),
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

class _EnumerationSubmissionBar extends StatelessWidget {

  const _EnumerationSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.completer,
    required this.option,
    required this.history,
    required this.value,
  });

  // ----------------

  final Completer<Void>                 completer;
  final List<String>                    option;
  final List<EnumerationExpression>     history;
  final Wrapper<EnumerationExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((item) => (item, this.option.contains(item.item))).toList(),
        onSelect: (value) async {
          this.value.value = value as EnumerationExpression;
          await refreshState(setState);
        },
        icon: IconSet.menu,
        content: StyledInputCombo.underlined(
          style: getSpecialFontTextStyle(context),
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
          option: this.option.map((value) => (value, value)).toList(),
          value: this.value.value == null ? null : this.value.value!.item, // ignore: prefer_null_aware_operators
          onChanged: (context, value) async {
            value as String;
            this.value.value = .new(value);
            await refreshState(setState);
          },
        ),
      ),
    );
  }

}

// ----------------

class SubmissionBar extends StatelessWidget {

  const SubmissionBar({
    super.key,
    required this.type,
    required this.option,
    required this.history,
    required this.value,
    required this.completer,
  });

  // ----------------

  final SubmissionType?            type;
  final List<String>?              option;
  final List<ValueExpression>?     history;
  final Wrapper<ValueExpression?>? value;
  final Completer<Void>?           completer;

  // ----------------

  @override
  build(context) {
    if (this.type == null) {
      assertTest(this.option == null && this.history == null && this.value == null && this.completer == null);
      return _IdleSubmissionBar();
    }
    else {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return switch (this.type!) {
        .pause => _PauseSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .boolean => _BooleanSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .integer => _IntegerSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .floater => _FloaterSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .size => _SizeSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .string => _StringSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .path => _PathSubmissionBar(
          completer: this.completer!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
        .enumeration => _EnumerationSubmissionBar(
          completer: this.completer!,
          option: this.option!,
          history: this.history!.cast(),
          value: this.value!.cast(),
        ),
      };
    }
  }

}
