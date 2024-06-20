import '/common.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import '/view/command_sender/configuration.dart';
import '/view/command_sender/value_expression.dart';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';

// ----------------

class _BasicArgumentBar extends StatelessWidget {

  const _BasicArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.icon,
    required this.content,
  });

  // ----------------

  final String   name;
  final IconData icon;
  final Widget   content;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Column(
      children: [
        Row(
          children: [
            Expanded(
              child: Text(
                this.name,
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.labelMedium?.copyWith(
                  color: theme.colorScheme.primary,
                ),
              ),
            ),
          ],
        ),
        this.content,
      ],
    );
  }

}

// ----------------

class _BooleanArgumentBar extends StatefulWidget {

  const _BooleanArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.value,
  });

  @override
  createState() => _BooleanArgumentBarState();

  // ----------------

  final String                      name;
  final Wrapper<BooleanExpression?> value;

}

class _BooleanArgumentBarState extends State<_BooleanArgumentBar> {

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
    return _BasicArgumentBar(
      name: this.widget.name,
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
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _IntegerArgumentBar extends StatefulWidget {

  const _IntegerArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.value,
  });

  @override
  createState() => _IntegerArgumentBarState();

  // ----------------

  final String                      name;
  final Wrapper<IntegerExpression?> value;

}

class _IntegerArgumentBarState extends State<_IntegerArgumentBar> {

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
    return _BasicArgumentBar(
      name: this.widget.name,
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
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _FloaterArgumentBar extends StatefulWidget {

  const _FloaterArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.value,
  });

  @override
  createState() => _FloaterArgumentBarState();

  // ----------------

  final String                      name;
  final Wrapper<FloaterExpression?> value;

}

class _FloaterArgumentBarState extends State<_FloaterArgumentBar> {

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
    return _BasicArgumentBar(
      name: this.widget.name,
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
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _SizeArgumentBar extends StatefulWidget {

  const _SizeArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.value,
  });

  @override
  createState() => _SizeArgumentBarState();

  // ----------------

  final String                   name;
  final Wrapper<SizeExpression?> value;

}

class _SizeArgumentBarState extends State<_SizeArgumentBar> {

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
    return _BasicArgumentBar(
      name: this.widget.name,
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
                  ),
                  itemBuilder: (context) => ['B', 'K', 'M', 'G'].mapIndexed((index, value) => PopupMenuItem(
                    value: index,
                    child: Text(
                      value,
                      overflow: TextOverflow.ellipsis,
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
          controller: this._countController,
        ),
      ),
    );
  }

}

// ----------------

class _StringArgumentBar extends StatefulWidget {

  const _StringArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.value,
  });

  @override
  createState() => _StringArgumentBarState();

  // ----------------

  final String                     name;
  final Wrapper<StringExpression?> value;

}

class _StringArgumentBarState extends State<_StringArgumentBar> {

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
    return _BasicArgumentBar(
      name: this.widget.name,
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
          controller: this._valueController,
        ),
      ),
    );
  }

}

// ----------------

class _PathArgumentBar extends StatefulWidget {

  const _PathArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.value,
  });

  @override
  createState() => _PathArgumentBarState();

  // ----------------

  final String                   name;
  final Wrapper<PathExpression?> value;

}

class _PathArgumentBarState extends State<_PathArgumentBar> {

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
    return _BasicArgumentBar(
      name: this.widget.name,
      icon: IconSymbols.link,
      content: Focus(
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
          controller: this._contentController,
        ),
      ),
    );
  }

}

// ----------------

class _EnumerationArgumentBar extends StatefulWidget {

  const _EnumerationArgumentBar({
    super.key, // ignore: unused_element
    required this.name,
    required this.option,
    required this.value,
  });

  @override
  createState() => _EnumerationArgumentBarState();

  // ----------------

  final String                    name;
  final List<ValueExpression>     option;
  final Wrapper<ValueExpression?> value;

}

class _EnumerationArgumentBarState extends State<_EnumerationArgumentBar> {

  late TextEditingController _itemController;

  Void _refresh(
  ) async {
    this._itemController.text = this.widget.value.value == null ? '' : ValueExpressionHelper.makeString(this.widget.value.value!);
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
    var theme = Theme.of(context);
    return _BasicArgumentBar(
      name: this.widget.name,
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
              backgroundColor: WidgetStatePropertyAll(this.widget.value.value == null || ValueExpressionHelper.makeString(value) != ValueExpressionHelper.makeString(this.widget.value.value!) ? null : theme.colorScheme.onSurface.withOpacity(0.12)),
            ),
            onPressed: () async {
              this.widget.value.value = value;
              this._refresh();
            },
            child: ConstrainedBox(
              constraints: BoxConstraints(maxWidth: constraints.maxWidth - 24),
              child: ListTile(
                contentPadding: EdgeInsets.zero,
                dense: true,
                title: Text(
                  ValueExpressionHelper.makeString(value),
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyLarge,
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

class ArgumentBar extends StatelessWidget {

  const ArgumentBar({
    super.key,
    required this.name,
    required this.type,
    required this.option,
    required this.value,
  });

  // ----------------

  final String                    name;
  final ArgumentType              type;
  final List<ValueExpression>?    option;
  final Wrapper<ValueExpression?> value;

  // ----------------

  @override
  build(context) {
    if (this.option == null) {
      if (this.type == ArgumentType.boolean) {
        return _BooleanArgumentBar(
          name: this.name,
          value: this.value.cast(),
        );
      }
      if (this.type == ArgumentType.integer) {
        return _IntegerArgumentBar(
          name: this.name,
          value: this.value.cast(),
        );
      }
      if (this.type == ArgumentType.floater) {
        return _FloaterArgumentBar(
          name: this.name,
          value: this.value.cast(),
        );
      }
      if (this.type == ArgumentType.size) {
        return _SizeArgumentBar(
          name: this.name,
          value: this.value.cast(),
        );
      }
      if (this.type == ArgumentType.string) {
        return _StringArgumentBar(
          name: this.name,
          value: this.value.cast(),
        );
      }
      if (this.type == ArgumentType.path) {
        return _PathArgumentBar(
          name: this.name,
          value: this.value.cast(),
        );
      }
    }
    if (this.option != null) {
      return _EnumerationArgumentBar(
        name: this.name,
        option: this.option!,
        value: this.value,
      );
    }
    throw UnimplementedError();
  }

}
