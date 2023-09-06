// ignore_for_file: unused_import, unnecessary_cast

import '/common.dart';
import 'dart:io';
import 'dart:async';
import 'package:async/async.dart';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '/common/path_picker.dart';

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
  Widget build(BuildContext context) {
    return Row(
      children: [
        IgnorePointer(
          child: IconButton(
            icon: Icon(
              this.icon,
              size: 18,
            ),
            onPressed: () async {},
          ),
        ),
        const SizedBox(width: 4),
        Expanded(
          child: this.child ?? const SizedBox(),
        ),
        const SizedBox(width: 8),
        IconButton(
          icon: const Icon(Icons.done_outlined),
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

  @override
  Widget build(BuildContext context) {
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
  final Completer<dynamic> completer;
  final dynamic Function() getCompleterValue;

  // ----------------

  @override
  Widget build(BuildContext context) {
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
  State<PauseInputBarContent> createState() => _PauseInputBarContentState();

  // ----------------

  final Completer<dynamic> completer;

}

class _PauseInputBarContentState extends State<PauseInputBarContent> {

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => null,
      icon: Icons.pause,
      children: const [
        Text('Pause ...'),
      ],
    );
  }

}

// ----------------

class ConfirmationInputBarContent extends StatefulWidget {

  const ConfirmationInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  State<ConfirmationInputBarContent> createState() => _ConfirmationInputBarContentState();

  // ----------------

  final Completer<dynamic> completer;

}

class _ConfirmationInputBarContentState extends State<ConfirmationInputBarContent> {

  Boolean? _value;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.check_circle_outline_outlined,
      children: [
        ...[false].map((e) {
          return Expanded(
            child: (this._value ?? !e) == e
                ? FilledButton.tonal(
                    onPressed: () async {
                      this._value = null;
                      this.setState(() {});
                    },
                    child: Text(!e ? 'No' : 'Yes'),
                  )
                : TextButton(
                    onPressed: () async {
                      this._value = e;
                      this.setState(() {});
                    },
                    child: Text(!e ? 'No' : 'Yes'),
                  ),
          );
        }),
        const SizedBox(width: 8),
        ...[true].map((e) {
          return Expanded(
            child: (this._value ?? !e) == e
                ? FilledButton.tonal(
                    onPressed: () async {
                      this._value = null;
                      this.setState(() {});
                    },
                    child: Text(!e ? 'No' : 'Yes'),
                  )
                : TextButton(
                    onPressed: () async {
                      this._value = e;
                      this.setState(() {});
                    },
                    child: Text(!e ? 'No' : 'Yes'),
                  ),
          );
        }),
      ],
    );
  }

}

// ----------------

class NumberInputBarContent extends StatefulWidget {

  const NumberInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  State<NumberInputBarContent> createState() => _NumberInputBarContentState();

  // ----------------

  final Completer<Floater?> completer;

}

class _NumberInputBarContentState extends State<NumberInputBarContent> {

  Floater? _value;

  final TextEditingController _controller = TextEditingController(text: '');

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.numbers,
      children: [
        Expanded(
          child: TextField(
            keyboardType: const TextInputType.numberWithOptions(
              signed: true,
              decimal: true,
            ),
            inputFormatters: [
              FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)([.][\d]*)?$')),
            ],
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Number ...',
            ),
            controller: this._controller,
            onChanged: (value) {
              try {
                this._value = Floater.parse(value);
              } catch (e) {
                this._value = null;
              }
              this.setState(() {});
            },
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
  State<IntegerInputBarContent> createState() => _IntegerInputBarContentState();

  // ----------------

  final Completer<Integer?> completer;

}

class _IntegerInputBarContentState extends State<IntegerInputBarContent> {

  Integer? _value;

  final TextEditingController _controller = TextEditingController(text: '');

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.numbers,
      children: [
        Expanded(
          child: TextField(
            keyboardType: const TextInputType.numberWithOptions(
              signed: true,
              decimal: false,
            ),
            inputFormatters: [
              FilteringTextInputFormatter.allow(RegExp(r'^([+-])?([\d]*)$')),
            ],
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Integer ...',
            ),
            controller: this._controller,
            onChanged: (value) {
              try {
                this._value = Integer.parse(value);
              } catch (e) {
                this._value = null;
              }
              this.setState(() {});
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
  State<SizeInputBarContent> createState() => _SizeInputBarContentState();

  // ----------------

  final Completer<String?> completer;

}

class _SizeInputBarContentState extends State<SizeInputBarContent> {

  Floater? _value;
  Integer  _unit = 3;

  final TextEditingController _controller = TextEditingController(text: '');

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value == null ? null : '${this._value!}${['', 'b', 'k', 'm', 'g'][this._unit]}',
      icon: Icons.memory_outlined,
      children: [
        Expanded(
          child: TextField(
            keyboardType: const TextInputType.numberWithOptions(
              signed: false,
              decimal: false,
            ),
            inputFormatters: [
              FilteringTextInputFormatter.allow(RegExp(r'^([\d]*)([.][\d]*)?$')),
            ],
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Size ...',
            ),
            controller: this._controller,
            onChanged: (value) {
              try {
                this._value = Floater.parse(value);
              } catch (e) {
                this._value = null;
              }
              this.setState(() {});
            },
          ),
        ),
        SizedBox(
          width: 48,
          child: DropdownButton<Integer>(
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
  State<StringInputBarContent> createState() => _StringInputBarContentState();

  // ----------------

  final Completer<String?> completer;

}

class _StringInputBarContentState extends State<StringInputBarContent> {

  String? _value;

  final TextEditingController _controller = TextEditingController(text: '');

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.text_fields,
      children: [
        Expanded(
          child: TextField(
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'String ...',
            ),
            controller: this._controller,
            onChanged: (value) {
              this._value = value.isEmpty ? null : value;
              this.setState(() {});
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
    required this.type,
    required this.rule,
  });

  @override
  State<PathInputBarContent> createState() => _PathInputBarContentState();

  // ----------------

  final Completer<String?> completer;
  final FileObjectType     type;
  final FileRequestRule    rule;

}

class _PathInputBarContentState extends State<PathInputBarContent> {

  String? _value;

  final TextEditingController _controller = TextEditingController(text: '');

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.text_fields,
      children: [
        Expanded(
          child: TextField(
            decoration: const InputDecoration(
              border: InputBorder.none,
              hintText: 'Path ...',
            ),
            controller: this._controller,
            onChanged: (value) {
              this._value = value.isEmpty ? null : value;
              this.setState(() {});
            },
          ),
        ),
        !(this.widget.rule == FileRequestRule.output)
        ? const SizedBox()
        : PopupMenuButton(
          offset: const Offset(0, -112),
          icon: const Icon(Icons.adjust_outlined),
          tooltip: '',
          itemBuilder: (context) => [
            const PopupMenuItem(
              value: ':o',
              child: Text('override'),
            ),
            const PopupMenuItem(
              value: ':d',
              child: Text('delete'),
            ),
            const PopupMenuItem(
              value: ':t',
              child: Text('trash'),
            ),
          ],
          onSelected: (value) async {
            this._value = value;
            this._controller.text = this._value ?? '';
            this.setState(() {});
          },
        ),
        PopupMenuButton(
          offset: const Offset(0, -64),
          icon: const Icon(Icons.outbond_outlined),
          tooltip: '',
          itemBuilder: (context) => [
            const PopupMenuItem(
              value: FileObjectType.file,
              child: Text('file'),
            ),
            const PopupMenuItem(
              value: FileObjectType.directory,
              child: Text('directory'),
            ),
          ],
          onSelected: (value) async {
            var selection = null as String?;
            if (value == FileObjectType.file) {
              selection = await PathPicker.pickFile();
            }
            if (value == FileObjectType.directory) {
              selection = await PathPicker.pickDirectory();
            }
            if (selection != null) {
              this._value = selection;
              this._controller.text = this._value ?? '';
            }
            this.setState(() {});
          },
        ),
      ],
    );
  }

}

// ----------------

class OptionInputBarContent extends StatefulWidget {

  const OptionInputBarContent({
    super.key,
    required this.completer,
    required this.option,
  });

  @override
  State<OptionInputBarContent> createState() => _OptionInputBarContentState();

  // ----------------

  final Completer<Integer?> completer;
  final List<String>        option;

}

class _OptionInputBarContentState extends State<OptionInputBarContent> {

  Integer? _value;

  // ----------------

  @override
  Widget build(BuildContext context) {
    List<DropdownMenuItem<Integer>> itemList = [];
    itemList.add(const DropdownMenuItem(
      value: -1,
      child: Text(''),
    ));
    for (var i = 0; i < this.widget.option.length; ++i) {
      itemList.add(DropdownMenuItem(
        value: i + 1,
        child: Text(this.widget.option[i]),
      ));
    }
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.menu,
      children: [
        Expanded(
          child: DropdownButton<Integer>(
            value: this._value,
            hint: const Text('Option ...'),
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
