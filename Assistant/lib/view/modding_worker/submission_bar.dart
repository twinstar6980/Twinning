import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import '/view/modding_worker/submission_type.dart';
import '/view/modding_worker/value_expression.dart';
import 'dart:async';
import 'package:collection/collection.dart';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class _BasicSubmissionBar extends StatelessWidget {

  const _BasicSubmissionBar({
    super.key, // ignore: unused_element
    required this.history,
    required this.onSelect,
    required this.onSubmit,
    required this.icon,
    required this.child,
  });

  // ----------------

  final List<(ValueExpression, Boolean)>? history;
  final Void Function(ValueExpression)?   onSelect;
  final Void Function()?                  onSubmit;
  final IconData                          icon;
  final Widget                            child;

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
          child: SizedBox(
            width: 56,
            height: 40,
            child: IconButton.filledTonal(
              padding: EdgeInsets.zero,
              icon: Stack(
                alignment: AlignmentDirectional.center,
                children: [
                  Icon(this.icon),
                  SizedBox.expand(
                    child: PopupMenuButton(
                      enabled: this.history != null,
                      style: const ButtonStyle(
                        overlayColor: WidgetStatePropertyAll(Colors.transparent),
                      ),
                      position: PopupMenuPosition.under,
                      tooltip: '',
                      icon: const SizedBox(),
                      itemBuilder: (context) => (this.history ?? []).mapIndexed((index, value) => PopupMenuItem(
                        enabled: value.$2,
                        value: value.$1,
                        child: Text(
                          ValueExpressionHelper.makeString(value.$1),
                          overflow: TextOverflow.ellipsis,
                          style: theme.textTheme.labelLarge?.copyWith(
                            fontFamily: '',
                            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                            color: value.$2 ? null : theme.disabledColor,
                          ),
                        ),
                      )).toList(),
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
        ),
        const SizedBox(width: 16),
        Expanded(
          child: this.child,
        ),
        const SizedBox(width: 16),
        FloatingActionButton(
          elevation: 0,
          focusElevation: 0,
          hoverElevation: 0,
          highlightElevation: 0,
          disabledElevation: 0,
          onPressed: this.onSubmit,
          child: this.onSubmit == null
            ? const CircularProgressIndicator()
            : const Icon(IconSymbols.send),
        ),
      ],
    );
  }

}

class _IdleSubmissionBar extends StatelessWidget {

  const _IdleSubmissionBar({
    super.key, // ignore: unused_element
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return const _BasicSubmissionBar(
      history: null,
      onSelect: null,
      onSubmit: null,
      icon: IconSymbols.more_horiz,
      child: SizedBox.expand(
        child: TextField(
          enabled: false,
        ),
      ),
    );
  }

}

class _BusySubmissionBar extends StatelessWidget {

  const _BusySubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.getValue,
    required this.history,
    required this.onSelect,
    required this.icon,
    required this.content,
  });

  // ----------------

  final Completer<ValueExpression?>      completer;
  final ValueExpression? Function()      getValue;
  final List<(ValueExpression, Boolean)> history;
  final Void Function(ValueExpression)   onSelect;
  final IconData                         icon;
  final Widget                           content;

  // ----------------

  @override
  build(context) {
    return _BasicSubmissionBar(
      history: this.history,
      onSelect: this.onSelect,
      onSubmit: () async {
        this.completer.complete(this.getValue());
      },
      icon: this.icon,
      child: SizedBox.expand(
        child: this.content,
      ),
    );
  }

}

// ----------------

class _PauseSubmissionBar extends StatefulWidget {

  const _PauseSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _PauseSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;

}

class _PauseSubmissionBarState extends State<_PauseSubmissionBar> {

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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (null),
      history: const [],
      onSelect: (value) async {
      },
      icon: IconSymbols.pause,
      content: TextField(
        keyboardType: TextInputType.none,
        inputFormatters: const [],
        decoration: const InputDecoration(
          border: UnderlineInputBorder(),
          hintText: 'Pause ...',
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
  });

  @override
  createState() => _BooleanSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;

}

class _BooleanSubmissionBarState extends State<_BooleanSubmissionBar> {

  late Boolean?              _value;
  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._valueController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._value == null ? null : BooleanExpression(this._value!)),
      history: this.widget.history.cast<BooleanExpression>().map((e) => (e, true)).toList(),
      onSelect: (value) async {
        value as BooleanExpression;
        this._value = value.value;
        this._valueController.text = value.value == false ? 'n' : 'y';
        this.setState(() {});
      },
      icon: IconSymbols.check_box,
      content: TextField(
        keyboardType: TextInputType.none,
        inputFormatters: const [],
        decoration: InputDecoration(
          border: const UnderlineInputBorder(),
          hintText: 'Boolean ...',
          suffixIconConstraints: const BoxConstraints(),
          suffixIcon: CustomTextFieldSuffixWidget(
            children: [
              (this._value ?? !false) == false
                ? IconButton.filledTonal(
                  icon: const Icon(IconSymbols.do_not_disturb_on),
                  onPressed: () async {
                    this._value = null;
                    this._valueController.text = '';
                    this.setState(() {});
                  },
                )
                : IconButton(
                  icon: const Icon(IconSymbols.do_not_disturb_on),
                  onPressed: () async {
                    this._value = false;
                    this._valueController.text = 'n';
                    this.setState(() {});
                  },
                ),
              const SizedBox(width: 4),
              (this._value ?? !true) == true
                ? IconButton.filledTonal(
                  icon: const Icon(IconSymbols.check_circle),
                  onPressed: () async {
                    this._value = null;
                    this._valueController.text = '';
                    this.setState(() {});
                  },
                )
                : IconButton(
                  icon: const Icon(IconSymbols.check_circle),
                  onPressed: () async {
                    this._value = true;
                    this._valueController.text = 'y';
                    this.setState(() {});
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
        controller: this._valueController,
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
  });

  @override
  createState() => _IntegerSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;

}

class _IntegerSubmissionBarState extends State<_IntegerSubmissionBar> {

  late Integer?              _value;
  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._valueController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._value == null ? null : IntegerExpression(this._value!)),
      history: this.widget.history.cast<IntegerExpression>().map((e) => (e, true)).toList(),
      onSelect: (value) async {
        value as IntegerExpression;
        this._value = value.value;
        this._valueController.text = value.value.toString();
        this.setState(() {});
      },
      icon: IconSymbols.speed_1_2,
      content: TextField(
        keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: false),
        inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)$'))],
        decoration: const InputDecoration(
          border: UnderlineInputBorder(),
          hintText: 'Integer ...',
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
        onChanged: (value) async {
          this._value = value.isEmpty ? null : Integer.tryParse(value);
        },
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
  });

  @override
  createState() => _FloaterSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;

}

class _FloaterSubmissionBarState extends State<_FloaterSubmissionBar> {

  late Floater?              _value;
  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._valueController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._value == null ? null : FloaterExpression(this._value!)),
      history: this.widget.history.cast<FloaterExpression>().map((e) => (e, true)).toList(),
      onSelect: (value) async {
        value as FloaterExpression;
        this._value = value.value;
        this._valueController.text = value.value.toString();
        this.setState(() {});
      },
      icon: IconSymbols.speed_1_2,
      content: TextField(
        keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: true),
        inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)([.][\d]*)?$'))],
        decoration: const InputDecoration(
          border: UnderlineInputBorder(),
          hintText: 'Floater ...',
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
        onChanged: (value) async {
          this._value = value.isEmpty ? null : Floater.tryParse(value);
        },
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
  });

  @override
  createState() => _SizeSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;

}

