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
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => Column(
        children: [
          const SizedBox(height: 8),
          CustomSettingItem(
            icon: IconSymbols.description,
            label: 'Option Configuration',
            content: [
              Text(
                !StorageHelper.existFileSync(this.data.mOptionConfiguration) ? 'Invalid' : 'Available',
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
                  inputFormatters: const [],
                  decoration: InputDecoration(
                    contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: const OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixRegion(
                      children: [
                        IconButton(
                          tooltip: 'Pick',
                          icon: const Icon(IconSymbols.open_in_new),
                          onPressed: () async {
                            var target = await StorageHelper.pickLoadFile(context, 'ResourceShipper.OptionConfiguration');
                            if (target != null) {
                              this.data.mOptionConfiguration = target;
                              setStateForPanel(() {});
                              setState(() {});
                              this.onUpdate();
                            }
                          },
                        ),
                      ],
                    ),
                  ),
                  value: this.data.mOptionConfiguration,
                  onChanged: (value) async {
                    this.data.mOptionConfiguration = StorageHelper.regularize(value);
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
                title: const Text(
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
                !this.data.mEnableFilter ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mEnableFilter,
                  onChanged: (value) async {
                    this.data.mEnableFilter = value;
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
                title: const Text(
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
                !this.data.mEnableBatch ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mEnableBatch,
                  onChanged: (value) async {
                    this.data.mEnableBatch = value;
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
                title: const Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          const SizedBox(height: 8),
        ],
      ),
    );
  }

}
