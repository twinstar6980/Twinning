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
        ...setting.data.mModuleLauncher.mModule.map((e) => CustomSettingItem(
          enabled: true,
          icon: ModuleHelper.query(e.type).icon,
          label: e.title,
          content: [
            IconButton(
              icon: const Icon(IconSymbols.settings),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Module Setting',
                  contentBuilder: (context, setState) => ModuleHelper.query(e.type).settingPanel(),
                );
                await setting.update();
              },
            ),
            IconButton(
              icon: const Icon(IconSymbols.edit),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Launcher Configuration',
                  contentBuilder: (context, setState) => LauncherConfigurationPanel(
                    data: e,
                    onUpdate: () async {
                      await setting.update();
                    },
                  ),
                );
                await setting.update();
              },
            ),
          ],
          onTap: () async {
            this.onLaunch(e);
          },
          panelBuilder: null,
        )),
        CustomSettingLabel(
          label: 'Pinned',
          action: IconButton(
            icon: const Icon(IconSymbols.add),
            onPressed: () async {
              setting.data.mModuleLauncher.mPinned.add(ModuleLauncherConfiguration(
                title: 'Untitled',
                type: ModuleType.modding_worker,
                option: [],
              ));
              setting.update();
            },
          ),
        ),
        ...setting.data.mModuleLauncher.mPinned.map((e) => CustomSettingItem(
          enabled: true,
          icon: ModuleHelper.query(e.type).icon,
          label: e.title,
          content: [
            IconButton(
              icon: const Icon(IconSymbols.remove),
              onPressed: () async {
                setting.data.mModuleLauncher.mPinned.remove(e);
                setting.update();
              },
            ),
            IconButton(
              icon: const Icon(IconSymbols.edit),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Launcher Configuration',
                  contentBuilder: (context, setState) => LauncherConfigurationPanel(
                    data: e,
                    onUpdate: () async {
                      await setting.update();
                    },
                  ),
                );
                await setting.update();
              },
            ),
          ],
          onTap: () async {
            this.onLaunch(e);
          },
          panelBuilder: null,
        )),
        CustomSettingLabel(
          label: 'Recent',
          action: IconButton(
            icon: const Icon(IconSymbols.clear),
            onPressed: () async {
              setting.data.mModuleLauncher.mRecent.clear();
              setting.update();
            },
          ),
        ),
        ...setting.data.mModuleLauncher.mRecent.map((e) => CustomSettingItem(
          enabled: true,
          icon: ModuleHelper.query(e.type).icon,
          label: e.title,
          content: [
            IconButton(
              icon: const Icon(IconSymbols.push_pin),
              onPressed: () async {
                setting.data.mModuleLauncher.mRecent.remove(e);
                setting.data.mModuleLauncher.mPinned.add(e);
                setting.update();
              },
            ),
            IconButton(
              icon: const Icon(IconSymbols.remove),
              onPressed: () async {
                setting.data.mModuleLauncher.mRecent.remove(e);
                setting.update();
              },
            ),
            IconButton(
              icon: const Icon(IconSymbols.edit),
              onPressed: () async {
                await ControlHelper.showCustomModalBottomSheet(
                  context: context,
                  title: 'Launcher Configuration',
                  contentBuilder: (context, setState) => LauncherConfigurationPanel(
                    data: e,
                    onUpdate: () async {
                      await setting.update();
                    },
                  ),
                );
                await setting.update();
              },
            ),
          ],
          onTap: () async {
            this.onLaunch(e);
          },
          panelBuilder: null,
        )),
      ],
    );
  }

}
