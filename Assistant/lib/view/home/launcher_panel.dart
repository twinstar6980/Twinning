import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/widget/export.dart';
import '/view/home/launcher_configuration_panel.dart';
import 'package:collection/collection.dart' as lib;

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
    var setting = SettingProvider.of(context);
    return StatefulBuilder(
      builder: (context, setState) => FlexContainer.vertical([
        Gap.vertical(12),
        FlexContainer.horizontal([
          Gap.horizontal(16),
          StyledButton.filled(
            icon: IconView.of(IconSet.keyboard_command_key),
            content: StyledText.inherit('Command'),
            onPressed: (context) async {
              var command = <String>[];
              var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
                title: 'Command',
                contentBuilder: (context, setStateForPanel) => [
                  StyledInput.outlined(
                    type: .multiline,
                    format: null,
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
                actionBuilder: (context) => MoreModalDialogExtension.createButtonForContinue(context),
              )) ?? false;
              if (canContinue) {
                await setting.state.handleCommand!(command);
              }
            },
          ).withFlexExpanded(),
          Gap.horizontal(16),
        ]),
        Gap.vertical(0),
        ...ModuleLauncherCategory.values.map((category) => <Widget>[
          SettingListLabel(
            label: ConvertHelper.changeStringFromCamelCaseToHeadlineStyle(category.name),
            action: switch (category) {
              .module => null,
              .pinned => StyledIconButton.standard(
                tooltip: 'Add',
                icon: IconView.of(IconSet.add),
                onPressed: (context) async {
                  setting.data.moduleLauncher[ModuleLauncherCategory.pinned]!.add(.new(
                    title: 'Untitled',
                    type: .coreTaskWorker,
                    option: ModuleHelper.query(.coreTaskWorker).generateDefaultOption(),
                  ));
                  await setting.save();
                },
              ),
              .recent => StyledIconButton.standard(
                tooltip: 'Clear',
                icon: IconView.of(IconSet.clear),
                onPressed: (context) async {
                  if (await MoreModalDialogExtension.showForConfirm(context, 'Clear')) {
                    setting.data.moduleLauncher[ModuleLauncherCategory.recent]!.clear();
                    await setting.save();
                  }
                },
              ),
            },
          ),
          ...setting.data.moduleLauncher[category]!.map((item) => SettingListItem(
            icon: ModuleHelper.query(item.type).icon,
            label: item.title,
            comment: [
              if (category == .module) ...[
                StyledIconButton.standard(
                  tooltip: 'Setting',
                  icon: IconView.of(IconSet.settings),
                  onPressed: (context) async {
                    await StyledFullDialogExtension.show<Void>(context, StyledFullDialog.standard(
                      title: 'Module Setting',
                      contentBuilder: (context, setStateForPanel) => [
                        ModuleHelper.query(item.type).buildSettingPanel(context),
                      ],
                    ));
                  },
                ),
                Gap.horizontal(4),
              ],
              if (category == .recent) ...[
                StyledIconButton.standard(
                  tooltip: 'Pin',
                  icon: IconView.of(IconSet.push_pin),
                  onPressed: (context) async {
                    setting.data.moduleLauncher[ModuleLauncherCategory.recent]!.remove(item);
                    setting.data.moduleLauncher[ModuleLauncherCategory.pinned]!.add(item);
                    await setting.save();
                  },
                ),
                Gap.horizontal(4),
              ],
              if (category == .pinned || category == .recent) ...[
                StyledIconButton.standard(
                  tooltip: 'Remove',
                  icon: IconView.of(IconSet.remove),
                  onPressed: (context) async {
                    if (await MoreModalDialogExtension.showForConfirm(context, 'Remove')) {
                      setting.data.moduleLauncher[category]!.remove(item);
                      await setting.save();
                    }
                  },
                ),
                Gap.horizontal(4),
              ],
              ...[
                StyledIconButton.standard(
                  tooltip: 'Edit',
                  icon: IconView.of(IconSet.edit),
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
            ],
            onPressed: (context) async {
              this.onLaunch(item);
            },
            panelBuilder: null,
          )),
        ]).flattened,
        Gap.vertical(8),
      ]),
    );
  }

}
