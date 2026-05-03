import '/common.dart';
import '/setting.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/miscellaneous_helper.dart';
import '/utility/application_permission_manager.dart';
import '/utility/application_extension_manager.dart';
import '/widget/export.dart';
import '/view/home/about_panel.dart';
import 'package:flutter/widgets.dart';
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

  late List<Boolean> _permissionState;
  late List<Boolean> _extensionState;

  // ----------------

  @override
  initState() {
    super.initState();
    this._permissionState = ApplicationPermissionName.values.map((it) => false).toList();
    this._extensionState = ApplicationExtensionName.values.map((it) => false).toList();
    postTask(() async {
      this._permissionState = await ApplicationPermissionName.values.map((it) async => await ApplicationPermissionManager.instance.check(it)).wait;
      this._extensionState = await ApplicationExtensionName.values.map((it) async => await ApplicationExtensionManager.instance.check(it)).wait;
      await refreshState(this.setState);
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
    return FlexContainer.vertical([
      Gap.vertical(4),
      SettingListLabel(
        label: 'Theme',
        action: null,
      ),
      SettingListItem(
        icon: IconSet.brightness_4,
        label: 'Mode',
        comment: [
          StyledText.inherit(['System', 'Light', 'Dark'][setting.data.themeMode.index]),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          ...StyledThemeMode.values.map((item) => StyledListTile.standardTight(
            leading: StyledRadio.standard(
              value: setting.data.themeMode == item,
              onChanged: (context) async {
                setting.data.themeMode = item;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit(['System', 'Light', 'Dark'][item.index]),
          )),
        ],
      ),
      SettingListItem(
        icon: IconSet.colorize,
        label: 'Color',
        comment: [
          StyledText.inherit(!setting.data.themeColorState ? 'Default' : 'Custom'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: setting.data.themeColorState,
              onChanged: (context, value) async {
                setting.data.themeColorState = value;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit('Enable'),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .text,
              format: .new(r'[0-9a-fA-F]'),
              hint: null,
              prefix: IconView.of(IconSet.light_mode),
              suffix: [
                IconView.of(
                  IconSet.circle,
                  fill: 0.6,
                  color: setting.data.themeColorLight,
                ),
              ],
              value: setting.data.themeColorLight.withValues(alpha: 0.0).toARGB32().toRadixString(16).padLeft(6, '0'),
              onChanged: (context, value) async {
                setting.data.themeColorLight = .new(Integer.tryParse(value, radix: 16) ?? 0x000000).withValues(alpha: 1.0);
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .text,
              format: .new(r'[0-9a-fA-F]'),
              hint: null,
              prefix: IconView.of(IconSet.dark_mode),
              suffix: [
                IconView.of(
                  IconSet.circle,
                  fill: 0.6,
                  color: setting.data.themeColorDark,
                ),
              ],
              value: setting.data.themeColorDark.withValues(alpha: 0.0).toARGB32().toRadixString(16).padLeft(6, '0'),
              onChanged: (context, value) async {
                setting.data.themeColorDark = .new(Integer.tryParse(value, radix: 16) ?? 0x000000).withValues(alpha: 1.0);
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
        ],
      ),
      SettingListItem(
        icon: IconSet.text_fields,
        label: 'Font',
        comment: [
          StyledText.inherit(!setting.data.themeFontState ? 'Default' : 'Custom'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: setting.data.themeFontState,
              onChanged: (context, value) async {
                setting.data.themeFontState = value;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit('Enable'),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .multiline,
              format: null,
              hint: null,
              prefix: null,
              suffix: [
                StyledIconButton.standard(
                  tooltip: 'Pick',
                  icon: IconView.of(IconSet.open_in_new),
                  onPressed: (context) async {
                    var target = await MiscellaneousHelper.pickStorageItem(context, 'application.theme_font', [.loadFile], true, null, null);
                    if (!target.isEmpty) {
                      setting.data.themeFontPath = [...setting.data.themeFontPath, ...target];
                      await refreshState(setStateForPanel);
                      await refreshState(this.setState);
                      await setting.save();
                    }
                  },
                ),
              ],
              value: ConvertHelper.makeStringListToStringWithLine(setting.data.themeFontPath.map((it) => it.emit()).toList()),
              onChanged: (context, value) async {
                setting.data.themeFontPath = ConvertHelper.parseStringListFromStringWithLine(value).map(StoragePath.of).toList();
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
        ],
      ),
      SettingListLabel(
        label: 'Window',
        action: null,
      ),
      SettingListItem(
        enabled: SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh,
        icon: IconSet.recenter,
        label: 'Position',
        comment: [
          StyledText.inherit(!setting.data.windowPositionState ? 'Default' : 'Custom'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: setting.data.windowPositionState,
              onChanged: (context, value) async {
                setting.data.windowPositionState = value;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit('Enable'),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .number,
              format: .new(r'[0-9]'),
              hint: null,
              prefix: IconView.of(IconSet.swap_horiz),
              suffix: null,
              value: setting.data.windowPositionX.toString(),
              onChanged: (context, value) async {
                setting.data.windowPositionX = Integer.tryParse(value) ?? setting.data.windowPositionX;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .number,
              format: .new(r'[0-9]'),
              hint: null,
              prefix: IconView.of(IconSet.swap_vert),
              suffix: null,
              value: setting.data.windowPositionY.toString(),
              onChanged: (context, value) async {
                setting.data.windowPositionY = Integer.tryParse(value) ?? setting.data.windowPositionY;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
        ],
      ),
      SettingListItem(
        enabled: SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh,
        icon: IconSet.fit_screen,
        label: 'Size',
        comment: [
          StyledText.inherit(!setting.data.windowSizeState ? 'Default' : 'Custom'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: setting.data.windowSizeState,
              onChanged: (context, value) async {
                setting.data.windowSizeState = value;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit('Enable'),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .number,
              format: .new(r'[0-9]'),
              hint: null,
              prefix: IconView.of(IconSet.width),
              suffix: null,
              value: setting.data.windowSizeWidth.toString(),
              onChanged: (context, value) async {
                setting.data.windowSizeWidth = Integer.tryParse(value) ?? setting.data.windowSizeWidth;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .number,
              format: .new(r'[0-9]'),
              hint: null,
              prefix: IconView.of(IconSet.height),
              suffix: null,
              value: setting.data.windowSizeHeight.toString(),
              onChanged: (context, value) async {
                setting.data.windowSizeHeight = Integer.tryParse(value) ?? setting.data.windowSizeHeight;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
        ],
      ),
      SettingListLabel(
        label: 'Forwarder',
        action: null,
      ),
      SettingListItem(
        icon: IconSet.nearby,
        label: 'Default Target',
        comment: [
          StyledText.inherit(ModuleHelper.query(setting.data.forwarderDefaultTarget).name),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          ...ModuleType.values.map((item) => StyledListTile.standardTight(
            leading: StyledRadio.standard(
              value: setting.data.forwarderDefaultTarget == item,
              onChanged: (context) async {
                setting.data.forwarderDefaultTarget = item;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit(ModuleHelper.query(item).name),
          )),
        ],
      ),
      SettingListItem(
        icon: IconSet.touch_app,
        label: 'Immediate Jump',
        comment: [
          StyledText.inherit(!setting.data.forwarderImmediateJump ? 'Disabled' : 'Enabled'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: setting.data.forwarderImmediateJump,
              onChanged: (context, value) async {
                setting.data.forwarderImmediateJump = value;
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
            content: StyledText.inherit('Enable'),
          ),
        ],
      ),
      SettingListLabel(
        label: 'Module',
        action: null,
      ),
      SettingListItem(
        icon: IconSet.description,
        label: 'Configuration Directory',
        comment: [
          StyledText.inherit(!StorageHelper.existDirectorySync(setting.data.moduleConfigurationDirectory) ? 'Invalid' : 'Available'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            content: StyledInput.outlined(
              type: .text,
              format: null,
              hint: null,
              prefix: null,
              suffix: [
                StyledIconButton.standard(
                  tooltip: 'Pick',
                  icon: IconView.of(IconSet.open_in_new),
                  onPressed: (context) async {
                    var target = (await MiscellaneousHelper.pickStorageItem(context, 'application.module_configuration_directory', [.loadDirectory], false, null, null)).firstOrNull;
                    if (target != null) {
                      setting.data.moduleConfigurationDirectory = target;
                      await refreshState(setStateForPanel);
                      await refreshState(this.setState);
                      await setting.save();
                    }
                  },
                ),
              ],
              value: setting.data.moduleConfigurationDirectory.emit(),
              onChanged: (context, value) async {
                setting.data.moduleConfigurationDirectory = .of(value);
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
                await setting.save();
              },
            ),
          ),
        ],
      ),
      SettingListLabel(
        label: 'Permission',
        action: null,
      ),
      ...<({ApplicationPermissionName name, String label, IconData icon})>[
        (name: .storage, label: 'Storage', icon: IconSet.storage),
        (name: .notification, label: 'Notification', icon: IconSet.notification_sound),
      ].map((it) => SettingListItem(
        icon: it.icon,
        label: it.label,
        comment: [
          StyledText.inherit(!this._permissionState[it.name.index] ? 'Denied' : 'Granted'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: this._permissionState[it.name.index],
              onChanged: (context, value) async {
                await ApplicationPermissionManager.instance.update(it.name);
                this._permissionState[it.name.index] = await ApplicationPermissionManager.instance.check(it.name);
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
              },
            ),
            content: StyledText.inherit('Grant'),
          ),
        ],
      )),
      SettingListLabel(
        label: 'Extension',
        action: null,
      ),
      ...<({ApplicationExtensionName name, String label, IconData icon})>[
        (name: .forwarder, label: 'Forwarder', icon: IconSet.send_time_extension),
      ].map((it) => SettingListItem(
        icon: it.icon,
        label: it.label,
        comment: [
          StyledText.inherit(!this._extensionState[it.name.index] ? 'Disabled' : 'Enabled'),
        ],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standardTight(
            leading: StyledSwitch.standard(
              value: this._extensionState[it.name.index],
              onChanged: (context, value) async {
                await ApplicationExtensionManager.instance.update(it.name, !this._extensionState[it.name.index]);
                this._extensionState[it.name.index] = await ApplicationExtensionManager.instance.check(it.name);
                await refreshState(setStateForPanel);
                await refreshState(this.setState);
              },
            ),
            content: StyledText.inherit('Enable'),
          ),
        ],
      )),
      SettingListLabel(
        label: 'Other',
        action: null,
      ),
      SettingListItem(
        icon: IconSet.info,
        label: 'About',
        comment: [],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          AboutPanel(),
        ],
      ),
      SettingListItem(
        icon: IconSet.tour,
        label: 'Onboarding',
        comment: [],
        onPressed: (context) async {
          await setting.state.homeShowOnboarding!();
        },
        panelBuilder: null,
      ),
      SettingListItem(
        icon: IconSet.settings,
        label: 'Setting File',
        comment: [],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standard(
            leading: IconView.of(IconSet.file_open),
            content: StyledText.inherit('Reveal'),
            onPressed: (context) async {
              Navigator.pop(context);
              var target = await setting.file();
              await MoreModalDialogExtension.showForRevealStoragePath(context, 'Setting File', target);
            },
          ),
          StyledListTile.standard(
            leading: IconView.of(IconSet.refresh),
            content: StyledText.inherit('Reload'),
            onPressed: (context) async {
              Navigator.pop(context);
              await setting.load();
              await setting.save();
              await StyledSnackExtension.show(context, 'Done!');
            },
          ),
          StyledListTile.standard(
            leading: IconView.of(IconSet.reset_wrench),
            content: StyledText.inherit('Reset'),
            onPressed: (context) async {
              if (await MoreModalDialogExtension.showForConfirm(context)) {
                Navigator.pop(context);
                await setting.reset();
                await setting.save();
                await StyledSnackExtension.show(context, 'Done!');
              }
            },
          ),
          StyledListTile.standard(
            leading: IconView.of(IconSet.download),
            content: StyledText.inherit('Import'),
            onPressed: (context) async {
              var target = (await MiscellaneousHelper.pickStorageItem(context, 'application.setting_file', [.loadFile], false, null, null)).firstOrNull;
              if (target != null) {
                Navigator.pop(context);
                await setting.load(file: target);
                await setting.save();
                await StyledSnackExtension.show(context, 'Done!');
              }
            },
          ),
          StyledListTile.standard(
            leading: IconView.of(IconSet.upload),
            content: StyledText.inherit('Export'),
            onPressed: (context) async {
              var target = (await MiscellaneousHelper.pickStorageItem(context, 'application.setting_file', [.saveFile], false, null, 'setting.json')).firstOrNull;
              if (target != null) {
                Navigator.pop(context);
                await setting.save(file: target, apply: false);
                await StyledSnackExtension.show(context, 'Done!');
              }
            },
          ),
        ],
      ),
      SettingListItem(
        icon: IconSet.folder_special,
        label: 'Shared Directory',
        comment: [],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standard(
            leading: IconView.of(IconSet.folder_open),
            content: StyledText.inherit('Reveal'),
            onPressed: (context) async {
              Navigator.pop(context);
              var target = await StorageHelper.query(.applicationShared);
              await MoreModalDialogExtension.showForRevealStoragePath(context, 'Shared Directory', target);
            },
          ),
        ],
      ),
      SettingListItem(
        icon: IconSet.folder_delete,
        label: 'Temporary Directory',
        comment: [],
        onPressed: null,
        panelBuilder: (context, setStateForPanel) => [
          StyledListTile.standard(
            leading: IconView.of(IconSet.delete_sweep),
            content: StyledText.inherit('Clear'),
            onPressed: (context) async {
              Navigator.pop(context);
              var target = await StorageHelper.query(.applicationTemporary);
              if (await StorageHelper.exist(target)) {
                await StorageHelper.remove(target);
              }
              await StyledSnackExtension.show(context, 'Done!');
            },
          ),
        ],
      ),
      Gap.vertical(8),
    ]);
  }

}
