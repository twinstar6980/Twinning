import '/common.dart';
import '/module.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/miscellaneous_helper.dart';
import '/widget/export.dart';
import '/view/core_task_worker/submission_type.dart';
import '/view/core_task_worker/value_expression.dart';
import 'package:collection/collection.dart' as lib;

// ----------------

class _BasicSubmissionBar extends StatelessWidget {

  const _BasicSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.terminate,
    required this.onTerminate,
    required this.onSubmit,
    required this.history,
    required this.macro,
    required this.value,
    required this.onHistory,
    required this.onMacro,
    required this.icon,
    required this.content,
  });

  // ----------------

  final Boolean?                                          terminate;
  final Void Function()?                                  onTerminate;
  final ValueExpression?                                  value;
  final Void Function()?                                  onSubmit;
  final List<({ValueExpression value, Boolean enabled})>? history;
  final Void Function(ValueExpression value)?             onHistory;
  final List<({String value, String name})>?              macro;
  final Void Function(String? value)?                     onMacro;
  final IconData                                          icon;
  final Widget                                            content;

  // ----------------

  @override
  build(context) {
    return StyledBottomBar.standard(
      primary: StyledFloatingButton.standard(
        tooltip: 'Submit',
        icon: IconView.of(this.onSubmit != null ? IconSet.play_circle : !this.terminate! ? IconSet.stop_circle : IconSet.play_circle),
        onPressed: (context) async {
          if (this.onSubmit != null) {
            this.onSubmit!();
          }
          else {
            this.onTerminate!();
          }
        },
      ),
      secondary: [
        StyledBadge.standard(
          visible: this.history != null,
          label: StyledText.inherit('${this.history == null ? 0 : this.history!.length}'),
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
                  enabled: value.enabled,
                  value: value.value,
                  content: StyledText.custom(
                    ValueExpressionHelper.makeString(value.value),
                    overflow: .clip,
                  ),
                )),
              ));
              if (value != null) {
                this.onHistory!(value);
              }
            },
          ),
        ),
        Gap.horizontal(16),
        if (this.value?.macro == null) ...[
          this.content.withFlexExpanded(),
        ],
        if (this.value?.macro != null) ...[
          StyledInputCombo.underlined(
            hint: null,
            prefix: null,
            suffix: [
              StyledIconButton.standard(
                tooltip: 'Reset',
                icon: IconView.of(IconSet.adjust, fill: 1),
                onPressed: (context) async {
                  this.onMacro!(null);
                },
              ),
            ],
            option: [
              (value: 'terminate', name: 'Terminate'),
              ...this.macro!,
            ],
            value: this.value!.macro!,
            onChanged: (context, value) async {
              value as String;
              this.onMacro!(value);
            },
          ).withFlexExpanded(),
        ],
      ],
    );
  }

}

// ----------------

class _IdleSubmissionBar extends StatelessWidget {

  const _IdleSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.terminate,
    required this.onTerminate,
  });

  // ----------------

  final Wrapper<Boolean> terminate;
  final Void Function()  onTerminate;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: this.terminate.value,
        onTerminate: () async {
          this.onTerminate();
          await refreshState(setState);
        },
        value: null,
        onSubmit: null,
        history: null,
        onHistory: null,
        macro: null,
        onMacro: null,
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
      ),
    );
  }

}

class _PauseSubmissionBar extends StatelessWidget {

