import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
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
    return ListView(
      shrinkWrap: true,
      physics: const NeverScrollableScrollPhysics(),
      padding: const EdgeInsets.fromLTRB(0, 4, 0, 4),
      children: [
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.adjust,
          label: 'Kernel',
          content: [
            Text(
              !StorageHelper.existFileSync(setting.data.mModdingWorker.mKernel) ? 'Invalid' : 'Available',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (value) async {
                  if (!value) {
                    await setting.update();
                  }
                },
                child: TextFormField(
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  initialValue: setting.data.mModdingWorker.mKernel,
                  onChanged: (value) async {
                    setting.data.mModdingWorker.mKernel = value;
                  },
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.data_object,
          label: 'Script',
          content: [
            Text(
              !StorageHelper.existFileSync(setting.data.mModdingWorker.mScript) ? 'Invalid' : 'Available',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (value) async {
                  if (!value) {
                    await setting.update();
                  }
                },
                child: TextFormField(
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  initialValue: setting.data.mModdingWorker.mScript,
                  onChanged: (value) async {
                    setting.data.mModdingWorker.mScript = value;
                  },
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.format_list_bulleted,
          label: 'Argument',
          content: [
            Text(
              !setting.data.mModdingWorker.mArgument.isNotEmpty ? 'Empty' : 'Defined',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (value) async {
                  if (!value) {
                    await setting.update();
                  }
                },
                child: TextFormField(
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  keyboardType: TextInputType.multiline,
                  maxLines: null,
                  inputFormatters: const [],
                  initialValue: ConvertHelper.convertStringListToTextWithLine(setting.data.mModdingWorker.mArgument),
                  onChanged: (value) async {
                    setting.data.mModdingWorker.mArgument = ConvertHelper.convertStringListFromTextWithLine(value);
                  },
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.touch_app,
          label: 'Immediate Launch',
          content: [
            Text(
              !setting.data.mModdingWorker.mImmediateLaunch ? 'Disabled' : 'Enabled',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mModdingWorker.mImmediateLaunch,
                onChanged: (value) async {
                  setting.data.mModdingWorker.mImmediateLaunch = value;
                  await setting.update();
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
          icon: IconSymbols.text_fields,
          label: 'Message Font',
          content: [
            Text(
              !setting.data.mModdingWorker.mMessageFont.isNotEmpty ? 'Default' : 'Custom',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: Focus(
                onFocusChange: (value) async {
                  if (!value) {
                    await setting.update();
                  }
                },
                child: TextFormField(
                  decoration: const InputDecoration(
                    isDense: true,
                  ),
                  keyboardType: TextInputType.multiline,
                  maxLines: null,
                  inputFormatters: const [],
                  initialValue: ConvertHelper.convertStringListToTextWithLine(setting.data.mModdingWorker.mMessageFont),
                  onChanged: (value) async {
                    setting.data.mModdingWorker.mMessageFont = ConvertHelper.convertStringListFromTextWithLine(value);
                  },
                ),
              ),
            ),
          ],
        ),
      ],
    );
  }

}
