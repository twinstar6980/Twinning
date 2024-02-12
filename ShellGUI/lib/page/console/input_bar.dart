import '/common.dart';
import '/utility/path_picker.dart';
import '/setting.dart';
import 'dart:async';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class InputBarContent extends StatelessWidget {

  const InputBarContent({
    super.key,
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
        IgnorePointer(
          child: IconButton(
            icon: Icon(
              this.icon,
              size: 18,
            ),
            onPressed: () async {
            },
          ),
        ),
        const SizedBox(width: 4),
        Expanded(
          child: this.child ?? const SizedBox(),
        ),
        const SizedBox(width: 8),
        IconButton(
          icon: const Icon(Icons.send_outlined),
          onPressed: this.onSubmit,
        ),
      ],
    );
  }

}

class IdleInputBarContent extends StatelessWidget {

  const IdleInputBarContent({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return const InputBarContent(
      icon: Icons.coffee_outlined,
      onSubmit: null,
      child: null,
    );
  }

}

class BusyInputBarContent extends StatelessWidget {

  const BusyInputBarContent({
    super.key,
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
    return InputBarContent(
      icon: this.icon,
      onSubmit: () {
        this.completer.complete(this.getCompleterValue());
      },
      child: Row(
        children: this.children,
      ),
    );
  }

}

// ----------------

class PauseInputBarContent extends StatefulWidget {

  const PauseInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _PauseInputBarContentState();

  // ----------------

  final Completer<Object?> completer;

}

class _PauseInputBarContentState extends State<PauseInputBarContent> {

  // ----------------

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (null),
      icon: Icons.pause,
      children: [
        Text(
          'Pause ...',
          style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
        ),
      ],
    );
  }

}

// ----------------

class BooleanInputBarContent extends StatefulWidget {

  const BooleanInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _BooleanInputBarContentState();

  // ----------------

  final Completer<Boolean?> completer;

}

class _BooleanInputBarContentState extends State<BooleanInputBarContent> {

  Boolean? _value = null;

  // ----------------

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: Icons.check_circle_outline_outlined,
      children: [
        ...[false].map(
          (value) => Expanded(
            child: (this._value ?? !value) == value
              ? FilledButton.tonal(
                style: FilledButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy])),
                onPressed: () async {
                  this._value = null;
                  this.setState(() {});
                },
                child: Text(!value ? 'No' : 'Yes'),
              )
              : TextButton(
                style: TextButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy])),
                onPressed: () async {
                  this._value = value;
                  this.setState(() {});
                },
                child: Text(!value ? 'No' : 'Yes'),
              ),
          ),
        ),
        const SizedBox(width: 8),
        ...[true].map(
          (value) => Expanded(
            child: (this._value ?? !value) == value
              ? FilledButton.tonal(
                style: FilledButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy])),
                onPressed: () async {
                  this._value = null;
                  this.setState(() {});
                },
                child: Text(!value ? 'No' : 'Yes'),
              )
              : TextButton(
                style: TextButton.styleFrom(textStyle: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy])),
                onPressed: () async {
                  this._value = value;
                  this.setState(() {});
                },
                child: Text(!value ? 'No' : 'Yes'),
              ),
          ),
        ),
      ],
    );
  }

}

// ----------------

class IntegerInputBarContent extends StatefulWidget {

  const IntegerInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _IntegerInputBarContentState();

  // ----------------

  final Completer<Integer?> completer;

}

class _IntegerInputBarContentState extends State<IntegerInputBarContent> {

  Integer?              _value = null;
  TextEditingController _controller = TextEditingController(text: '');

