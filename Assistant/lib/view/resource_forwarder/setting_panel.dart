import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import 'package:provider/provider.dart';
import 'package:flutter/material.dart';

// ----------------

class SettingPanel extends StatefulWidget {

  const SettingPanel({
    super.key,
  });

  @override
  createState() => _SettingPanelState();

  // ----------------

}

class _SettingPanelState extends State<SettingPanel> {

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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return Column(
      children: [
        const SizedBox(height: 4),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.description,
          label: 'Option Configuration',
          content: [
            Text(
              !StorageHelper.existFileSync(setting.data.mResourceForwarder.mOptionConfiguration) ? 'Invalid' : 'Available',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (focused) async {
                  if (!focused) {
                    await setting.save();
                  }
                },
                child: TextFormField(
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  initialValue: setting.data.mResourceForwarder.mOptionConfiguration,
                  onChanged: (value) async {
                    setting.data.mResourceForwarder.mOptionConfiguration = value;
                  },
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.shuffle,
          label: 'Parallel Forward',
          content: [
            Text(
              !setting.data.mResourceForwarder.mParallelForward ? 'Disabled' : 'Enabled',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mResourceForwarder.mParallelForward,
                onChanged: (value) async {
                  setting.data.mResourceForwarder.mParallelForward = value;
                  await setting.save();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.filter_alt,
          label: 'Enable Filter',
          content: [
            Text(
              !setting.data.mResourceForwarder.mEnableFilter ? 'Disabled' : 'Enabled',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mResourceForwarder.mEnableFilter,
                onChanged: (value) async {
                  setting.data.mResourceForwarder.mEnableFilter = value;
                  await setting.save();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.stacks,
          label: 'Enable Batch',
          content: [
            Text(
              !setting.data.mResourceForwarder.mEnableBatch ? 'Disabled' : 'Enabled',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mResourceForwarder.mEnableBatch,
                onChanged: (value) async {
                  setting.data.mResourceForwarder.mEnableBatch = value;
                  await setting.save();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
          ],
        ),
        const SizedBox(height: 4),
      ],
    );
  }

}
