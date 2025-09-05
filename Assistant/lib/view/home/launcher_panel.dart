import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/control_helper.dart';
import '/view/home/common.dart';
import '/view/home/launcher_configuration_panel.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class LauncherPanel extends StatelessWidget {

  const LauncherPanel({
    super.key,
    required this.onLaunch,
  });

  // ----------------

  final Void Function(ModuleLauncherConfiguration) onLaunch;

  // ----------------

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    return StatefulBuilder(
      builder: (context, setState) => Column(
        children: [
          SizedBox(height: 12),
          Row(
            children: [
              SizedBox(width: 16),
              FilledButton.icon(
                icon: Icon(IconSymbols.keyboard_command_key),
                label: Text(
                  'Command',
                  overflow: TextOverflow.ellipsis,
                ),
                onPressed: () async {
                  var command = <String>[];
                  var canContinue = await ControlHelper.showDialogAsModal<Boolean>(context, CustomModalDialog(
                    title: 'Command',
                    contentBuilder: (context, setStateForPanel) => [
                      CustomTextField(
                        keyboardType: TextInputType.multiline,
                        inputFormatters: [],
                        decoration: InputDecoration(
                          contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                          filled: false,
                          border: OutlineInputBorder(),
                        ),
                        value: ConvertHelper.makeStringListToStringWithLine(command),
                        onChanged: (value) async {
                          command = ConvertHelper.parseStringListFromStringWithLine(value);
                          await refreshState(setStateForPanel);
                        },
                      ),
                    ],
                    actionBuilder: (context) => [
                      TextButton(
                        child: Text('Cancel'),
                        onPressed: () => Navigator.pop(context, false),
                      ),
                      TextButton(
                        child: Text('Continue'),
                        onPressed: () => Navigator.pop(context, true),
                      ),
                    ],
                  )) ?? false;
                  if (canContinue) {
                    await setting.state.handleCommand!(command);
                  }
                },
              ).withExpanded(),
              SizedBox(width: 16),
            ],
          ),
          SizedBox(height: 0),
          CustomSettingLabel(
            label: 'Module',
            action: null,
          ),
          ...setting.data.moduleLauncher.module.map((item) => CustomSettingItem(
            icon: ModuleHelper.query(item.type).icon,
            label: item.title,
            content: [
              IconButton(
                tooltip: 'Setting',
                icon: Icon(IconSymbols.settings),
                onPressed: () async {
                  await ControlHelper.showDialogAsFull<Void>(context, CustomFullDialog(
                    title: 'Module Setting',
                    contentBuilder: (context, setStateForPanel) => [
                      ModuleHelper.query(item.type).settingPanel(context),
                    ],
                  ));
                },
              ),
              SizedBox(width: 4),
              IconButton(
                tooltip: 'Edit',
                icon: Icon(IconSymbols.edit),
                onPressed: () async {
                  await ControlHelper.showDialogAsFull<Void>(context, CustomFullDialog(
                    title: 'Launcher Configuration',
                    contentBuilder: (context, setStateForPanel) => [
                      LauncherConfigurationPanel(
                        data: item,
                        onUpdate: () async {
                          await refreshState(setState);
                          await setting.save();
                        },
                      ),
                    ],
                  ));
                },
              ),
            ],
            onTap: () async {
              this.onLaunch(item);
            },
            panelBuilder: null,
          )),
          CustomSettingLabel(
            label: 'Pinned',
            action: IconButton(
              tooltip: 'Add',
              icon: Icon(IconSymbols.add),
              onPressed: () async {
                setting.data.moduleLauncher.pinned.add(ModuleLauncherConfiguration(
                  title: 'Untitled',
                  type: ModuleType.modding_worker,
                  option: [],
                ));
                await setting.save();
              },
            ),
          ),
          ...setting.data.moduleLauncher.pinned.map((item) => CustomSettingItem(
            icon: ModuleHelper.query(item.type).icon,
            label: item.title,
            content: [
              IconButton(
                tooltip: 'Remove',
                icon: Icon(IconSymbols.remove),
                onPressed: () async {
                  if (await ControlHelper.showDialogForConfirm(context)) {
                    setting.data.moduleLauncher.pinned.remove(item);
                    await setting.save();
                  }
                },
              ),
              SizedBox(width: 4),
              IconButton(
                tooltip: 'Edit',
                icon: Icon(IconSymbols.edit),
                onPressed: () async {
                  await ControlHelper.showDialogAsFull<Void>(context, CustomFullDialog(
                    title: 'Launcher Configuration',
                    contentBuilder: (context, setStateForPanel) => [
                      LauncherConfigurationPanel(
                        data: item,
                        onUpdate: () async {
                          await refreshState(setState);
                          await setting.save();
                        },
                      ),
                    ],
                  ));
                },
              ),
            ],
            onTap: () async {
              this.onLaunch(item);
            },
            panelBuilder: null,
          )),
          CustomSettingLabel(
            label: 'Recent',
            action: IconButton(
              tooltip: 'Clear',
              icon: Icon(IconSymbols.clear),
              onPressed: () async {
                if (await ControlHelper.showDialogForConfirm(context)) {
                  setting.data.moduleLauncher.recent.clear();
                  await setting.save();
                }
              },
            ),
          ),
          ...setting.data.moduleLauncher.recent.map((item) => CustomSettingItem(
            icon: ModuleHelper.query(item.type).icon,
            label: item.title,
            content: [
              IconButton(
                tooltip: 'Pin',
                icon: Icon(IconSymbols.push_pin),
                onPressed: () async {
                  setting.data.moduleLauncher.recent.remove(item);
                  setting.data.moduleLauncher.pinned.add(item);
                  await setting.save();
                },
              ),
              SizedBox(width: 4),
              IconButton(
                tooltip: 'Remove',
                icon: Icon(IconSymbols.remove),
                onPressed: () async {
                  setting.data.moduleLauncher.recent.remove(item);
                  await setting.save();
                },
              ),
              SizedBox(width: 4),
              IconButton(
                tooltip: 'Edit',
                icon: Icon(IconSymbols.edit),
                onPressed: () async {
                  await ControlHelper.showDialogAsFull<Void>(context, CustomFullDialog(
                    title: 'Launcher Configuration',
                    contentBuilder: (context, setStateForPanel) => [
                      LauncherConfigurationPanel(
                        data: item,
                        onUpdate: () async {
                          await refreshState(setState);
                          await setting.save();
                        },
                      ),
                    ],
                  ));
                },
              ),
            ],
            onTap: () async {
              this.onLaunch(item);
            },
            panelBuilder: null,
          )),
          SizedBox(height: 8),
        ],
      ),
    );
  }

}
