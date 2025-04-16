import '/common.dart';
import '/setting.dart';
import '/module.dart';
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
              (item) => ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Radio(
                  value: item,
                  groupValue: setting.data.mThemeMode,
                  onChanged: (value) async {
                    setting.data.mThemeMode = value!;
                    setStateForPanel(() {});
                    this.setState(() {});
                    await setting.save();
                  },
                ),
                title: Text(
                  ['System', 'Light', 'Dark'][item.index],
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ),
          ],
        ),
        CustomSettingItem(
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
              title: CustomTextField(
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
                value: setting.data.mThemeColorLight.withValues(alpha: 0.0).toARGB32().toRadixString(16).padLeft(6, '0'),
                onChanged: (value) async {
                  setting.data.mThemeColorLight = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withValues(alpha: 1.0);
                  setStateForPanel(() {});
                  this.setState(() {});
                  await setting.save();
                },
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextField(
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
                value: setting.data.mThemeColorDark.withValues(alpha: 0.0).toARGB32().toRadixString(16).padLeft(6, '0'),
                onChanged: (value) async {
                  setting.data.mThemeColorDark = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withValues(alpha: 1.0);
                  setStateForPanel(() {});
                  this.setState(() {});
                  await setting.save();
                },
              ),
            ),
          ],
        ),
        CustomSettingItem(
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
              title: CustomTextField(
                keyboardType: TextInputType.multiline,
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
                          var target = await StorageHelper.pickLoadFile(context, 'Application.ThemeFont');
                          if (target != null) {
                            setting.data.mThemeFontPath = setting.data.mThemeFontPath + [target];
                            setStateForPanel(() {});
                            this.setState(() {});
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
          icon: IconSymbols.recenter,
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
              title: CustomTextField(
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
                  setStateForPanel(() {});
                  this.setState(() {});
                  await setting.save();
                },
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextField(
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
              title: CustomTextField(
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
                  setStateForPanel(() {});
                  this.setState(() {});
                  await setting.save();
                },
              ),
            ),
            ListTile(
              contentPadding: EdgeInsets.zero,
              title: CustomTextField(
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
                  setStateForPanel(() {});
                  this.setState(() {});
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
          label: 'Storage Picker Fallback Directory',
          content: [
            Text(
              !StorageHelper.existDirectorySync(setting.data.mStoragePickerFallbackDirectory) ? 'Invalid' : 'Available',
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
                          var target = await StorageHelper.pickLoadDirectory(context, 'Application.StoragePickerFallbackDirectory');
                          if (target != null) {
                            setting.data.mStoragePickerFallbackDirectory = target;
                            setStateForPanel(() {});
                            this.setState(() {});
                            await setting.save();
                          }
                        },
                      ),
                    ],
                  ),
                ),
                value: setting.data.mStoragePickerFallbackDirectory,
                onChanged: (value) async {
                  setting.data.mStoragePickerFallbackDirectory = StorageHelper.regularize(value);
                  setStateForPanel(() {});
                  this.setState(() {});
                  await setting.save();
                },
              ),
            ),
          ],
        ),
        const CustomSettingLabel(
          label: 'Forwarder',
          action: null,
        ),
        CustomSettingItem(
          icon: IconSymbols.nearby,
          label: 'Forwarder Default Target',
          content: [
            Text(
              ModuleHelper.query(setting.data.mForwarderDefaultTarget).name,
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setStateForPanel) => [
            ...ModuleType.values.map(
              (item) => ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Radio(
                  value: item,
                  groupValue: setting.data.mForwarderDefaultTarget,
                  onChanged: (value) async {
                    setting.data.mForwarderDefaultTarget = value!;
                    setStateForPanel(() {});
                    this.setState(() {});
                    await setting.save();
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
          icon: IconSymbols.next_plan,
          label: 'Forwarder Immediate Forward',
          content: [
            Text(
              !setting.data.mForwarderImmediateForward ? 'Disabled' : 'Enabled',
              overflow: TextOverflow.ellipsis,
              style: theme.textTheme.bodyMedium,
            ),
          ],
          onTap: null,
          panelBuilder: (context, setStateForPanel) => [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: Switch(
                value: setting.data.mForwarderImmediateForward,
                onChanged: (value) async {
                  setting.data.mForwarderImmediateForward = value;
                  setStateForPanel(() {});
                  this.setState(() {});
                  await setting.save();
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
    );
  }

}
