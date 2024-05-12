import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/control_helper.dart';
import '/view/home/common.dart';
import '/view/home/blank_panel.dart';
import '/view/home/launcher_panel.dart';
import '/view/home/setting_panel.dart';
import '/view/home/about_panel.dart';
import 'dart:math';
import 'package:collection/collection.dart';
import 'package:provider/provider.dart';
import 'package:flutter/material.dart';

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
  });

  @override
  createState() => _MainPageState();

  // ----------------

}

class _MainPageState extends State<MainPage> {

  late Widget                                  _blankPanel;
  late List<(String, ModuleType, Widget, Key)> _tabList;
  late Integer                                 _tabIndex;

  Future<Void> _insertTabItem(
    String       title,
    ModuleType   type,
    List<String> option,
  ) async {
    this._tabList.add((title, type, ModuleHelper.query(type).mainPage(option), ValueKey(DateTime.now().millisecondsSinceEpoch)));
    this._tabIndex = this._tabList.length - 1;
    this.setState(() {});
    return;
  }

  Future<Void> _removeTabItem(
    Integer index,
  ) async {
    this._tabList.removeAt(index);
    if (this._tabIndex > index) {
      this._tabIndex--;
    }
    this._tabIndex = min(this._tabIndex, this._tabList.length - 1);
    this.setState(() {});
    return;
  }

  Future<Void> _toggleTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    this._tabIndex = index;
    this.setState(() {});
    return;
  }

  Future<Void> _executeLauncher(
    ModuleLauncherConfiguration configuration,
  ) async {
    await this._insertTabItem(configuration.title, configuration.type, configuration.option);
    return;
  }

  Future<Void> _openLauncherPanel(
  ) async {
    await ControlHelper.showCustomModalBottomSheet(
      context: this.context,
      title: 'Launcher',
      contentBuilder: (context, setState) => LauncherPanel(
        onLaunch: (configuration) async {
          Navigator.pop(context);
          await this._executeLauncher(configuration);
        },
      ),
    );
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._blankPanel = BlankPanel(
      showLauncherPanel: this._openLauncherPanel,
    );
    this._tabList = [];
    this._tabIndex = -1;
    ControlHelper.runAfterNextFrame(() async {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      if (setting.state.mInitialTab != null) {
        await this._insertTabItem(setting.state.mInitialTab!.$1, setting.state.mInitialTab!.$2, setting.state.mInitialTab!.$3);
      }
      else {
        await this._openLauncherPanel();
      }
    });
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    return Scaffold(
      appBar: AppBar(
        centerTitle: false,
        elevation: 3,
        scrolledUnderElevation: 3,
        title: const SizedBox(),
        leading: const SizedBox(),
        actions: [
          Expanded(
            child: CustomTitleBar(
              title: this._tabList.isEmpty ? '' : this._tabList[this._tabIndex].$1,
            ),
          ),
        ],
      ),
      drawer: NavigationDrawer(
        children: [
          const SizedBox(height: 16),
          const CustomNavigationDrawerLabel(
            label: 'Assistant',
          ),
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.widgets,
            label: 'Launcher',
            action: const [],
            onPressed: () async {
              Navigator.pop(context);
              await this._openLauncherPanel();
            },
          ),
          const CustomNavigationDrawerDivider(),
          ...this._tabList.mapIndexed((index, e) => CustomNavigationDrawerItem(
            selected: index == this._tabIndex,
            icon: ModuleHelper.query(e.$2).icon,
            label: e.$1,
            action: [
              IconButton(
                icon: const Icon(IconSymbols.remove),
                onPressed: () async {
                  if (await ControlHelper.showCustomConfirmDialog(context: context)) {
                    await this._removeTabItem(index);
                  }
                },
              ),
            ],
            onPressed: () async {
              Navigator.pop(context);
              await this._toggleTabItem(index);
            },
          )),
          const CustomNavigationDrawerDivider(),
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.settings,
            label: 'Setting',
            action: const [],
            onPressed: () async {
              await ControlHelper.showCustomModalBottomSheet(
                context: context,
                title: 'Setting',
                contentBuilder: (context, setState) => const SettingPanel(),
              );
            },
          ),
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.contact_support,
            label: 'About',
            action: const [],
            onPressed: () async {
              await ControlHelper.showCustomModalBottomSheet(
                context: context,
                title: 'About',
                contentBuilder: (context, setState) => const AboutPanel(),
              );
            },
          ),
          const SizedBox(height: 16),
        ],
      ),
      body: Stack(
        children: [
          Offstage(
            offstage: this._tabIndex != -1,
            child: this._blankPanel,
          ),
          ...this._tabList.mapIndexed((index, e) => Offstage(
            key: e.$4,
            offstage: this._tabIndex != index,
            child: e.$3,
          )),
        ],
      ),
    );
  }

}
