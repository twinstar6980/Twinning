import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/view/home/common.dart';
import 'package:flutter/material.dart';

// ----------------

class LauncherConfigurationPanel extends StatelessWidget {

  const LauncherConfigurationPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  // ----------------

  final ModuleLauncherConfiguration data;
  final Void Function()             onUpdate;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => Column(
        children: [
          SizedBox(height: 8),
          CustomSettingItem(
            icon: IconSymbols.text_fields,
            label: 'Title',
            content: [
              Text(
                this.data.title,
                overflow: TextOverflow.ellipsis,
                textAlign: TextAlign.right,
              ).withExpanded(),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextField(
                  keyboardType: TextInputType.text,
                  inputFormatters: [],
                  decoration: InputDecoration(
                    contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: OutlineInputBorder(),
                  ),
                  value: this.data.title,
                  onChanged: (value) async {
                    this.data.title = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.label,
            label: 'Type',
            content: [
              Text(
                ModuleHelper.query(this.data.type).name,
                overflow: TextOverflow.ellipsis,
                textAlign: TextAlign.right,
              ).withExpanded(),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ...ModuleType.values.map(
                (item) => ListTile(
                  contentPadding: EdgeInsets.zero,
                  leading: Radio(
                    value: item,
                    groupValue: this.data.type,
                    onChanged: (value) async {
                      this.data.type = value!;
                      await refreshState(setStateForPanel);
                      await refreshState(setState);
                      this.onUpdate();
                    },
                  ),
                  title: Text(
                    ModuleHelper.query(item).name,
                    overflow: TextOverflow.ellipsis,
                  ),
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.format_list_bulleted,
            label: 'Option',
            content: [
              Text(
                '${this.data.option.length}',
                overflow: TextOverflow.ellipsis,
                textAlign: TextAlign.right,
              ).withExpanded(),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextField(
                  keyboardType: TextInputType.multiline,
                  inputFormatters: [],
                  decoration: InputDecoration(
                    contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: OutlineInputBorder(),
                  ),
                  value: ConvertHelper.makeStringListToStringWithLine(this.data.option),
                  onChanged: (value) async {
                    this.data.option = ConvertHelper.parseStringListFromStringWithLine(value);
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          SizedBox(height: 8),
        ],
      ),
    );
  }

}
