import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/view/home/common.dart';
import 'package:flutter/material.dart';

// ----------------

class LauncherConfigurationPanel extends StatefulWidget {

  const LauncherConfigurationPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  @override
  createState() => _LauncherConfigurationPanelState();

  // ----------------

  final ModuleLauncherConfiguration data;
  final Void Function()             onUpdate;

}

class _LauncherConfigurationPanelState extends State<LauncherConfigurationPanel> {

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
    var theme = Theme.of(context);
    return Column(
      children: [
        const SizedBox(height: 4),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.text_fields,
          label: 'Title',
          content: [
            Expanded(
              child: Text(
                this.widget.data.title,
                overflow: TextOverflow.ellipsis,
                textAlign: TextAlign.right,
                style: theme.textTheme.bodyMedium,
              ),
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (focused) async {
                  if (!focused) {
                    this.setState(() {});
                    this.widget.onUpdate();
                  }
                },
                child: TextFormField(
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  initialValue: this.widget.data.title,
                  onChanged: (value) async {
                    this.widget.data.title = value;
                  },
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.label,
          label: 'Type',
          content: [
            Expanded(
              child: Text(
                ModuleHelper.query(this.widget.data.type).name,
                overflow: TextOverflow.ellipsis,
                textAlign: TextAlign.right,
                style: theme.textTheme.bodyMedium,
              ),
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ...ModuleType.values.map(
              (type) => ListTile(
                leading: Radio<ModuleType>(
                  value: type,
                  groupValue: this.widget.data.type,
                  onChanged: (value) async {
                    value!;
                    this.widget.data.type = value;
                    this.widget.onUpdate();
                  },
                ),
                title: Text(
                  ModuleHelper.query(type).name,
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.format_list_bulleted,
          label: 'Option',
          content: [
            Expanded(
              child: Text(
                '${this.widget.data.option.length}',
                overflow: TextOverflow.ellipsis,
                textAlign: TextAlign.right,
                style: theme.textTheme.bodyMedium,
              ),
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (focused) async {
                  if (!focused) {
                    this.setState(() {});
                    this.widget.onUpdate();
                  }
                },
                child: TextFormField(
                  keyboardType: TextInputType.multiline,
                  maxLines: null,
                  inputFormatters: const [],
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  initialValue: ConvertHelper.makeStringListToStringWithLine(this.widget.data.option),
                  onChanged: (value) async {
                    this.widget.data.option = ConvertHelper.parseStringListFromStringWithLine(value);
                  },
                ),
              ),
            ),
          ],
        ),
        const SizedBox(height: 4),
      ],
    );
  }

}
