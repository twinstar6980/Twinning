import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/permission_helper.dart';
import '/view/home/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

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

  late String? _applicationSharedDirectory;
  late Boolean _hasStoragePermission;

  // ----------------

  @override
  initState() {
    super.initState();
    this._applicationSharedDirectory = null;
    this._hasStoragePermission = false;
    (() async {
      this._applicationSharedDirectory = await StorageHelper.queryApplicationSharedDirectory();
      this._hasStoragePermission = await PermissionHelper.checkStorage();
      this.setState(() {});
    })();
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
        const CustomSettingLabel(
          label: 'Theme',
          action: null,
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.brightness_4,
          label: 'Theme Mode',
          content: [
            Text(
              ['System', 'Light', 'Dark'][setting.data.mThemeMode.index],
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ...ThemeMode.values.map(
              (mode) => ListTile(
                leading: Radio<ThemeMode>(
                  value: mode,
                  groupValue: setting.data.mThemeMode,
                  onChanged: (value) async {
                    value!;
                    setting.data.mThemeMode = value;
                    await setting.update();
                  },
                ),
                title: Text(
                  ['System', 'Light', 'Dark'][mode.index],
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.colorize,
          label: 'Theme Color',
          content: [
            Text(
              !setting.data.mThemeColorState ? 'Default' : 'Custom',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mThemeColorState,
                onChanged: (value) async {
                  setting.data.mThemeColorState = value;
                  await setting.update();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
            ListTile(
              leading: const Icon(IconSymbols.light_mode),
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
                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9a-fA-F]'))],
                  initialValue: setting.data.mThemeColorLight.withOpacity(0.0).value.toRadixString(16).padRight(6, '0'),
                  onChanged: (value) async {
                    setting.data.mThemeColorLight = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withOpacity(1.0);
                  },
                ),
              ),
              trailing: Icon(
                IconSymbols.circle,
                color: setting.data.mThemeColorLight,
                fill: 0.8,
              ),
            ),
            ListTile(
              leading: const Icon(IconSymbols.dark_mode),
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
                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9a-fA-F]'))],
                  initialValue: setting.data.mThemeColorDark.withOpacity(0.0).value.toRadixString(16).padRight(6, '0'),
                  onChanged: (value) async {
                    setting.data.mThemeColorDark = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withOpacity(1.0);
                  },
                ),
              ),
              trailing: Icon(
                IconSymbols.circle,
                color: setting.data.mThemeColorDark,
                fill: 0.8,
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.text_fields,
          label: 'Theme Font',
          content: [
            Text(
              !setting.data.mThemeFontState ? 'Default' : 'Custom',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mThemeFontState,
                onChanged: (value) async {
                  setting.data.mThemeFontState = value;
                  await setting.update();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
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
                  initialValue: ConvertHelper.convertStringListToTextWithLine(setting.data.mThemeFontPath),
                  onChanged: (value) async {
                    setting.data.mThemeFontPath = ConvertHelper.convertStringListFromTextWithLine(value);
                  },
                ),
              ),
            ),
          ],
        ),
        const CustomSettingLabel(
          label: 'Window',
          action: null,
        ),
        CustomSettingItem(
          enabled: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
          icon: IconSymbols.open_with,
          label: 'Window Position',
          content: [
            Text(
              !setting.data.mWindowPositionState ? 'Default' : 'Custom',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mWindowPositionState,
                onChanged: (value) async {
                  setting.data.mWindowPositionState = value;
                  await setting.update();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
            ListTile(
              leading: SizedBox(
                width: 16,
                child: Container(
                  alignment: Alignment.center,
                  child: Text(
                    'X',
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.titleMedium,
                  ),
                ),
              ),
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
                  keyboardType: TextInputType.number,
                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                  initialValue: setting.data.mWindowPositionX.toString(),
                  onChanged: (value) async {
                    setting.data.mWindowPositionX = Integer.tryParse(value) ?? 0;
                  },
                ),
              ),
            ),
            ListTile(
              leading: SizedBox(
                width: 16,
                child: Container(
                  alignment: Alignment.center,
                  child: Text(
                    'Y',
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.titleMedium,
                  ),
                ),
              ),
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
                  keyboardType: TextInputType.number,
                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                  initialValue: setting.data.mWindowPositionY.toString(),
                  onChanged: (value) async {
                    setting.data.mWindowPositionY = Integer.tryParse(value) ?? 0;
                  },
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
          enabled: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
          icon: IconSymbols.fit_screen,
          label: 'Window Size',
          content: [
            Text(
              !setting.data.mWindowSizeState ? 'Default' : 'Custom',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mWindowSizeState,
                onChanged: (value) async {
                  setting.data.mWindowSizeState = value;
                  await setting.update();
                },
              ),
              title: Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleMedium,
              ),
            ),
            ListTile(
              leading: SizedBox(
                width: 16,
                child: Container(
                  alignment: Alignment.center,
                  child: Text(
                    'W',
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.titleMedium,
                  ),
                ),
              ),
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
                  keyboardType: TextInputType.number,
                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                  initialValue: setting.data.mWindowSizeWidth.toString(),
                  onChanged: (value) async {
                    setting.data.mWindowSizeWidth = Integer.tryParse(value) ?? 0;
                  },
                ),
              ),
            ),
            ListTile(
              leading: SizedBox(
                width: 16,
                child: Container(
                  alignment: Alignment.center,
                  child: Text(
                    'H',
                    overflow: TextOverflow.ellipsis,
                    style: theme.textTheme.titleMedium,
                  ),
                ),
              ),
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
                  keyboardType: TextInputType.number,
                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                  initialValue: setting.data.mWindowSizeHeight.toString(),
                  onChanged: (value) async {
                    setting.data.mWindowSizeHeight = Integer.tryParse(value) ?? 0;
                  },
                ),
              ),
            ),
          ],
        ),
        const CustomSettingLabel(
          label: 'Storage',
          action: null,
        ),
        CustomSettingItem(
          enabled: true,
          icon: IconSymbols.folder_special,
          label: 'Shared Directory',
          content: [
            Text(
              this._applicationSharedDirectory == null ? 'Invalid' : 'Available',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setState) => [
            TextFormField(
              decoration: const InputDecoration(
                isDense: true,
                border: OutlineInputBorder(),
              ),
              keyboardType: TextInputType.text,
              inputFormatters: const [],
              initialValue: this._applicationSharedDirectory,
              readOnly: true,
            ),
          ],
        ),
        CustomSettingItem(
          enabled: Platform.isAndroid,
          icon: IconSymbols.key,
          label: 'Storage Permission',
          content: [
            Text(
              !this._hasStoragePermission ? 'Denied' : 'Granted',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: () async {
            this._hasStoragePermission = await PermissionHelper.requestStorage();
            this.setState(() {});
          },
          panelBuilder: null,
        ),
        CustomSettingItem(
          enabled: Platform.isAndroid,
          icon: IconSymbols.snippet_folder,
          label: 'Fallback Directory',
          content: [
            Text(
              !StorageHelper.existDirectorySync(setting.data.mFallbackDirectory) ? 'Invalid' : 'Available',
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
                  initialValue: setting.data.mFallbackDirectory,
                  onChanged: (value) async {
                    setting.data.mFallbackDirectory = value;
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
