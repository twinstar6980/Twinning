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
            SizedBox(width: 8),
            Text(
              this.name,
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.labelLarge!.copyWith(
                color: theme.colorScheme.primary,
              ),
            ).withTooltip(
              message: this.name,
            ).withExpanded(),
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
          inputFormatters: [],
          decoration: InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Boolean',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
                IconButton(
                  tooltip: 'No',
                  isSelected: this.value.value == null ? false : this.value.value!.value == false,
                  icon: Icon(IconSymbols.do_not_disturb_on),
                  selectedIcon: Icon(IconSymbols.do_not_disturb_on, fill: 1),
                  onPressed: () async {
                    this.value.value = this.value.value?.value == false ? null : BooleanExpression(false);
                    await refreshState(setState);
                  },
                ),
                SizedBox(width: 4),
                IconButton(
                  tooltip: 'Yes',
                  isSelected: this.value.value == null ? false : this.value.value!.value == true,
                  icon: Icon(IconSymbols.check_circle),
                  selectedIcon: Icon(IconSymbols.check_circle, fill: 1),
                  onPressed: () async {
                    this.value.value = this.value.value?.value == true ? null : BooleanExpression(true);
                    await refreshState(setState);
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
            await refreshState(setState);
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
          keyboardType: TextInputType.numberWithOptions(signed: true, decimal: false),
          inputFormatters: [],
          decoration: InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Integer',
            suffixIcon: null,
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
            await refreshState(setState);
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
          keyboardType: TextInputType.numberWithOptions(signed: true, decimal: true),
          inputFormatters: [],
          decoration: InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Floater',
            suffixIcon: null,
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
            await refreshState(setState);
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
          keyboardType: TextInputType.numberWithOptions(signed: false, decimal: true),
          inputFormatters: [],
          decoration: InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Size',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
                PopupMenuButton(
                  tooltip: 'Exponent',
                  position: PopupMenuPosition.under,
                  icon: this.value.value == null
                    ? Icon(IconSymbols.expand_circle_down)
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
                    await refreshState(setState);
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
            await refreshState(setState);
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
          inputFormatters: [],
          decoration: InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'String',
            suffixIcon: null,
          ),
          value: this.value.value == null ? '' : this.value.value!.value,
          onChanged: (text) async {
            if (text.isEmpty) {
              this.value.value = null;
            }
            else {
              this.value.value = StringExpression(text);
            }
            await refreshState(setState);
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
            await refreshState(setState);
          },
          child: CustomTextField(
            keyboardType: TextInputType.text,
            inputFormatters: [],
            decoration: InputDecoration(
              contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
              filled: false,
              border: UnderlineInputBorder(),
              hintText: 'Path',
              suffixIcon: CustomTextFieldSuffixRegion(
                children: [
                  CustomStorageItemPickerButton(
                    allowLoadFile: true,
                    allowLoadDirectory: true,
                    allowSaveFile: true,
                    location: '@CommandSender.Generic',
                    onPicked: (target) async {
                      this.value.value = PathExpression(target);
                      await refreshState(setState);
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
              await refreshState(setState);
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
            contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
            filled: false,
            border: UnderlineInputBorder(),
            hintText: 'Enumeration',
            suffixIcon: CustomTextFieldSuffixRegion(
              children: [
                IconButton(
                  tooltip: 'Reset',
                  icon: Icon(IconSymbols.restart_alt),
                  onPressed: () async {
                    this.value.value = null;
                    await refreshState(setState);
                  },
                ),
              ],
            ),
          ),
          option: this.option.map((value) => (value, ValueExpressionHelper.makeString(value))).toList(),
          value: this.value.value == null ? '' : ValueExpressionHelper.makeString(this.value.value!),
          onChanged: (value) async {
            this.value.value = value as ValueExpression;
            await refreshState(setState);
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
      return switch (this.type) {
        ArgumentType.boolean => _BooleanArgumentBar(
          name: this.name,
          value: this.value.cast(),
        ),
        ArgumentType.integer => _IntegerArgumentBar(
          name: this.name,
          value: this.value.cast(),
        ),
        ArgumentType.floater => _FloaterArgumentBar(
          name: this.name,
          value: this.value.cast(),
        ),
        ArgumentType.size => _SizeArgumentBar(
          name: this.name,
          value: this.value.cast(),
        ),
        ArgumentType.string => _StringArgumentBar(
          name: this.name,
          value: this.value.cast(),
        ),
        ArgumentType.path => _PathArgumentBar(
          name: this.name,
          value: this.value.cast(),
        ),
      };
    }
    else {
      return _EnumerationArgumentBar(
        name: this.name,
        option: this.option!,
        value: this.value,
      );
    }
  }

}
