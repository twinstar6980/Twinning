import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/widget/export.dart';
import '/view/home/launcher_configuration_panel.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class LauncherPanel extends StatelessWidget {

  const LauncherPanel({
    super.key,
    required this.onLaunch,
  });

  // ----------------

  final Void Function(ModuleLauncherConfiguration configuration) onLaunch;

  // ----------------

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    return StatefulBuilder(
      builder: (context, setState) => FlexContainer.vertical([
        Gap.vertical(12),
        FlexContainer.horizontal([
          Gap.horizontal(16),
          StyledButton.filled(
            icon: Icon(IconSet.keyboard_command_key),
            content: StyledText.inherit('Command'),
            onPressed: (context) async {
              var command = <String>[];
              var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
                title: 'Command',
                contentBuilder: (context, setStateForPanel) => [
                  StyledInput.outlined(
                    type: StyledInputType.multiline,
                    format: [],
                    hint: null,
                    prefix: null,
                    suffix: null,
                    value: ConvertHelper.makeStringListToStringWithLine(command),
                    onChanged: (context, value) async {
                      command = ConvertHelper.parseStringListFromStringWithLine(value);
                      await refreshState(setStateForPanel);
                    },
                  ),
                ],
                actionBuilder: (context) => [
                  StyledButton.text(
                    content: StyledText.inherit('Cancel'),
                    onPressed: (context) => Navigator.pop(context, false),
                  ),
                  StyledButton.text(
                    content: StyledText.inherit('Continue'),
                    onPressed: (context) => Navigator.pop(context, true),
                  ),
                ],
              )) ?? false;
              if (canContinue) {
                await setting.state.handleCommand!(command);
              }
            },
          ).withFlexExpanded(),
          Gap.horizontal(16),
        ]),
        Gap.vertical(0),
        SettingListLabel(
          label: 'Module',
          action: null,
        ),
        ...setting.data.moduleLauncher.module.map((item) => SettingListItem(
          icon: ModuleHelper.query(item.type).icon,
          label: item.title,
          comment: [
            StyledIconButton.standard(
              tooltip: 'Setting',
              icon: Icon(IconSet.settings),
              onPressed: (context) async {
                await StyledFullDialogExtension.show<Void>(context, StyledFullDialog.standard(
                  title: 'Module Setting',
                  contentBuilder: (context, setStateForPanel) => [
                    ModuleHelper.query(item.type).settingPanel(context),
                  ],
                ));
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Edit',
              icon: Icon(IconSet.edit),
              onPressed: (context) async {
                await StyledFullDialogExtension.show<Void>(context, StyledFullDialog.standard(
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
          onPressed: (context) async {
            this.onLaunch(item);
          },
          panelBuilder: null,
        )),
        SettingListLabel(
          label: 'Pinned',
          action: StyledIconButton.standard(
            tooltip: 'Add',
            icon: Icon(IconSet.add),
            onPressed: (context) async {
              setting.data.moduleLauncher.pinned.add(ModuleLauncherConfiguration(
                title: 'Untitled',
                type: ModuleType.modding_worker,
                option: [],
              ));
              await setting.save();
            },
          ),
        ),
        ...setting.data.moduleLauncher.pinned.map((item) => SettingListItem(
          icon: ModuleHelper.query(item.type).icon,
          label: item.title,
          comment: [
            StyledIconButton.standard(
              tooltip: 'Remove',
              icon: Icon(IconSet.remove),
              onPressed: (context) async {
                if (await showDialogForConfirm(context)) {
                  setting.data.moduleLauncher.pinned.remove(item);
                  await setting.save();
                }
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Edit',
              icon: Icon(IconSet.edit),
              onPressed: (context) async {
                await StyledFullDialogExtension.show<Void>(context, StyledFullDialog.standard(
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
          onPressed: (context) async {
            this.onLaunch(item);
          },
          panelBuilder: null,
        )),
        SettingListLabel(
          label: 'Recent',
          action: StyledIconButton.standard(
            tooltip: 'Clear',
            icon: Icon(IconSet.clear),
            onPressed: (context) async {
              if (await showDialogForConfirm(context)) {
                setting.data.moduleLauncher.recent.clear();
                await setting.save();
              }
            },
          ),
        ),
        ...setting.data.moduleLauncher.recent.map((item) => SettingListItem(
          icon: ModuleHelper.query(item.type).icon,
          label: item.title,
          comment: [
            StyledIconButton.standard(
              tooltip: 'Pin',
              icon: Icon(IconSet.push_pin),
              onPressed: (context) async {
                setting.data.moduleLauncher.recent.remove(item);
                setting.data.moduleLauncher.pinned.add(item);
                await setting.save();
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Remove',
              icon: Icon(IconSet.remove),
              onPressed: (context) async {
                setting.data.moduleLauncher.recent.remove(item);
                await setting.save();
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Edit',
              icon: Icon(IconSet.edit),
              onPressed: (context) async {
                await StyledFullDialogExtension.show<Void>(context, StyledFullDialog.standard(
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
          onPressed: (context) async {
            this.onLaunch(item);
          },
          panelBuilder: null,
        )),
        Gap.vertical(8),
      ]),
    );
  }

}
