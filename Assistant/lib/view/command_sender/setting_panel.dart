import '/common.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import '/view/command_sender/setting.dart';
import 'package:flutter/material.dart';

// ----------------

class SettingPanel extends StatelessWidget {

  const SettingPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  // ----------------

  final Setting         data;
  final Void Function() onUpdate;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => Column(
        children: [
          SizedBox(height: 8),
          CustomSettingItem(
            icon: IconSymbols.description,
            label: 'Method Configuration',
            content: [
              Text(
                !StorageHelper.existFileSync(this.data.mMethodConfiguration) ? 'Invalid' : 'Available',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
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
                    suffixIcon: CustomTextFieldSuffixRegion(
                      children: [
                        IconButton(
                          tooltip: 'Pick',
                          icon: Icon(IconSymbols.open_in_new),
                          onPressed: () async {
                            var target = await StorageHelper.pickLoadFile(context, 'CommandSender.MethodConfiguration');
                            if (target != null) {
                              this.data.mMethodConfiguration = target;
                              setStateForPanel(() {});
                              setState(() {});
                              this.onUpdate();
                            }
                          },
                        ),
                      ],
                    ),
                  ),
                  value: this.data.mMethodConfiguration,
                  onChanged: (value) async {
                    this.data.mMethodConfiguration = StorageHelper.regularize(value);
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.shuffle,
            label: 'Parallel Forward',
            content: [
              Text(
                !this.data.mParallelForward ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mParallelForward,
                  onChanged: (value) async {
                    this.data.mParallelForward = value;
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
                title: Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
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
