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
    super.key, // ignore: unused_element
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
    return Row(
      children: [
        Badge.count(
          isLabelVisible: this.history != null,
          count: this.history == null ? 0 : this.history!.length,
          textStyle: theme.textTheme.labelSmall?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          child: IconButton.filledTonal(
            tooltip: 'History',
            padding: EdgeInsets.zero,
            onPressed: this.history == null
              ? null
              : () async {
              },
            icon: Stack(
              children: [
                SizedBox(
                  width: 56,
                  child: Icon(this.icon),
                ),
                Positioned.fill(
                  child: PopupMenuButton<ValueExpression>(
                    tooltip: '',
                    enabled: this.history != null,
                    style: const ButtonStyle(
                      overlayColor: WidgetStatePropertyAll(Colors.transparent),
                    ),
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
          ),
        ),
        const SizedBox(width: 16),
        Expanded(
          child: this.content,
        ),
        const SizedBox(width: 16),
        FloatingActionButton(
          tooltip: 'Submit',
          elevation: 0,
          focusElevation: 0,
          hoverElevation: 0,
          highlightElevation: 0,
          disabledElevation: 0,
          onPressed: this.completer == null
            ? null
            : () async {
              this.completer?.complete();
            },
          child: this.completer == null
            ? const CircularProgressIndicator()
            : const Icon(IconSymbols.send),
        ),
      ],
    );
  }

}

// ----------------

class _IdleSubmissionBar extends StatelessWidget {

  const _IdleSubmissionBar({
    super.key, // ignore: unused_element
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
      content: TextField(
        enabled: false,
      ),
    );
  }

}

// ----------------

class _PauseSubmissionBar extends StatefulWidget {

  const _PauseSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _PauseSubmissionBarState();

  // ----------------

  final Completer<Void>           completer;
  final List<PauseExpression>     history;
  final Wrapper<PauseExpression?> value;

}

class _PauseSubmissionBarState extends State<_PauseSubmissionBar> {

  Void _refresh(
  ) async {
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._refresh();
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as PauseExpression;
        this._refresh();
      },
      icon: IconSymbols.pause,
      content: TextField(
        keyboardType: TextInputType.none,
        inputFormatters: const [],
        decoration: const InputDecoration(
          border: UnderlineInputBorder(),
          hintText: 'Pause',
          suffixIconConstraints: BoxConstraints(),
          suffixIcon: CustomTextFieldSuffixWidget(
            children: [
            ],
          ),
        ),
        style: theme.textTheme.bodyLarge?.copyWith(
          fontFamily: '',
          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
        ),
        readOnly: true,
      ),
    );
  }

}

// ----------------

class _BooleanSubmissionBar extends StatefulWidget {

  const _BooleanSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _BooleanSubmissionBarState();

  // ----------------

  final Completer<Void>             completer;
  final List<BooleanExpression>     history;
  final Wrapper<BooleanExpression?> value;

}

class _BooleanSubmissionBarState extends State<_BooleanSubmissionBar> {

  late TextEditingController _valueController;

