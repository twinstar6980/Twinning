import '/common.dart';
import '/module.dart';
import '/setting.dart';
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
    return ListView(
      shrinkWrap: true,
      physics: const NeverScrollableScrollPhysics(),
      padding: const EdgeInsets.fromLTRB(0, 4, 0, 8),
      children: [
        const CustomSettingLabel(
          label: 'Module',
          action: null,
        ),
        ...setting.data.mModuleLauncher.mModule.map((item) => CustomSettingItem(
          enabled: true,
          icon: ModuleHelper.query(item.type).icon,
          label: item.title,
          content: [
            IconButton(
              tooltip: 'Setting',
              icon: const Icon(IconSymbols.settings),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Module Setting',
                  contentBuilder: (context, setState) => [
                    ModuleHelper.query(item.type).settingPanel(),
                  ],
                );
                await setting.save();
              },
            ),
            IconButton(
              tooltip: 'Edit',
              icon: const Icon(IconSymbols.edit),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Launcher Configuration',
                  contentBuilder: (context, setState) => [
                    LauncherConfigurationPanel(
                      data: item,
                      onUpdate: () async {
                        await setting.save();
                      },
                    ),
                  ],
                );
                await setting.save();
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
            icon: const Icon(IconSymbols.add),
            onPressed: () async {
              setting.data.mModuleLauncher.mPinned.add(ModuleLauncherConfiguration(
                title: 'Untitled',
                type: ModuleType.modding_worker,
                option: [],
              ));
              setting.save();
            },
          ),
        ),
        ...setting.data.mModuleLauncher.mPinned.map((item) => CustomSettingItem(
          enabled: true,
          icon: ModuleHelper.query(item.type).icon,
          label: item.title,
          content: [
            IconButton(
              tooltip: 'Remove',
              icon: const Icon(IconSymbols.remove),
              onPressed: () async {
                setting.data.mModuleLauncher.mPinned.remove(item);
                setting.save();
              },
            ),
            IconButton(
              tooltip: 'Edit',
              icon: const Icon(IconSymbols.edit),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Launcher Configuration',
                  contentBuilder: (context, setState) => [
                    LauncherConfigurationPanel(
                      data: item,
                      onUpdate: () async {
                        await setting.save();
                      },
                    ),
                  ],
                );
                await setting.save();
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
            icon: const Icon(IconSymbols.clear),
            onPressed: () async {
              if (await ControlHelper.showCustomConfirmDialog(context: context)) {
                setting.data.mModuleLauncher.mRecent.clear();
                setting.save();
              }
            },
          ),
        ),
        ...setting.data.mModuleLauncher.mRecent.map((item) => CustomSettingItem(
          enabled: true,
          icon: ModuleHelper.query(item.type).icon,
          label: item.title,
          content: [
            IconButton(
              tooltip: 'Pin',
              icon: const Icon(IconSymbols.push_pin),
              onPressed: () async {
                setting.data.mModuleLauncher.mRecent.remove(item);
                setting.data.mModuleLauncher.mPinned.add(item);
                setting.save();
              },
            ),
            IconButton(
              tooltip: 'Remove',
              icon: const Icon(IconSymbols.remove),
              onPressed: () async {
                setting.data.mModuleLauncher.mRecent.remove(item);
                setting.save();
              },
            ),
            IconButton(
              tooltip: 'Edit',
              icon: const Icon(IconSymbols.edit),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Launcher Configuration',
                  contentBuilder: (context, setState) => [
                    LauncherConfigurationPanel(
                      data: item,
                      onUpdate: () async {
                        await setting.save();
                      },
                    ),
                  ],
                );
                await setting.save();
              },
            ),
          ],
          onTap: () async {
            this.onLaunch(item);
          },
          panelBuilder: null,
        )),
      ],
    );
  }

}
