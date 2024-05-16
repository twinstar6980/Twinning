import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import '/view/modding_worker/submission_type.dart';
import 'dart:async';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class _BasicSubmissionBar extends StatelessWidget {

  const _BasicSubmissionBar({
    super.key, // ignore: unused_element
    required this.icon,
    required this.child,
    required this.onSubmit,
  });

  // ----------------

  final IconData         icon;
  final Widget?          child;
  final Void Function()? onSubmit;

  // ----------------

  @override
  build(context) {
    return Row(
      children: [
        SizedBox(
          width: 56,
          child: IconButton.filledTonal(
            icon: Icon(this.icon),
            onPressed: () async {
              // TODO : recent value
            },
          ),
        ),
        const SizedBox(width: 16),
        Expanded(
          child: this.child ?? const SizedBox(),
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
      icon: IconSymbols.coffee,
      onSubmit: null,
      child: null,
    );
  }

}

class _BusySubmissionBar extends StatelessWidget {

  const _BusySubmissionBar({
    super.key, // ignore: unused_element
    required this.icon,
    required this.children,
    required this.completer,
    required this.getCompleterValue,
  });

  // ----------------

  final IconData           icon;
  final List<Widget>       children;
  final Completer<Object?> completer;
  final Object? Function() getCompleterValue;

  // ----------------

  @override
  build(context) {
    return _BasicSubmissionBar(
      icon: this.icon,
      onSubmit: () async {
        this.completer.complete(this.getCompleterValue());
      },
      child: Row(
        children: this.children,
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

  final Completer<Object?> completer;

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
      getCompleterValue: () => (null),
      icon: IconSymbols.pause,
      children: [
        Expanded(
          child: Text(
            'Pause ...',
            overflow: TextOverflow.ellipsis,
            style: theme.textTheme.bodyLarge?.copyWith(
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
              color: theme.colorScheme.secondary,
            ),
          ),
        ),
      ],
    );
  }

}

// ----------------

class _BooleanSubmissionBar extends StatefulWidget {

  const _BooleanSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _BooleanSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;

}

class _BooleanSubmissionBarState extends State<_BooleanSubmissionBar> {

  late Boolean? _value;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
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
      getCompleterValue: () => (this._value),
      icon: IconSymbols.task_alt,
      children: [
        Expanded(
          child: (this._value ?? !false) == false
            ? FilledButton.tonal(
              style: FilledButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy])),
              onPressed: () async {
                this._value = null;
                this.setState(() {});
              },
              child: const Text('No'),
            )
            : TextButton(
              style: TextButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy])),
              onPressed: () async {
                this._value = false;
                this.setState(() {});
              },
              child: const Text('No'),
            ),
        ),
        const SizedBox(width: 8),
        Expanded(
          child: (this._value ?? !true) == true
            ? FilledButton.tonal(
              style: FilledButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy])),
              onPressed: () async {
                this._value = null;
                this.setState(() {});
              },
              child: const Text('Yes'),
            )
            : TextButton(
              style: TextButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy])),
              onPressed: () async {
                this._value = true;
                this.setState(() {});
              },
              child: const Text('Yes'),
            ),
        ),
      ],
    );
  }

}

// ----------------

class _IntegerSubmissionBar extends StatefulWidget {

  const _IntegerSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _IntegerSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;

}

class _IntegerSubmissionBarState extends State<_IntegerSubmissionBar> {

