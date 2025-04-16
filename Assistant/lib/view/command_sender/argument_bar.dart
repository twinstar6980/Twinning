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
    super.key, // ignore: unused_element_parameter
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
            const SizedBox(width: 8),
            Expanded(
              child: Text(
                this.name,
                overflow: TextOverflow.clip,
                style: theme.textTheme.labelLarge?.copyWith(
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

class _BooleanArgumentBar extends StatelessWidget {

  const _BooleanArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
  });

  // ----------------

  final String                      name;
  final Wrapper<BooleanExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.check_box,
        content: CustomTextField(
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

class _IntegerArgumentBar extends StatelessWidget {

  const _IntegerArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
  });

  // ----------------

  final String                      name;
  final Wrapper<IntegerExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.speed_1_2,
        content: CustomTextField(
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

class _FloaterArgumentBar extends StatelessWidget {

  const _FloaterArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
  });

  // ----------------

  final String                      name;
  final Wrapper<FloaterExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.speed_1_2,
        content: CustomTextField(
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

class _SizeArgumentBar extends StatelessWidget {

  const _SizeArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
  });

  // ----------------

  final String                   name;
  final Wrapper<SizeExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.memory,
        content: CustomTextField(
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
                      ),
                    ),
                  itemBuilder: (context) => ['B', 'K', 'M', 'G'].mapIndexed((index, value) => PopupMenuItem(
                    value: index,
                    child: Text(
                      value,
                      overflow: TextOverflow.ellipsis,
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

class _StringArgumentBar extends StatelessWidget {

  const _StringArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
  });

  // ----------------

  final String                     name;
  final Wrapper<StringExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.text_fields,
        content: CustomTextField(
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

class _PathArgumentBar extends StatelessWidget {

  const _PathArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.value,
  });

  // ----------------

  final String                   name;
  final Wrapper<PathExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.link,
        content: CustomFileDropRegion(
          onDrop: (item) async {
            this.value.value = PathExpression(item.first);
            setState(() {});
          },
          child: CustomTextField(
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

class _EnumerationArgumentBar extends StatelessWidget {

  const _EnumerationArgumentBar({
    super.key, // ignore: unused_element_parameter
    required this.name,
    required this.option,
    required this.value,
  });

  // ----------------

  final String                    name;
  final List<ValueExpression>     option;
  final Wrapper<ValueExpression?> value;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => _BasicArgumentBar(
        name: this.name,
        icon: IconSymbols.menu,
        content: CustomOptionField(
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
          option: this.option.map((value) => (value, ValueExpressionHelper.makeString(value))).toList(),
          value: this.value.value == null ? '' : ValueExpressionHelper.makeString(this.value.value!),
          onChanged: (value) async {
            this.value.value = value as ValueExpression;
            setState(() {});
          },
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