  // ----------------

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
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: Icons.numbers,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Integer ...',
            ),
            controller: this._controller,
            keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: false),
            inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)$'))],
            onChanged: (value) {
              this._value = value.isEmpty ? null : Integer.tryParse(value);
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class FloaterInputBarContent extends StatefulWidget {

  const FloaterInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _FloaterInputBarContentState();

  // ----------------

  final Completer<Floater?> completer;

}

class _FloaterInputBarContentState extends State<FloaterInputBarContent> {

  Floater?              _value = null;
  TextEditingController _controller = TextEditingController(text: '');

  // ----------------

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
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: Icons.numbers,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Floater ...',
            ),
            controller: this._controller,
            keyboardType: const TextInputType.numberWithOptions(signed: true, decimal: true),
            inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)([.][\d]*)?$'))],
            onChanged: (value) {
              this._value = value.isEmpty ? null : Floater.tryParse(value);
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class SizeInputBarContent extends StatefulWidget {

  const SizeInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _SizeInputBarContentState();

  // ----------------

  final Completer<String?> completer;

}

class _SizeInputBarContentState extends State<SizeInputBarContent> {

  Floater?              _value = null;
  Integer               _unit  = 3;
  TextEditingController _controller = TextEditingController(text: '');

  // ----------------

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
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value == null ? null : '${this._value!}${['', 'b', 'k', 'm', 'g'][this._unit]}'),
      icon: Icons.memory_outlined,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Size ...',
            ),
            controller: this._controller,
            keyboardType: const TextInputType.numberWithOptions(signed: false, decimal: true),
            inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'^([\d]*)([.][\d]*)?$'))],
            onChanged: (value) {
              this._value = value.isEmpty ? null : Floater.tryParse(value);
            },
          ),
        ),
        SizedBox(
          width: 48,
          child: DropdownButton<Integer>(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
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
            onChanged: (value) {
              value!;
              this._unit = value;
              this.setState(() {});
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class StringInputBarContent extends StatefulWidget {

  const StringInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _StringInputBarContentState();

  // ----------------

  final Completer<String?> completer;

}

class _StringInputBarContentState extends State<StringInputBarContent> {

  String?               _value = null;
  TextEditingController _controller = TextEditingController(text: '');

  // ----------------

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
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: Icons.text_fields,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'String ...',
            ),
            controller: this._controller,
            keyboardType: TextInputType.text,
            inputFormatters: const [],
            onChanged: (value) {
              this._value = value.isEmpty ? null : value;
            },
          ),
        ),
      ],
    );
  }

}

// ----------------

class PathInputBarContent extends StatefulWidget {

  const PathInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  createState() => _PathInputBarContentState();

  // ----------------

  final Completer<String?> completer;

}

class _PathInputBarContentState extends State<PathInputBarContent> {

  String?               _value = null;
  TextEditingController _controller = TextEditingController(text: '');

  // ----------------

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
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: Icons.link_outlined,
      children: [
        Expanded(
          child: TextField(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Path ...',
            ),
            controller: this._controller,
            keyboardType: TextInputType.text,
            inputFormatters: const [],
            onChanged: (value) {
              this._value = value.isEmpty ? null : value;
            },
          ),
        ),
        PopupMenuButton(
          offset: const Offset(0, -272),
          icon: const Icon(Icons.adjust_outlined),
          tooltip: '',
          itemBuilder: (context) => <PopupMenuEntry>[
            PopupMenuItem(
              value: ':g',
              child: Text(
                'Generate',
                style: theme.textTheme.labelLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
              ),
            ),
            PopupMenuItem(
              value: ':m',
              child: Text(
                'Move',
                style: theme.textTheme.labelLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
              ),
            ),
            PopupMenuItem(
              value: ':d',
              child: Text(
                'Delete',
                style: theme.textTheme.labelLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
              ),
            ),
            PopupMenuItem(
              value: ':o',
              child: Text(
                'Overwrite',
                style: theme.textTheme.labelLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
              ),
            ),
            const PopupMenuDivider(
            ),
            PopupMenuItem(
              value: FileObjectType.file,
              child: Text(
                'File',
                style: theme.textTheme.labelLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
              ),
            ),
            PopupMenuItem(
              value: FileObjectType.directory,
              child: Text(
                'Directory',
                style: theme.textTheme.labelLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
              ),
            ),
          ],
          onSelected: (value) async {
            if (value is String) {
              this._value = value;
            }
            if (value is FileObjectType) {
              var selection = null as String?;
              if (value == FileObjectType.file) {
                selection = await PathPicker.pickFile(setting.data.mFallbackDirectory);
              }
              if (value == FileObjectType.directory) {
                selection = await PathPicker.pickDirectory();
              }
              if (selection != null) {
                this._value = selection;
              }
            }
            this._controller.text = this._value ?? '';
            this.setState(() {});
          },
        ),
      ],
    );
  }

}

// ----------------

class EnumerationInputBarContent extends StatefulWidget {

  const EnumerationInputBarContent({
    super.key,
    required this.completer,
    required this.option,
  });

  @override
  createState() => _EnumerationInputBarContentState();

  // ----------------

  final Completer<Integer?> completer;
  final List<String>        option;

}

class _EnumerationInputBarContentState extends State<EnumerationInputBarContent> {

  Integer? _value = null;

  // ----------------

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
    for (var index = 0; index < this.widget.option.length; index++) {
      itemList.add(DropdownMenuItem(
        value: index + 1,
        child: Text(this.widget.option[index]),
      ));
    }
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => (this._value),
      icon: Icons.menu,
      children: [
        Expanded(
          child: DropdownButton<Integer>(
            style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy]),
            value: this._value,
            hint: const Text('Enumeration ...'),
            items: itemList,
            underline: Container(),
            isExpanded: true,
            borderRadius: const BorderRadius.all(Radius.circular(10)),
            onChanged: (value) {
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
