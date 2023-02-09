// ignore_for_file: unused_import

import '/common.dart';
import 'dart:async';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';

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
            onPressed: () {},
          ),
        ),
        const SizedBox(width: 4),
        Expanded(
          child: this.child ?? const SizedBox(),
        ),
        const SizedBox(width: 8),
        IconButton(
          icon: const Icon(Icons.done),
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
    required this.child,
    required this.completer,
    required this.getCompleterValue,
  });

  // ----------------

  final IconData           icon;
  final Widget             child;
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
      child: this.child,
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
      child: const Text('响应以继续 ...'),
    );
  }

}

// ----------------

class ConfirmInputBarContent extends StatefulWidget {

  const ConfirmInputBarContent({
    super.key,
    required this.completer,
  });

  @override
  State<ConfirmInputBarContent> createState() => _ConfirmInputBarContentState();

  // ----------------

  final Completer<dynamic> completer;

}

class _ConfirmInputBarContentState extends State<ConfirmInputBarContent> {

  Boolean? _value;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.check_circle_outline_outlined,
      child: Row(
        children: [
          ...[false].map((e) {
            return Expanded(
              child: (this._value ?? !e) == e
                  ? FilledButton.tonal(
                      onPressed: () {
                        this._value = null;
                        this.setState(() {});
                      },
                      child: Text(!e ? '否' : '是'),
                    )
                  : OutlinedButton(
                      onPressed: () {
                        this._value = e;
                        this.setState(() {});
                      },
                      child: Text(!e ? '否' : '是'),
                    ),
            );
          }),
          const SizedBox(width: 8),
          ...[true].map((e) {
            return Expanded(
              child: (this._value ?? !e) == e
                  ? FilledButton.tonal(
                      onPressed: () {
                        this._value = null;
                        this.setState(() {});
                      },
                      child: Text(!e ? '否' : '是'),
                    )
                  : OutlinedButton(
                      onPressed: () {
                        this._value = e;
                        this.setState(() {});
                      },
                      child: Text(!e ? '否' : '是'),
                    ),
            );
          }),
        ],
      ),
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

  final Completer<Floating?> completer;

}

class _NumberInputBarContentState extends State<NumberInputBarContent> {

  Floating? _value;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.numbers,
      child: TextField(
        keyboardType: const TextInputType.numberWithOptions(
          signed: true,
          decimal: true,
        ),
        inputFormatters: [
          FilteringTextInputFormatter.allow(RegExp(r'^(\+|\-)?(\d*)(\.\d*)?$')),
        ],
        decoration: const InputDecoration(
          border: InputBorder.none,
          hintText: '请输入数字',
        ),
        onChanged: (value) {
          try {
            this._value = Floating.parse(value);
          } catch (e) {
            this._value = null;
          }
          this.setState(() {});
        },
      ),
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

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.numbers,
      child: TextField(
        keyboardType: const TextInputType.numberWithOptions(
          signed: true,
          decimal: false,
        ),
        inputFormatters: [
          FilteringTextInputFormatter.allow(RegExp(r'^(\+|\-)?(\d*)$')),
        ],
        decoration: const InputDecoration(
          border: InputBorder.none,
          hintText: '请输入整数',
        ),
        onChanged: (value) {
          try {
            this._value = Integer.parse(value);
          } catch (e) {
            this._value = null;
          }
          this.setState(() {});
        },
      ),
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

  Floating? _value;
  Integer   _unit = 3;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value == null ? null : '${this._value!}${['', 'b', 'k', 'm', 'g'][this._unit]}',
      icon: Icons.memory_outlined,
      child: Row(
        children: [
          Expanded(
            child: TextField(
              keyboardType: const TextInputType.numberWithOptions(
                signed: false,
                decimal: false,
              ),
              inputFormatters: [
                FilteringTextInputFormatter.allow(RegExp(r'^(\d*)(\.\d*)?$')),
              ],
              decoration: const InputDecoration(
                border: InputBorder.none,
                hintText: '请输入尺寸',
              ),
              onChanged: (value) {
                try {
                  this._value = Floating.parse(value);
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
      ),
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

  // ----------------

  @override
  Widget build(BuildContext context) {
    return BusyInputBarContent(
      completer: this.widget.completer,
      getCompleterValue: () => this._value,
      icon: Icons.text_fields,
      child: TextField(
        decoration: const InputDecoration(
          border: InputBorder.none,
          hintText: '请输入字符串',
        ),
        onChanged: (value) {
          this._value = value.isEmpty ? null : value;
          this.setState(() {});
        },
      ),
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
      child: DropdownButton<Integer>(
        value: this._value,
        hint: const Text('请选择一项'),
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
    );
  }

}