  Void _refresh(
  ) async {
    this._valueController.text = this.widget.value.value == null ? '' : ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.widget.value.value!.value);
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._valueController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as BooleanExpression;
        this._refresh();
      },
      icon: IconSymbols.check_box,
      content: Focus(
        onFocusChange: (focused) async {
          if (!focused) {
            if (this._valueController.text.isEmpty) {
              this.widget.value.value = null;
            }
            else {
              if (this._valueController.text == 'n' || this._valueController.text == 'y') {
                this.widget.value.value = BooleanExpression(this._valueController.text == 'y');
              }
            }
            this._refresh();
          }
        },
        child: TextField(
          keyboardType: TextInputType.text,
          inputFormatters: const [],
          decoration: InputDecoration(
            border: const UnderlineInputBorder(),
            hintText: 'Boolean',
            suffixIconConstraints: const BoxConstraints(),
            suffixIcon: CustomTextFieldSuffixWidget(
              children: [
                IconButton(
                  tooltip: 'No',
                  isSelected: this.widget.value.value == null ? false : this.widget.value.value!.value == false,
                  icon: const Icon(IconSymbols.do_not_disturb_on),
                  selectedIcon: const Icon(IconSymbols.do_not_disturb_on, fill: 1),
                  onPressed: () async {
                    this.widget.value.value = this.widget.value.value?.value == false ? null : BooleanExpression(false);
                    this._refresh();
                  },
                ),
                const SizedBox(width: 4),
                IconButton(
                  tooltip: 'Yes',
                  isSelected: this.widget.value.value == null ? false : this.widget.value.value!.value == true,
                  icon: const Icon(IconSymbols.check_circle),
                  selectedIcon: const Icon(IconSymbols.check_circle, fill: 1),
                  onPressed: () async {
                    this.widget.value.value = this.widget.value.value?.value == true ? null : BooleanExpression(true);
                    this._refresh();
                  },
                ),
              ],
            ),
          ),
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _IntegerSubmissionBar extends StatefulWidget {

  const _IntegerSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _IntegerSubmissionBarState();

  // ----------------

  final Completer<Void>             completer;
  final List<IntegerExpression>     history;
  final Wrapper<IntegerExpression?> value;

}

class _IntegerSubmissionBarState extends State<_IntegerSubmissionBar> {

  late TextEditingController _valueController;

  Void _refresh(
  ) async {
    this._valueController.text = this.widget.value.value == null ? '' : ConvertHelper.makeIntegerToString(this.widget.value.value!.value, false);
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._valueController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as IntegerExpression;
        this._refresh();
      },
      icon: IconSymbols.speed_1_2,
      content: Focus(
        onFocusChange: (focused) async {
          if (!focused) {
            if (this._valueController.text.isEmpty) {
              this.widget.value.value = null;
            }
            else {
              var value = Integer.tryParse(this._valueController.text);
              if (value != null) {
                this.widget.value.value = IntegerExpression(value);
              }
            }
            this._refresh();
          }
        },
        child: TextField(
          keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: false),
          inputFormatters: const [],
          decoration: const InputDecoration(
            border: UnderlineInputBorder(),
            hintText: 'Integer',
            suffixIconConstraints: BoxConstraints(),
            suffixIcon: CustomTextFieldSuffixWidget(
              children: [
              ],
            ),
          ),
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _FloaterSubmissionBar extends StatefulWidget {

  const _FloaterSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _FloaterSubmissionBarState();

  // ----------------

  final Completer<Void>             completer;
  final List<FloaterExpression>     history;
  final Wrapper<FloaterExpression?> value;

}

class _FloaterSubmissionBarState extends State<_FloaterSubmissionBar> {

  late TextEditingController _valueController;

  Void _refresh(
  ) async {
    this._valueController.text = this.widget.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.widget.value.value!.value, false);
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._valueController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as FloaterExpression;
        this._refresh();
      },
      icon: IconSymbols.speed_1_2,
      content: Focus(
        onFocusChange: (focused) async {
          if (!focused) {
            if (this._valueController.text.isEmpty) {
              this.widget.value.value = null;
            }
            else {
              var value = Floater.tryParse(this._valueController.text);
              if (value != null && value.isFinite) {
                this.widget.value.value = FloaterExpression(value);
              }
            }
            this._refresh();
          }
        },
        child: TextField(
          keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: true),
          inputFormatters: const [],
          decoration: const InputDecoration(
            border: UnderlineInputBorder(),
            hintText: 'Floater',
            suffixIconConstraints: BoxConstraints(),
            suffixIcon: CustomTextFieldSuffixWidget(
              children: [
              ],
            ),
          ),
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _SizeSubmissionBar extends StatefulWidget {

  const _SizeSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _SizeSubmissionBarState();

  // ----------------

  final Completer<Void>          completer;
  final List<SizeExpression>     history;
  final Wrapper<SizeExpression?> value;

}

class _SizeSubmissionBarState extends State<_SizeSubmissionBar> {

  late Integer               _exponent;
  late TextEditingController _countController;

  Void _refresh(
  ) async {
    this._exponent = this.widget.value.value == null ? this._exponent : this.widget.value.value!.exponent;
    this._countController.text = this.widget.value.value == null ? '' : ConvertHelper.makeFloaterToString(this.widget.value.value!.count, false);
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._exponent = 2;
    this._countController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._countController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as SizeExpression;
        this._refresh();
      },
      icon: IconSymbols.memory,
      content: Focus(
        onFocusChange: (focused) async {
          if (!focused) {
            if (this._countController.text.isEmpty) {
              this.widget.value.value = null;
            }
            else {
              var count = Floater.tryParse(this._countController.text);
              if (count != null && count.isFinite && count >= 0.0) {
                this.widget.value.value = SizeExpression(count, this._exponent);
              }
            }
            this._refresh();
          }
        },
        child: TextField(
          keyboardType: const TextInputType.numberWithOptions(signed: false, decimal: true),
          inputFormatters: const [],
          decoration: InputDecoration(
            border: const UnderlineInputBorder(),
            hintText: 'Size',
            suffixIconConstraints: const BoxConstraints(),
            suffixIcon: CustomTextFieldSuffixWidget(
              children: [
                PopupMenuButton(
                  tooltip: 'Exponent',
                  position: PopupMenuPosition.under,
                  icon: Text(
                    ['B', 'K', 'M', 'G'][this._exponent],
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.labelLarge?.copyWith(
                      fontFamily: '',
                      fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
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
                    this._exponent = value;
                    if (this.widget.value.value != null) {
                      this.widget.value.value = SizeExpression(this.widget.value.value!.count, this._exponent);
                    }
                    this._refresh();
                  },
                ),
              ],
            ),
          ),
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          controller: this._countController,
        ),
      ),
    );
  }

}

// ----------------

class _StringSubmissionBar extends StatefulWidget {

  const _StringSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _StringSubmissionBarState();

  // ----------------

  final Completer<Void>            completer;
  final List<StringExpression>     history;
  final Wrapper<StringExpression?> value;

}

class _StringSubmissionBarState extends State<_StringSubmissionBar> {

  late TextEditingController _valueController;

  Void _refresh(
  ) async {
    this._valueController.text = this.widget.value.value == null ? '' : this.widget.value.value!.value;
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._valueController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as StringExpression;
        this._refresh();
      },
      icon: IconSymbols.text_fields,
      content: Focus(
        onFocusChange: (focused) async {
          if (!focused) {
            if (this._valueController.text.isEmpty) {
              this.widget.value.value = null;
            }
            else {
              this.widget.value.value = StringExpression(this._valueController.text);
            }
            this._refresh();
          }
        },
        child: TextField(
          keyboardType: TextInputType.text,
          inputFormatters: const [],
          decoration: const InputDecoration(
            border: UnderlineInputBorder(),
            hintText: 'String',
            suffixIconConstraints: BoxConstraints(),
            suffixIcon: CustomTextFieldSuffixWidget(
              children: [
              ],
            ),
          ),
          style: theme.textTheme.bodyLarge?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _PathSubmissionBar extends StatefulWidget {

  const _PathSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.history,
    required this.value,
  });

  @override
  createState() => _PathSubmissionBarState();

  // ----------------

  final Completer<Void>          completer;
  final List<PathExpression>     history;
  final Wrapper<PathExpression?> value;

}

class _PathSubmissionBarState extends State<_PathSubmissionBar> {

  late TextEditingController _contentController;

  Void _refresh(
  ) async {
    this._contentController.text = this.widget.value.value == null ? '' : this.widget.value.value!.content;
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._contentController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._contentController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, true)).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as PathExpression;
        this._refresh();
      },
      icon: IconSymbols.link,
      content: CustomFileDropRegion(
        onDrop: (item) async {
          this.widget.value.value = PathExpression(item.first);
          this._refresh();
        },
        child: Focus(
          onFocusChange: (focused) async {
            if (!focused) {
              if (this._contentController.text.isEmpty) {
                this.widget.value.value = null;
              }
              else {
                this.widget.value.value = PathExpression(StorageHelper.regularize(this._contentController.text));
              }
              this._refresh();
            }
          },
          child: TextField(
            keyboardType: TextInputType.text,
            inputFormatters: const [],
            decoration: InputDecoration(
              border: const UnderlineInputBorder(),
              hintText: 'Path',
              suffixIconConstraints: const BoxConstraints(),
              suffixIcon: CustomTextFieldSuffixWidget(
                children: [
                  PopupMenuButton(
                    tooltip: 'Command',
                    position: PopupMenuPosition.under,
                    icon: const Icon(IconSymbols.adjust),
                    itemBuilder: (context) => <PopupMenuEntry<String>>[
                      ...[
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
                      )),
                    ],
                    onSelected: (value) async {
                      this.widget.value.value = PathExpression(value);
                      this._refresh();
                    },
                  ),
                  const SizedBox(width: 4),
                  PopupMenuButton(
                    tooltip: 'Pick',
                    position: PopupMenuPosition.under,
                    icon: const Icon(IconSymbols.open_in_new),
                    itemBuilder: (context) => <PopupMenuEntry<String>>[
                      ...[
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
                      )),
                    ],
                    onSelected: (value) async {
                      var target = switch (value) {
                        'load_file'      => await StorageHelper.pickLoadFile(context, 'ModdingWorker.Generic'),
                        'load_directory' => await StorageHelper.pickLoadDirectory(context, 'ModdingWorker.Generic'),
                        'save_file'      => await StorageHelper.pickSaveFile(context, 'ModdingWorker.Generic'),
                        _                => throw Exception(),
                      };
                      if (target != null) {
                        this.widget.value.value = PathExpression(target);
                        this._refresh();
                      }
                    },
                  ),
                ],
              ),
            ),
            style: theme.textTheme.bodyLarge?.copyWith(
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
            ),
            controller: this._contentController,
          ),
        ),
      ),
    );
  }

}

