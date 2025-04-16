import '/common.dart';
import '/setting.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import '/view/modding_worker/submission_type.dart';
import '/view/modding_worker/value_expression.dart';
import 'dart:async';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

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

  final Completer<Void>?                  completer;
  final List<(ValueExpression, Boolean)>? history;
  final Void Function(ValueExpression)?   onSelect;
  final IconData                          icon;
  final Widget                            content;

  // ----------------

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return CustomBottomBarContent(
      primary: FloatingActionButton(
        tooltip: this.completer == null ? '' : 'Submit',
        elevation: 0,
        focusElevation: 0,
        hoverElevation: 0,
        highlightElevation: 0,
        disabledElevation: 0,
        child: this.completer == null
          ? const CircularProgressIndicator()
          : const Icon(IconSymbols.send),
        onPressed: this.completer == null
          ? null
          : () async {
            this.completer?.complete();
          },
      ),
      secondary: [
        Badge.count(
          textStyle: theme.textTheme.labelSmall?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          isLabelVisible: this.history != null,
          count: this.history == null ? 0 : this.history!.length,
          child: IconButton.filledTonal(
            style: const ButtonStyle(
              padding: WidgetStatePropertyAll(EdgeInsets.zero),
              overlayColor: WidgetStatePropertyAll(Colors.transparent),
            ),
            tooltip: this.history == null ? '' : 'History',
            icon: Stack(
              children: [
                SizedBox(
                  width: 56,
                  height: 40,
                  child: Icon(this.icon, fill: 1),
                ),
                Positioned.fill(
                  child: PopupMenuButton<ValueExpression>(
                    tooltip: '',
                    enabled: this.history != null,
                    position: PopupMenuPosition.under,
                    icon: const SizedBox(),
                    itemBuilder: (context) => [
                      if ((this.history ?? []).isEmpty)
                        const PopupMenuItem(
                          height: 16,
                          enabled: false,
                          child: null,
                        ),
                      ...(this.history ?? []).mapIndexed((index, value) => PopupMenuItem(
                        enabled: value.$2,
                        value: value.$1,
                        child: ListTile(
                          contentPadding: EdgeInsets.zero,
                          dense: true,
                          title: Text(
                            ValueExpressionHelper.makeString(value.$1),
                            overflow: TextOverflow.clip,
                            style: theme.textTheme.labelLarge?.copyWith(
                              fontFamily: '',
                              fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                              color: value.$2 ? null : theme.disabledColor,
                            ),
                          ),
                        ),
                      )),
                    ],
                    onSelected: (value) async {
                      this.onSelect!(value);
                    },
                  ),
                ),
              ],
            ),
            onPressed: this.history == null
              ? null
              : () async {
              },
          ),
        ),
        const SizedBox(width: 16),
        Expanded(
          child: this.content,
        ),
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
    return const _BasicSubmissionBar(
      completer: null,
      history: null,
      onSelect: null,
      icon: IconSymbols.more_horiz,
      content: CustomTextField(
        enabled: false,
        keyboardType: TextInputType.none,
        inputFormatters: [],
        decoration: InputDecoration(
          contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
          filled: false,
          border: UnderlineInputBorder(),
        ),
        value: '',
        onChanged: null,
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as PauseExpression;
          setState(() {});
        },
        icon: IconSymbols.pause,
        content: CustomTextField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          keyboardType: TextInputType.none,
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Pause',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
              ],
            ),
          ),
          value: '',
          onChanged: null,
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as BooleanExpression;
          setState(() {});
        },
        icon: IconSymbols.check_box,
        content: CustomTextField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          keyboardType: TextInputType.text,
          inputFormatters: const [],
          decoration: InputDecoration(
            contentPadding: const EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: const UnderlineInputBorder(),
            hintText: 'Boolean',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
                IconButton(
                  tooltip: 'No',
                  isSelected: this.value.value == null ? false : this.value.value!.value == false,
                  icon: const Icon(IconSymbols.do_not_disturb_on),
                  selectedIcon: const Icon(IconSymbols.do_not_disturb_on, fill: 1),
                  onPressed: () async {
                    this.value.value = this.value.value?.value == false ? null : BooleanExpression(false);
                    setState(() {});
                  },
                ),
                const SizedBox(width: 4),
                IconButton(
                  tooltip: 'Yes',
                  isSelected: this.value.value == null ? false : this.value.value!.value == true,
                  icon: const Icon(IconSymbols.check_circle),
                  selectedIcon: const Icon(IconSymbols.check_circle, fill: 1),
                  onPressed: () async {
                    this.value.value = this.value.value?.value == true ? null : BooleanExpression(true);
                    setState(() {});
                  },
                ),
              ],
            ),
          ),
          value: this.value.value == null ? '' : ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.value.value!.value),
          onChanged: (text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              if (text == 'n' || text == 'y') {
                this.value.value = BooleanExpression(text == 'y');
              }
            }
            setState(() {});
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as IntegerExpression;
          setState(() {});
        },
        icon: IconSymbols.speed_1_2,
        content: CustomTextField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: false),
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Integer',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
              ],
            ),
          ),
          value: this.value.value == null ? '' : ConvertHelper.makeIntegerToString(this.value.value!.value, false),
          onChanged: (text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              var value = Integer.tryParse(text);
              if (value != null) {
                this.value.value = IntegerExpression(value);
              }
            }
            setState(() {});
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as FloaterExpression;
          setState(() {});
        },
        icon: IconSymbols.speed_1_2,
        content: CustomTextField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: true),
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Floater',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
              ],
            ),
          ),
          value: this.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.value.value!.value, false),
          onChanged: (text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              var value = Floater.tryParse(text);
              if (value != null && value.isFinite) {
                this.value.value = FloaterExpression(value);
              }
            }
            setState(() {});
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as SizeExpression;
          setState(() {});
        },
        icon: IconSymbols.memory,
        content: CustomTextField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          keyboardType: const TextInputType.numberWithOptions(signed: false, decimal: true),
          inputFormatters: const [],
          decoration: InputDecoration(
            contentPadding: const EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: const UnderlineInputBorder(),
            hintText: 'Size',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
                PopupMenuButton(
                  tooltip: 'Exponent',
                  position: PopupMenuPosition.under,
                  icon: this.value.value == null
                    ? const Icon(IconSymbols.expand_circle_down)
                    : Container(
                      alignment: Alignment.center,
                      width: 24,
                      height: 24,
                      child: Text(
                        ['B', 'K', 'M', 'G'][this.value.value!.exponent],
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                  itemBuilder: (context) => ['B', 'K', 'M', 'G'].mapIndexed((index, value) => PopupMenuItem(
                    value: index,
                    child: Text(
                      value,
                      overflow: TextOverflow.ellipsis,
                      style: theme.textTheme.labelLarge?.copyWith(
                        fontFamily: '',
                        fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                      ),
                    ),
                  )).toList(),
                  onSelected: (value) async {
                    this.value.value = SizeExpression(this.value.value?.count ?? 1.0, value);
                    setState(() {});
                  },
                ),
              ],
            ),
          ),
          value: this.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.value.value!.count, false),
          onChanged: (text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              var count = Floater.tryParse(text);
              if (count != null && count.isFinite && count >= 0.0) {
                this.value.value = SizeExpression(count, this.value.value?.exponent ?? 2);
              }
            }
            setState(() {});
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as StringExpression;
          setState(() {});
        },
        icon: IconSymbols.text_fields,
        content: CustomTextField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          keyboardType: TextInputType.text,
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'String',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
              ],
            ),
          ),
          value: this.value.value == null ? '' : this.value.value!.value,
          onChanged: (text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = StringExpression(text);
            }
            setState(() {});
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, true)).toList(),
        onSelect: (value) async {
          this.value.value = value as PathExpression;
          setState(() {});
        },
        icon: IconSymbols.link,
        content: CustomFileDropRegion(
          onDrop: (item) async {
            this.value.value = PathExpression(item.first);
            setState(() {});
          },
          child: CustomTextField(
            style: theme.textTheme.bodyLarge?.copyWith(
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
            ),
            keyboardType: TextInputType.text,
            inputFormatters: const [],
            decoration: InputDecoration(
              contentPadding: const EdgeInsets.fromLTRB(8, 12, 8, 12),
              filled: false,
              border: const UnderlineInputBorder(),
              hintText: 'Path',
              suffixIcon: CustomTextFieldSuffixRegion(
                children: [
                  PopupMenuButton(
                    tooltip: 'Command',
                    position: PopupMenuPosition.under,
                    icon: const Icon(IconSymbols.adjust),
                    itemBuilder: (context) => [
                      (':g', 'Generate'),
                      (':m', 'Move'),
                      (':d', 'Delete'),
                      (':o', 'Overwrite'),
                    ].map((value) => PopupMenuItem(
                      value: value.$1,
                      child: Text(
                        value.$2,
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    )).toList(),
                    onSelected: (value) async {
                      this.value.value = PathExpression(value);
                      setState(() {});
                    },
                  ),
                  const SizedBox(width: 4),
                  PopupMenuButton(
                    tooltip: 'Pick',
                    position: PopupMenuPosition.under,
                    icon: const Icon(IconSymbols.open_in_new),
                    itemBuilder: (context) => [
                      ('load_file', 'Load File'),
                      ('load_directory', 'Load Directory'),
                      ('save_file', 'Save File'),
                    ].map((value) => PopupMenuItem(
                      value: value.$1,
                      child: Text(
                        value.$2,
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    )).toList(),
                    onSelected: (value) async {
                      var target = switch (value) {
                        'load_file'      => await StorageHelper.pickLoadFile(context, 'CommandSender.Generic'),
                        'load_directory' => await StorageHelper.pickLoadDirectory(context, 'CommandSender.Generic'),
                        'save_file'      => await StorageHelper.pickSaveFile(context, 'CommandSender.Generic'),
                        _                => throw Exception(),
                      };
                      if (target != null) {
                        this.value.value = PathExpression(target);
                        setState(() {});
                      }
                    },
                  ),
                ],
              ),
            ),
            value: this.value.value == null ? '' : this.value.value!.content,
            onChanged: (text) async {
              if (text.isEmpty) {
                this.value.value = null;
              }
              else {
                this.value.value = PathExpression(StorageHelper.regularize(text));
              }
              setState(() {});
            },
          ),
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => _BasicSubmissionBar(
        completer: this.completer,
        history: this.history.map((e) => (e, this.option.contains(e.item))).toList(),
        onSelect: (value) async {
          this.value.value = value as EnumerationExpression;
          setState(() {});
        },
        icon: IconSymbols.menu,
        content: CustomOptionField(
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          decoration: InputDecoration(
            contentPadding: const EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: const UnderlineInputBorder(),
            hintText: 'Enumeration',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
                IconButton(
                  tooltip: 'Reset',
                  icon: const Icon(IconSymbols.restart_alt),
                  onPressed: () async {
                    this.value.value = null;
                    setState(() {});
                  },
                ),
              ],
            ),
          ),
          option: this.option.map((value) => (value, value)).toList(),
          value: this.value.value == null ? '' : this.value.value!.item,
          onChanged: (value) async {
            this.value.value = EnumerationExpression(value as String);
            setState(() {});
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
      return const _IdleSubmissionBar();
    }
    if (this.type == SubmissionType.pause) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _PauseSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.boolean) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _BooleanSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.integer) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _IntegerSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.floater) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _FloaterSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.size) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _SizeSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.string) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _StringSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.path) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _PathSubmissionBar(
        completer: this.completer!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    if (this.type == SubmissionType.enumeration) {
      assertTest(this.option != null && this.history != null && this.value != null && this.completer != null);
      return _EnumerationSubmissionBar(
        completer: this.completer!,
        option: this.option!,
        history: this.history!.cast(),
        value: this.value!.cast(),
      );
    }
    throw UnimplementedError();
  }

}