  late Integer?              _value;
  late TextEditingController _controller;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._controller = TextEditingController();
    return;
  }

  @override
  dispose() {
    this._controller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: IconSymbols.speed_1_2,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Integer ...',
            ),
            controller: this._controller,
            keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: false),
            inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)$'))],
            onChanged: (value) async {
              this._value = value.isEmpty ? null : Integer.tryParse(value);
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class _FloaterSubmissionBar extends StatefulWidget {

  const _FloaterSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _FloaterSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;

}

class _FloaterSubmissionBarState extends State<_FloaterSubmissionBar> {

  late Floater?              _value;
  late TextEditingController _controller;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._controller = TextEditingController();
    return;
  }

  @override
  dispose() {
    this._controller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: IconSymbols.speed_1_2,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Floater ...',
            ),
            controller: this._controller,
            keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: true),
            inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)([.][\d]*)?$'))],
            onChanged: (value) async {
              this._value = value.isEmpty ? null : Floater.tryParse(value);
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class _SizeSubmissionBar extends StatefulWidget {

  const _SizeSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _SizeSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;

}

class _SizeSubmissionBarState extends State<_SizeSubmissionBar> {

  late Floater?              _value;
  late Integer               _unit;
  late TextEditingController _controller;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._unit = 3;
    this._controller = TextEditingController();
    return;
  }

  @override
  dispose() {
    this._controller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value == null ? null : '${this._value!}${['', 'b', 'k', 'm', 'g'][this._unit]}'),
      icon: IconSymbols.memory,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
            decoration: InputDecoration(
              border: const UnderlineInputBorder(),
              hintText: 'Size ...',
              suffixIcon: SizedBox(
                width: 48,
                child: DropdownButton<Integer>(
                  style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
                  value: this._unit,
                  items: const [
                    DropdownMenuItem(
                      value: 1,
                      child: Center(child: Text('b')),
                    ),
                    DropdownMenuItem(
                      value: 2,
                      child: Center(child: Text('k')),
                    ),
                    DropdownMenuItem(
                      value: 3,
                      child: Center(child: Text('m')),
                    ),
                    DropdownMenuItem(
                      value: 4,
                      child: Center(child: Text('g')),
                    ),
                  ],
                  underline: Container(),
                  alignment: AlignmentDirectional.center,
                  isExpanded: true,
                  borderRadius: const BorderRadius.all(Radius.circular(10)),
                  onChanged: (value) async {
                    value!;
                    this._unit = value;
                    this.setState(() {});
                  },
                ),
              ),
            ),
            controller: this._controller,
            keyboardType: const TextInputType.numberWithOptions(signed: false, decimal: true),
            inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([\d]*)([.][\d]*)?$'))],
            onChanged: (value) async {
              this._value = value.isEmpty ? null : Floater.tryParse(value);
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class _StringSubmissionBar extends StatefulWidget {

  const _StringSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _StringSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;

}

class _StringSubmissionBarState extends State<_StringSubmissionBar> {

  late String?               _value;
  late TextEditingController _controller;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._controller = TextEditingController();
    return;
  }

  @override
  dispose() {
    this._controller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: IconSymbols.text_fields,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'String ...',
            ),
            controller: this._controller,
            keyboardType: TextInputType.text,
            inputFormatters: const [],
            onChanged: (value) async {
              this._value = value.isEmpty ? null : value;
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class _PathSubmissionBar extends StatefulWidget {

  const _PathSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
  });

  @override
  createState() => _PathSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;

}

class _PathSubmissionBarState extends State<_PathSubmissionBar> {

  late String?               _value;
  late TextEditingController _controller;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
    this._controller = TextEditingController();
    return;
  }

  @override
  dispose() {
    this._controller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: IconSymbols.link,
      children: [
        Expanded(
          child: Focus(
            onFocusChange: (value) async {
              if (!value) {
                if (this._value != null) {
                  this._value = StorageHelper.regularize(this._value!);
                  this._controller.text = this._value!;
                }
              }
            },
            child: TextField(
              style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
              decoration: InputDecoration(
                border: const UnderlineInputBorder(),
                hintText: 'Path ...',
                suffixIcon: PopupMenuButton(
                  offset: const Offset(0, -316),
                  icon: const Icon(IconSymbols.adjust),
                  tooltip: '',
                  itemBuilder: (context) => <PopupMenuEntry<String>>[
                    PopupMenuItem(
                      value: ':g',
                      child: Text(
                        'Generate',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
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
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
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
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
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
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                    const PopupMenuDivider(
                    ),
                    PopupMenuItem(
                      value: '@open_file',
                      child: Text(
                        'Open File',
                        overflow: TextOverflow.ellipsis,
                        style: theme.textTheme.labelLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
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
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
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
                          fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
                        ),
                      ),
                    ),
                  ],
                  onSelected: (value) async {
                    if (value.startsWith(':')) {
                      this._value = value;
                    }
                    if (value.startsWith('@')) {
                      var selection = null as String?;
                      if (value.substring(1) == 'open_file') {
                        selection = await StorageHelper.pickOpenFile('ModdingWorker.Generic', setting.data.mFallbackDirectory);
                      }
                      if (value.substring(1) == 'open_directory') {
                        selection = await StorageHelper.pickOpenDirectory('ModdingWorker.Generic');
                      }
                      if (value.substring(1) == 'save_file') {
                        selection = await StorageHelper.pickSaveFile('ModdingWorker.Generic');
                      }
                      if (selection != null) {
                        this._value = selection;
                      }
                    }
                    this._controller.text = this._value ?? '';
                    this.setState(() {});
                  },
                ),
              ),
              controller: this._controller,
              keyboardType: TextInputType.text,
              inputFormatters: const [],
              onChanged: (value) async {
                this._value = value.isEmpty ? null : value;
              },
            ),
          ),
        ),
      ],
    );
  }

}

