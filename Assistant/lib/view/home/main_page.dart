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
    ModuleLauncherConfiguration configuration,
  ) async {
    while (Navigator.canPop(context)) {
      Navigator.pop(context);
    }
    this._tabList.add((configuration.title, configuration.type, ModuleHelper.query(configuration.type).mainPage(configuration.option), ValueKey(DateTime.now().millisecondsSinceEpoch)));
    this._tabIndex = this._tabList.length - 1;
    this.setState(() {});
    await Future.delayed(const Duration(milliseconds: 10));
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

  Future<Void> _showLauncherPanel(
  ) async {
    await ControlHelper.showCustomModalBottomSheet(
      context: this.context,
      title: 'Launcher',
      contentBuilder: (context, setState) => [
        LauncherPanel(
          onLaunch: (configuration) async {
            Navigator.pop(context);
            await this._insertTabItem(configuration);
          },
        ),
      ],
    );
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._blankPanel = BlankPanel(
      showLauncherPanel: this._showLauncherPanel,
    );
    this._tabList = [];
    this._tabIndex = -1;
    {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      setting.state.mHomeShowLauncherPanel = this._showLauncherPanel;
      setting.state.mHomeInsertTabItem = this._insertTabItem;
    }
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
        backgroundColor: Colors.transparent,
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
              await this._showLauncherPanel();
            },
          ),
          const CustomNavigationDrawerDivider(),
          ...this._tabList.mapIndexed((index, value) => CustomNavigationDrawerItem(
            selected: index == this._tabIndex,
            icon: ModuleHelper.query(value.$2).icon,
            label: value.$1,
            action: [
              IconButton(
                tooltip: 'Remove',
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
              Navigator.pop(context);
              await ControlHelper.showCustomModalBottomSheet(
                context: context,
                title: 'Setting',
                contentBuilder: (context, setState) => [
                  const SettingPanel(),
                ],
              );
            },
          ),
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.contact_support,
            label: 'About',
            action: const [],
            onPressed: () async {
              Navigator.pop(context);
              await ControlHelper.showCustomModalBottomSheet(
                context: context,
                title: 'About',
                contentBuilder: (context, setState) => [
                  const AboutPanel(),
                ],
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
          ...this._tabList.mapIndexed((index, value) => Offstage(
            key: value.$4,
            offstage: this._tabIndex != index,
            child: value.$3,
          )),
        ],
      ),
    );
  }

}
