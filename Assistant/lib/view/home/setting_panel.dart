import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/permission_helper.dart';
import '/utility/control_helper.dart';
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

  // ----------------

  // ----------------

  @override
  createState() => _SettingPanelState();

}

class _SettingPanelState extends State<SettingPanel> {

  late Boolean _hasStoragePermission;

  // ----------------

  @override
  initState() {
    super.initState();
    this._hasStoragePermission = false;
    ControlHelper.postTask(() async {
      this._hasStoragePermission = await PermissionHelper.checkStorage();
      this.setState(() {});
    });
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
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
          panelBuilder: (context, setStateForPanel) => [
            ...ThemeMode.values.map(
              (mode) => ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Radio<ThemeMode>(
                  value: mode,
                  groupValue: setting.data.mThemeMode,
                  onChanged: (value) async {
                    setting.data.mThemeMode = value!;
                    await setting.save();
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
          panelBuilder: (context, setStateForPanel) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mThemeColorState,
                onChanged: (value) async {
                  setting.data.mThemeColorState = value;
                  await setting.save();
                },
              ),
              title: const Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.text,
                inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9a-fA-F]'))],
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: const OutlineInputBorder(),
                  prefixIcon: const Icon(IconSymbols.light_mode),
                  suffixIcon: Icon(
                    IconSymbols.circle,
                    fill: 0.6,
                    color: setting.data.mThemeColorLight,
                  ),
                ),
                value: setting.data.mThemeColorLight.withOpacity(0.0).value.toRadixString(16).padLeft(6, '0'),
                onChanged: (value) async {
                  setting.data.mThemeColorLight = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withOpacity(1.0);
                  await setting.save();
                },
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.text,
                inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9a-fA-F]'))],
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: const OutlineInputBorder(),
                  prefixIcon: const Icon(IconSymbols.dark_mode),
                  suffixIcon: Icon(
                    IconSymbols.circle,
                    fill: 0.6,
                    color: setting.data.mThemeColorDark,
                  ),
                ),
                value: setting.data.mThemeColorDark.withOpacity(0.0).value.toRadixString(16).padLeft(6, '0'),
                onChanged: (value) async {
                  setting.data.mThemeColorDark = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withOpacity(1.0);
                  await setting.save();
                },
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
          panelBuilder: (context, setStateForPanel) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mThemeFontState,
                onChanged: (value) async {
                  setting.data.mThemeFontState = value;
                  await setting.save();
                },
              ),
              title: const Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.multiline,
                inputFormatters: const [],
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: const OutlineInputBorder(),
                  suffixIcon: CustomTextFieldSuffixWidget(
                    children: [
                      IconButton(
                        tooltip: 'Pick',
                        icon: const Icon(IconSymbols.open_in_new),
                        onPressed: () async {
                          var target = await StorageHelper.pickLoadFile(context, 'Application.ThemeFont');
                          if (target != null) {
                            setting.data.mThemeFontPath = setting.data.mThemeFontPath + [target];
                            await setting.save();
                          }
                        },
                      ),
                    ],
                  ),
                ),
                value: ConvertHelper.makeStringListToStringWithLine(setting.data.mThemeFontPath),
                onChanged: (value) async {
                  setting.data.mThemeFontPath = ConvertHelper.parseStringListFromStringWithLine(value);
                  await setting.save();
                },
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
          panelBuilder: (context, setStateForPanel) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mWindowPositionState,
                onChanged: (value) async {
                  setting.data.mWindowPositionState = value;
                  await setting.save();
                },
              ),
              title: const Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.number,
                inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                decoration: const InputDecoration(
                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: OutlineInputBorder(),
                  prefixIcon: Icon(IconSymbols.swap_horiz),
                ),
                value: setting.data.mWindowPositionX.toString(),
                onChanged: (value) async {
                  setting.data.mWindowPositionX = Integer.tryParse(value) ?? setting.data.mWindowPositionX;
                  await setting.save();
                },
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.number,
                inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                decoration: const InputDecoration(
                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: OutlineInputBorder(),
                  prefixIcon: Icon(IconSymbols.swap_vert),
                ),
                value: setting.data.mWindowPositionY.toString(),
                onChanged: (value) async {
                  setting.data.mWindowPositionY = Integer.tryParse(value) ?? setting.data.mWindowPositionY;
                  await setting.save();
                },
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
          panelBuilder: (context, setStateForPanel) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mWindowSizeState,
                onChanged: (value) async {
                  setting.data.mWindowSizeState = value;
                  await setting.save();
                },
              ),
              title: const Text(
                'Enable',
                overflow: TextOverflow.ellipsis,
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.number,
                inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                decoration: const InputDecoration(
                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: OutlineInputBorder(),
                  prefixIcon: Icon(IconSymbols.width),
                ),
                value: setting.data.mWindowSizeWidth.toString(),
                onChanged: (value) async {
                  setting.data.mWindowSizeWidth = Integer.tryParse(value) ?? setting.data.mWindowSizeWidth;
                  await setting.save();
                },
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.number,
                inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                decoration: const InputDecoration(
                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: OutlineInputBorder(),
                  prefixIcon: Icon(IconSymbols.height),
                ),
                value: setting.data.mWindowSizeHeight.toString(),
                onChanged: (value) async {
                  setting.data.mWindowSizeHeight = Integer.tryParse(value) ?? setting.data.mWindowSizeHeight;
                  await setting.save();
                },
              ),
            ),
          ],
        ),
        const CustomSettingLabel(
          label: 'Storage',
          action: null,
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
          panelBuilder: (context, setStateForPanel) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextFieldWithFocus(
                keyboardType: TextInputType.text,
                inputFormatters: const [],
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                  filled: false,
                  border: const OutlineInputBorder(),
                  suffixIcon: CustomTextFieldSuffixWidget(
                    children: [
                      IconButton(
                        tooltip: 'Pick',
                        icon: const Icon(IconSymbols.open_in_new),
                        onPressed: () async {
                          var target = await StorageHelper.pickLoadDirectory(context, 'Application.FallbackDirectory');
                          if (target != null) {
                            setting.data.mFallbackDirectory = target;
                            await setting.save();
                          }
                        },
                      ),
                    ],
                  ),
                ),
                value: setting.data.mFallbackDirectory,
                onChanged: (value) async {
                  setting.data.mFallbackDirectory = StorageHelper.regularize(value);
                  await setting.save();
                },
              ),
            ),
          ],
        ),
        const SizedBox(height: 8),
      ],
    );
  }

}