class _SizeSubmissionBarState extends State<_SizeSubmissionBar> {

  late Floater?              _count;
  late Integer               _exponent;
  late TextEditingController _countController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._count = null;
    this._exponent = 2;
    this._countController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._count == null ? null : SizeExpression(this._count!, this._exponent)),
      history: this.widget.history.cast<SizeExpression>().map((e) => (e, true)).toList(),
      onSelect: (value) async {
        value as SizeExpression;
        this._count = value.count;
        this._exponent = value.exponent;
        this._countController.text = value.count.toString();
        this.setState(() {});
      },
      icon: IconSymbols.memory,
      content: TextField(
        keyboardType: const TextInputType.numberWithOptions(signed: false, decimal: true),
        inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([\d]*)([.][\d]*)?$'))],
        decoration: InputDecoration(
          border: const UnderlineInputBorder(),
          hintText: 'Size ...',
          suffixIconConstraints: const BoxConstraints(),
          suffixIcon: CustomTextFieldSuffixWidget(
            children: [
              PopupMenuButton(
                position: PopupMenuPosition.under,
                tooltip: '',
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
                  this.setState(() {});
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
        onChanged: (value) async {
          this._count = value.isEmpty ? null : Floater.tryParse(value);
        },
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
  });

  @override
  createState() => _StringSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;

}

class _StringSubmissionBarState extends State<_StringSubmissionBar> {

  late String?               _value;
  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._valueController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._value == null ? null : StringExpression(this._value!)),
      history: this.widget.history.cast<StringExpression>().map((e) => (e, true)).toList(),
      onSelect: (value) async {
        value as StringExpression;
        this._value = value.value;
        this._valueController.text = value.value;
        this.setState(() {});
      },
      icon: IconSymbols.text_fields,
      content: TextField(
        keyboardType: TextInputType.text,
        inputFormatters: const [],
        decoration: const InputDecoration(
          border: UnderlineInputBorder(),
          hintText: 'String ...',
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
        onChanged: (value) async {
          this._value = value.isEmpty ? null : value;
        },
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
  });

  @override
  createState() => _PathSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;

}

class _PathSubmissionBarState extends State<_PathSubmissionBar> {

  late String?               _content;
  late TextEditingController _contentController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._content = null;
    this._contentController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._content == null ? null : PathExpression(this._content!)),
      history: this.widget.history.cast<PathExpression>().map((e) => (e, true)).toList(),
      onSelect: (value) async {
        value as PathExpression;
        this._content = value.content;
        this._contentController.text = value.content;
        this.setState(() {});
      },
      icon: IconSymbols.link,
      content: Focus(
        onFocusChange: (value) async {
          if (!value) {
            if (this._content != null) {
              this._content = StorageHelper.regularize(this._content!);
              this._contentController.text = this._content!;
            }
          }
        },
        child: TextField(
          keyboardType: TextInputType.text,
          inputFormatters: const [],
          decoration: InputDecoration(
            border: const UnderlineInputBorder(),
            hintText: 'Path ...',
            suffixIconConstraints: const BoxConstraints(),
            suffixIcon: CustomTextFieldSuffixWidget(
              children: [
                PopupMenuButton(
                  position: PopupMenuPosition.under,
                  tooltip: '',
                  icon: const Icon(IconSymbols.adjust),
                  itemBuilder: (context) => <PopupMenuEntry<String>>[
                    PopupMenuItem(
                      value: '@open_file',
                      child: Text(
                        'Open File',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    PopupMenuItem(
                      value: '@open_directory',
                      child: Text(
                        'Open Directory',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    PopupMenuItem(
                      value: '@save_file',
                      child: Text(
                        'Save File',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    const PopupMenuDivider(
                    ),
                    PopupMenuItem(
                      value: ':g',
                      child: Text(
                        'Generate',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    PopupMenuItem(
                      value: ':m',
                      child: Text(
                        'Move',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    PopupMenuItem(
                      value: ':d',
                      child: Text(
                        'Delete',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    PopupMenuItem(
                      value: ':o',
                      child: Text(
                        'Overwrite',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                  ],
                  onSelected: (value) async {
                    if (value.startsWith(':')) {
                      this._content = value;
                    }
                    if (value.startsWith('@')) {
                      var target = switch (value.substring(1)) {
                        'open_file'      => await StorageHelper.pickOpenFile('ModdingWorker.Generic', setting.data.mFallbackDirectory),
                        'open_directory' => await StorageHelper.pickOpenDirectory('ModdingWorker.Generic'),
                        'save_file'      => await StorageHelper.pickSaveFile('ModdingWorker.Generic'),
                        _                => throw Exception(),
                      };
                      if (target != null) {
                        this._content = target;
                      }
                    }
                    this._contentController.text = this._content ?? '';
                    this.setState(() {});
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
          onChanged: (value) async {
            this._content = value.isEmpty ? null : value;
          },
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
    required this.history,
    required this.item,
  });

  @override
  createState() => _EnumerationSubmissionBarState();

  // ----------------

  final Completer<ValueExpression?> completer;
  final List<ValueExpression>       history;
  final List<String>                item;

}

class _EnumerationSubmissionBarState extends State<_EnumerationSubmissionBar> {

  late String?               _item;
  late TextEditingController _itemController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._item = null;
    this._itemController = TextEditingController();
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
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getValue: () => (this._item == null ? null : EnumerationExpression(this._item!)),
      history: this.widget.history.cast<EnumerationExpression>().map((e) => (e, this.widget.item.contains(e.item))).toList(),
      onSelect: (value) async {
        value as EnumerationExpression;
        this._item = value.item;
        this._itemController.text = value.item;
        this.setState(() {});
      },
      icon: IconSymbols.menu,
      content: LayoutBuilder(
        builder: (context, constraints) => MenuAnchor(
          crossAxisUnconstrained: false,
          alignmentOffset: const Offset(-12, 0),
          style: MenuStyle(
            minimumSize: WidgetStatePropertyAll(Size(constraints.maxWidth + 24, 0.0)),
            maximumSize: WidgetStatePropertyAll(Size(constraints.maxWidth + 24, Floater.infinity)),
            visualDensity: VisualDensity.standard,
          ),
          menuChildren: this.widget.item.map((value) => MenuItemButton(
            style: MenuItemButton.styleFrom(
              padding: const EdgeInsets.fromLTRB(12, 0, 12, 0),
              backgroundColor: value != this._item ? null : theme.colorScheme.onSurface.withOpacity(0.12),
            ),
            onPressed: () async {
              this._item = value;
              this._itemController.text = value;
              this.setState(() {});
            },
            child: ConstrainedBox(
              constraints: BoxConstraints(maxWidth: constraints.maxWidth - 24),
              child: Text(
                value,
                overflow: TextOverflow.clip,
                style: theme.textTheme.bodyLarge?.copyWith(
                  fontFamily: '',
                  fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                ),
              ),
            ),
          )).toList(),
          builder: (context, controller, child) => TextField(
            keyboardType: TextInputType.none,
            inputFormatters: const [],
            decoration: InputDecoration(
              border: const UnderlineInputBorder(),
              hintText: 'Enumeration ...',
              suffixIconConstraints: const BoxConstraints(),
              suffixIcon: CustomTextFieldSuffixWidget(
                children: [
                  IconButton(
                    icon: const Icon(IconSymbols.restart_alt),
                    onPressed: () async {
                      this._item = null;
                      this._itemController.text = '';
                      this.setState(() {});
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
              controller.open();
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
    required this.completer,
  });

  // ----------------

  final SubmissionType?              type;
  final List<String>?                option;
  final List<ValueExpression>?       history;
  final Completer<ValueExpression?>? completer;

  // ----------------

  @override
  build(context) {
    if (type == null) {
      assertTest(this.option == null && this.history == null && this.completer == null);
      return const _IdleSubmissionBar();
    }
    if (type == SubmissionType.pause) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _PauseSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.boolean) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _BooleanSubmissionBar(
        completer: this.completer!,
        history: this.history!,
      );
    }
    if (type == SubmissionType.integer) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _IntegerSubmissionBar(
        completer: this.completer!,
        history: this.history!,
      );
    }
    if (type == SubmissionType.floater) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _FloaterSubmissionBar(
        completer: this.completer!,
        history: this.history!,
      );
    }
    if (type == SubmissionType.size) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _SizeSubmissionBar(
        completer: this.completer!,
        history: this.history!,
      );
    }
    if (type == SubmissionType.string) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _StringSubmissionBar(
        completer: this.completer!,
        history: this.history!,
      );
    }
    if (type == SubmissionType.path) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length == 0);
      return _PathSubmissionBar(
        completer: this.completer!,
        history: this.history!,
      );
    }
    if (type == SubmissionType.enumeration) {
      assertTest(this.option != null && this.history != null && this.completer != null);
      assertTest(this.option!.length >= 0);
      var item = this.option!.sublist(0);
      return _EnumerationSubmissionBar(
        completer: this.completer!,
        history: this.history!,
        item: item,
      );
    }
    throw UnimplementedError();
  }

}