  const _PauseSubmissionBar({
    super.key, // ignore: unused_element_parameter
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<PauseExpression>     history;
  final Wrapper<PauseExpression?> value;
  final Void Function()           onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value);
          await refreshState(setState);
        },
        icon: IconSet.pause,
        content: StyledInput.underlined(
          type: .none,
          format: null,
          hint: 'Pause',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate');
                await refreshState(setState);
              },
            ),
          ],
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
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------


  final List<BooleanExpression>     history;
  final Wrapper<BooleanExpression?> value;
  final Void Function()             onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, false);
          await refreshState(setState);
        },
        icon: IconSet.check_box,
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
                this.value.value = this.value.value?.value == false ? null : .new(null, false);
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
                this.value.value = this.value.value?.value == true ? null : .new(null, true);
                await refreshState(setState);
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', false);
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.value.selfLet((it) => ConvertHelper.makeBooleanToStringOfConfirmationCharacter(it)),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              if (value == 'n' || value == 'y') {
                this.value.value = .new(null, value == 'y');
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
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<IntegerExpression>     history;
  final Wrapper<IntegerExpression?> value;
  final Void Function()             onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, 0);
          await refreshState(setState);
        },
        icon: IconSet.speed_1_2,
        content: StyledInput.underlined(
          type: .numberWithOptions(signed: true, decimal: false),
          format: null,
          hint: 'Integer',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', 0);
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.value.selfLet((it) => ConvertHelper.makeIntegerToString(it)),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              var parsedValue = Integer.tryParse(value);
              if (parsedValue != null) {
                this.value.value = .new(null, parsedValue);
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
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<FloaterExpression>     history;
  final Wrapper<FloaterExpression?> value;
  final Void Function()             onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, 0.0);
          await refreshState(setState);
        },
        icon: IconSet.speed_1_2,
        content: StyledInput.underlined(
          type: .numberWithOptions(signed: true, decimal: true),
          format: null,
          hint: 'Floater',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', 0.0);
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.value.selfLet((it) => ConvertHelper.makeFloaterToString(it)),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              var parsedValue = Floater.tryParse(value);
              if (parsedValue != null && parsedValue.isFinite) {
                this.value.value = .new(null, parsedValue);
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
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<StringExpression>     history;
  final Wrapper<StringExpression?> value;
  final Void Function()            onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
          (value: 'empty', name: 'Empty'),
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, '');
          await refreshState(setState);
        },
        icon: IconSet.text_fields,
        content: StyledInput.underlined(
          type: .text,
          format: null,
          hint: 'String',
          prefix: null,
          suffix: [
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', '');
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.value.selfLet((it) => it),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = .new(null, value);
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
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<SizeExpression>     history;
  final Wrapper<SizeExpression?> value;
  final Void Function()          onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, 0.0, 0);
          await refreshState(setState);
        },
        icon: IconSet.memory,
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
                    content: StyledText.inherit(tooltip: true, value),
                  )),
                ));
                if (value != null) {
                  this.value.value = .new(null, this.value.value?.count ?? 1.0, value);
                  await refreshState(setState);
                }
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', 0.0, 0);
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.count.selfLet((it) => ConvertHelper.makeFloaterToString(it)),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              var parsedCount = Floater.tryParse(value);
              if (parsedCount != null && parsedCount.isFinite && parsedCount >= 0.0) {
                this.value.value = .new(null, parsedCount, this.value.value?.exponent ?? 2);
              }
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
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<PathExpression>     history;
  final Wrapper<PathExpression?> value;
  final Void Function()          onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: true)).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
          (value: 'generate', name: 'Generate'),
          (value: 'move', name: 'Move'),
          (value: 'delete', name: 'Delete'),
          (value: 'overwrite', name: 'Overwrite'),
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, .new());
          await refreshState(setState);
        },
        icon: IconSet.link,
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
                var target = (await MiscellaneousHelper.pickStorageItem(context, '${ModuleHelper.query(.coreTaskWorker).identifier}.generic', [.loadFile, .loadDirectory, .saveFile], false, null, null)).firstOrNull;
                if (target != null) {
                  this.value.value = .new(null, target);
                  await refreshState(setState);
                }
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', .new());
                await refreshState(setState);
              },
            ),
          ],
          value: this.value.value == null ? '' : this.value.value!.content.selfLet((it) => it.emit()),
          onChanged: (context, value) async {
            if (value.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = .new(null, .of(value));
            }
            await refreshState(setState);
          },
        ).withStorageDropRegion(
          onDrop: (item) async {
            this.value.value = .new(null, item.first);
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
    required this.option,
    required this.history,
    required this.value,
    required this.onSubmit,
  });

  // ----------------

  final List<String>                    option;
  final List<EnumerationExpression>     history;
  final Wrapper<EnumerationExpression?> value;
  final Void Function()                 onSubmit;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        terminate: null,
        onTerminate: null,
        value: this.value.value,
        onSubmit: this.onSubmit,
        history: this.history.map((item) => (value: item, enabled: this.option.contains(item.item))).toList(),
        onHistory: (value) async {
          this.value.value = value.as();
          await refreshState(setState);
        },
        macro: [
        ],
        onMacro: (value) async {
          this.value.value = value == null ? null : .new(value, '');
          await refreshState(setState);
        },
        icon: IconSet.menu,
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
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Macro',
              icon: IconView.of(IconSet.adjust),
              onPressed: (context) async {
                this.value.value = .new('terminate', '');
                await refreshState(setState);
              },
            ),
          ],
          option: this.option.map((value) => (value: value, name: value)).toList(),
          value: this.value.value == null ? null : this.value.value!.item, // ignore: prefer_null_aware_operators
          onChanged: (context, value) async {
            value as String;
            this.value.value = .new(null, value);
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
    required this.onSubmit,
    required this.terminate,
    required this.onTerminate,
  });

  // ----------------

  final SubmissionType?            type;
  final List<String>?              option;
  final List<ValueExpression>?     history;
  final Wrapper<ValueExpression?>? value;
  final Void Function()?           onSubmit;
  final Wrapper<Boolean>?          terminate;
  final Void Function()?           onTerminate;

  // ----------------

  @override
  build(context) {
    if (this.type == null) {
      assertTest(this.option == null && this.history == null && this.value == null && this.onSubmit == null && this.terminate != null && this.onTerminate != null);
      return _IdleSubmissionBar(
        terminate: this.terminate!,
        onTerminate: this.onTerminate!,
      );
    }
    else {
      assertTest(this.option != null && this.history != null && this.value != null && this.onSubmit != null && this.terminate == null && this.onTerminate == null);
      return switch (this.type!) {
        .pause => _PauseSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .boolean => _BooleanSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .integer => _IntegerSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .floater => _FloaterSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .string => _StringSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .size => _SizeSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .path => _PathSubmissionBar(
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
        .enumeration => _EnumerationSubmissionBar(
          option: this.option!,
          history: this.history!.cast(),
          value: this.value!.cast(),
          onSubmit: this.onSubmit!,
        ),
      };
    }
  }

}