// ----------------

class _EnumerationSubmissionBar extends StatefulWidget {

  const _EnumerationSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.option,
    required this.history,
    required this.value,
  });

  @override
  createState() => _EnumerationSubmissionBarState();

  // ----------------

  final Completer<Void>                 completer;
  final List<String>                    option;
  final List<EnumerationExpression>     history;
  final Wrapper<EnumerationExpression?> value;

}

class _EnumerationSubmissionBarState extends State<_EnumerationSubmissionBar> {

  late TextEditingController _itemController;

  Void _refresh(
  ) async {
    this._itemController.text = this.widget.value.value == null ? '' : this.widget.value.value!.item;
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._itemController = TextEditingController();
    this._refresh();
    return;
  }

  @override
  dispose() {
    this._itemController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BasicSubmissionBar(
      completer: this.widget.completer,
      history: this.widget.history.map((e) => (e, this.widget.option.contains(e.item))).toList(),
      onSelect: (value) async {
        this.widget.value.value = value as EnumerationExpression;
        this._refresh();
      },
      icon: IconSymbols.menu,
      content: LayoutBuilder(
        builder: (context, constraints) => MenuAnchor(
          crossAxisUnconstrained: false,
          alignmentOffset: const Offset(-12, 0),
          style: MenuStyle(
            minimumSize: WidgetStatePropertyAll(Size(constraints.maxWidth + 24, 0)),
            maximumSize: WidgetStatePropertyAll(Size(constraints.maxWidth + 24, Floater.infinity)),
          ),
          menuChildren: this.widget.option.map((value) => MenuItemButton(
            style: ButtonStyle(
              backgroundColor: WidgetStatePropertyAll(value != this.widget.value.value?.item ? null : theme.colorScheme.onSurface.withOpacity(0.12)),
            ),
            onPressed: () async {
              this.widget.value.value = EnumerationExpression(value);
              this._refresh();
            },
            child: ConstrainedBox(
              constraints: BoxConstraints(maxWidth: constraints.maxWidth - 24),
              child: ListTile(
                contentPadding: EdgeInsets.zero,
                dense: true,
                title: Text(
                  value,
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyLarge?.copyWith(
                    fontFamily: '',
                    fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                  ),
                ),
              ),
            ),
          )).toList(),
          builder: (context, controller, child) => TextField(
            keyboardType: TextInputType.none,
            inputFormatters: const [],
            decoration: InputDecoration(
              border: const UnderlineInputBorder(),
              hintText: 'Enumeration',
              suffixIconConstraints: const BoxConstraints(),
              suffixIcon: CustomTextFieldSuffixWidget(
                children: [
                  IconButton(
                    tooltip: 'Reset',
                    icon: const Icon(IconSymbols.restart_alt),
                    onPressed: () async {
                      this.widget.value.value = null;
                      this._refresh();
                    },
                  ),
                ],
              ),
            ),
            style: theme.textTheme.bodyLarge?.copyWith(
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
            ),
            readOnly: true,
            controller: this._itemController,
            onTap: () async {
              if (controller.isOpen) {
                controller.close();
              }
              else {
                controller.open();
              }
            },
          ),
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
