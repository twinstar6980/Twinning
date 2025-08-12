import '/common.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import '/view/resource_shipper/setting.dart';
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
    return StatefulBuilder(
      builder: (context, setState) => Column(
        children: [
          SizedBox(height: 8),
          CustomSettingItem(
            icon: IconSymbols.description,
            label: 'Option Configuration',
            content: [
              Text(
                !StorageHelper.existFileSync(this.data.optionConfiguration) ? 'Invalid' : 'Available',
                overflow: TextOverflow.ellipsis,
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
                        CustomStorageItemPickerButton(
                          allowLoadFile: true,
                          allowLoadDirectory: false,
                          allowSaveFile: false,
                          location: '@ResourceShipper.OptionConfiguration',
                          onPicked: (target) async {
                            this.data.optionConfiguration = target;
                            await refreshState(setStateForPanel);
                            await refreshState(setState);
                            this.onUpdate();
                          },
                        ),
                      ],
                    ),
                  ),
                  value: this.data.optionConfiguration,
                  onChanged: (value) async {
                    this.data.optionConfiguration = StorageHelper.regularize(value);
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
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
                !this.data.parallelForward ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.parallelForward,
                  onChanged: (value) async {
                    this.data.parallelForward = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
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
          CustomSettingItem(
            icon: IconSymbols.filter_alt,
            label: 'Enable Filter',
            content: [
              Text(
                !this.data.enableFilter ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.enableFilter,
                  onChanged: (value) async {
                    this.data.enableFilter = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
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
          CustomSettingItem(
            icon: IconSymbols.stacks,
            label: 'Enable Batch',
            content: [
              Text(
                !this.data.enableBatch ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.enableBatch,
                  onChanged: (value) async {
                    this.data.enableBatch = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
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