// ----------------

class _EnumerationSubmissionBar extends StatefulWidget {

  const _EnumerationSubmissionBar({
    super.key, // ignore: unused_element
    required this.completer,
    required this.item,
  });

  @override
  createState() => _EnumerationSubmissionBarState();

  // ----------------

  final Completer<Object?> completer;
  final List<String>       item;

}

class _EnumerationSubmissionBarState extends State<_EnumerationSubmissionBar> {

  late Integer? _value;

  // ----------------

  @override
  initState() {
    super.initState();
    this._value = null;
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
    var itemList = <DropdownMenuItem<Integer>>[];
    itemList.add(const DropdownMenuItem(
      value: -1,
      child: Text(''),
    ));
    for (var index = 0; index < this.widget.item.length; index++) {
      itemList.add(DropdownMenuItem(
        value: index + 1,
        child: Text(this.widget.item[index]),
      ));
    }
    return _BusySubmissionBar(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: IconSymbols.menu,
      children: [
        Expanded(
          child: DropdownButton<Integer>(
            style: theme.textTheme.bodyLarge?.copyWith(
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy],
              overflow: TextOverflow.clip,
            ),
            value: this._value,
            hint: const Text(
              'Enumeration ...',
              overflow: TextOverflow.ellipsis,
            ),
            items: itemList,
            isExpanded: true,
            borderRadius: const BorderRadius.all(Radius.circular(10)),
            onChanged: (value) async {
              value!;
              this._value = value == -1 ? null : value;
              this.setState(() {});
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class SubmissionBar extends StatelessWidget {

  const SubmissionBar({
    super.key,
    required this.type,
    required this.option,
    required this.completer,
  });

  // ----------------

  final SubmissionType?     type;
  final List<String>        option;
  final Completer<Object?>? completer;

  // ----------------

  @override
  build(context) {
    if (type == null) {
      assertTest(this.option.length == 0);
      assertTest(this.completer == null);
      return const _IdleSubmissionBar();
    }
    if (type == SubmissionType.pause) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _PauseSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.boolean) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _BooleanSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.integer) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _IntegerSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.floater) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _FloaterSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.size) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _SizeSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.string) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _StringSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.path) {
      assertTest(this.option.length == 0);
      assertTest(this.completer != null);
      return _PathSubmissionBar(
        completer: this.completer!,
      );
    }
    if (type == SubmissionType.enumeration) {
      assertTest(this.option.length >= 0);
      assertTest(this.completer != null);
      var option = this.option.sublist(0);
      return _EnumerationSubmissionBar(
        completer: this.completer!,
        item: option,
      );
    }
    throw UnimplementedError();
  }

}
