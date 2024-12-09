import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/control_helper.dart';
import '/utility/system_overlay_helper.dart';
import '/view/home/common.dart';
import '/view/home/blank_page.dart';
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

  // ----------------

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> {

  late List<(String, ModuleType, Widget)> _tabList;
  late Integer                            _tabIndex;

  Future<Void> _insertTabItem(
    ModuleLauncherConfiguration configuration,
  ) async {
    while (Navigator.canPop(context)) {
      Navigator.pop(context);
    }
    this._tabList.add((configuration.title, configuration.type, ModuleHelper.query(configuration.type).mainPage(configuration.option)));
    this._tabIndex = this._tabList.length - 1;
    this.setState(() {});
    await Future.delayed(const Duration(milliseconds: 10));
    return;
  }

  Future<Void> _removeTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    var itemState = this._tabList[index].$3.key!.as<GlobalKey>().currentState!.as<CustomModulePageState>();
    if (!await itemState.modulePageRequestClose()) {
      return;
    }
    this._tabList.removeAt(index);
    if (this._tabIndex > index) {
      this._tabIndex--;
    }
    this._tabIndex = min(this._tabIndex, this._tabList.length - 1);
    this.setState(() {});
    return;
  }

  Future<Void> _renameTabItem(
    Integer index,
    String  title,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    this._tabList[index] = (
      title,
      this._tabList[index].$2,
      this._tabList[index].$3,
    );
    this.setState(() {});
    return;
  }

  Future<Void> _keepTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    var itemState = this._tabList[index].$3.key!.as<GlobalKey>().currentState!.as<CustomModulePageState>();
    var configuration = ModuleLauncherConfiguration(
      title: this._tabList[index].$1,
      type: this._tabList[index].$2,
      option: await itemState.modulePageCollectOption(),
    );
    var setting = Provider.of<SettingProvider>(context, listen: false);
    setting.data.mModuleLauncher.pinned.add(configuration);
    await setting.save();
    return;
  }

  Future<Void> _duplicateTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    var itemState = this._tabList[index].$3.key!.as<GlobalKey>().currentState!.as<CustomModulePageState>();
    var configuration = ModuleLauncherConfiguration(
      title: this._tabList[index].$1,
      type: this._tabList[index].$2,
      option: await itemState.modulePageCollectOption(),
    );
    await this._insertTabItem(configuration);
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
    await ControlHelper.showBottomSheetAsModal<Void>(this.context, CustomModalBottomSheet(
      title: 'Launcher',
      contentBuilder: (context, setState) => [
        LauncherPanel(
          onLaunch: (configuration) async {
            Navigator.pop(context);
            await this._insertTabItem(configuration);
          },
        ),
      ],
    ));
    return;
  }

  Future<Void> _showForwarderPanel(
  ) async {
    var command = <String>[];
    var canContinue = await ControlHelper.showDialogAsModal<Boolean>(context, CustomModalDialog(
      title: 'Forwarder',
      contentBuilder: (context, setState) => [
        CustomTextField(
          keyboardType: TextInputType.multiline,
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
            filled: false,
            border: OutlineInputBorder(),
          ),
          value: ConvertHelper.makeStringListToStringWithLine(command),
          onChanged: (value) async {
            command = ConvertHelper.parseStringListFromStringWithLine(value);
            setState(() {});
          },
        ),
      ],
      actionBuilder: (context) => [
        TextButton(
          child: const Text('Cancel'),
          onPressed: () => Navigator.pop(context, false),
        ),
        TextButton(
          child: const Text('Continue'),
          onPressed: () => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
    if (canContinue) {
      await Provider.of<SettingProvider>(context, listen: false).state.mHandleForward!(command);
    }
    return;
  }

  Future<Void> _showCommanderPanel(
  ) async {
    var command = <String>[];
    var canContinue = await ControlHelper.showDialogAsModal<Boolean>(context, CustomModalDialog(
      title: 'Commander',
      contentBuilder: (context, setState) => [
        CustomTextField(
          keyboardType: TextInputType.multiline,
          inputFormatters: const [],
          decoration: const InputDecoration(
            contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
            filled: false,
            border: OutlineInputBorder(),
          ),
          value: ConvertHelper.makeStringListToStringWithLine(command),
          onChanged: (value) async {
            command = ConvertHelper.parseStringListFromStringWithLine(value);
            setState(() {});
          },
        ),
      ],
      actionBuilder: (context) => [
        TextButton(
          child: const Text('Cancel'),
          onPressed: () => Navigator.pop(context, false),
        ),
        TextButton(
          child: const Text('Continue'),
          onPressed: () => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
    if (canContinue) {
      await Provider.of<SettingProvider>(context, listen: false).state.mHandleCommand!(command);
    }
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._tabList = [];
    this._tabIndex = -1;
    {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      setting.state.mHomeShowLauncherPanel = this._showLauncherPanel;
      setting.state.mHomeShowForwarderPanel = this._showForwarderPanel;
      setting.state.mHomeShowCommanderPanel = this._showCommanderPanel;
      setting.state.mHomeInsertTabItem = this._insertTabItem;
    }
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
    SystemOverlayHelper.apply(Theme.of(context).colorScheme.brightness);
    return Scaffold(
      appBar: AppBar(
        systemOverlayStyle: SystemOverlayHelper.query(Theme.of(context).colorScheme.brightness),
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
              leading: Builder(
                builder: (context) => IconButton(
                  tooltip: 'Navigation',
                  icon: const Icon(IconSymbols.menu),
                  onPressed: () async {
                    Scaffold.of(context).openDrawer();
                  },
                ),
              ),
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
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.send_time_extension,
            label: 'Forwarder',
            action: const [],
            onPressed: () async {
              Navigator.pop(context);
              await this._showForwarderPanel();
            },
          ),
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.keyboard_command_key,
            label: 'Commander',
            action: const [],
            onPressed: () async {
              Navigator.pop(context);
              await this._showCommanderPanel();
            },
          ),
          const CustomNavigationDrawerDivider(),
          if (this._tabList.isEmpty)
            const SizedBox(height: 16),
          ...this._tabList.mapIndexed((index, item) => CustomNavigationDrawerItem(
            key: ObjectKey(item), // NOTE : fix button ripple effect error when remove item
            selected: index == this._tabIndex,
            icon: ModuleHelper.query(item.$2).icon,
            label: item.$1,
            action: [
              PopupMenuButton(
                tooltip: 'Action',
                position: PopupMenuPosition.under,
                icon: const Icon(IconSymbols.more_vert),
                itemBuilder: (context) => [
                  ('remove', 'Remove', IconSymbols.cancel),
                  ('rename', 'Rename', IconSymbols.draw),
                  ('keep', 'Keep', IconSymbols.pinboard),
                  ('duplicate', 'Duplicate', IconSymbols.tab_duplicate),
                ].map((value) => PopupMenuItem(
                  value: value.$1,
                  child: ListTile(
                    contentPadding: EdgeInsets.zero,
                    dense: true,
                    leading: Icon(value.$3),
                    title: Text(
                      value.$2,
                      overflow: TextOverflow.ellipsis,
                    ),
                  ),
                )).toList(),
                onSelected: (value) async {
                  switch (value) {
                    case 'remove':{
                      await this._removeTabItem(index);
                      break;
                    }
                    case 'rename':{
                      var title = item.$1;
                      var canContinue = await ControlHelper.showDialogAsModal<Boolean>(context, CustomModalDialog(
                        title: 'Tab Rename',
                        contentBuilder: (context, setState) => [
                          CustomTextField(
                            keyboardType: TextInputType.multiline,
                            inputFormatters: const [],
                            decoration: const InputDecoration(
                              contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                              filled: false,
                              border: OutlineInputBorder(),
                            ),
                            value: title,
                            onChanged: (value) async {
                              title = value;
                              setState(() {});
                            },
                          ),
                        ],
                        actionBuilder: (context) => [
                          TextButton(
                            child: const Text('Cancel'),
                            onPressed: () => Navigator.pop(context, false),
                          ),
                          TextButton(
                            child: const Text('Continue'),
                            onPressed: () => Navigator.pop(context, true),
                          ),
                        ],
                      )) ?? false;
                      if (canContinue) {
                        await this._renameTabItem(index, title);
                      }
                      break;
                    }
                    case 'keep':{
                      await this._keepTabItem(index);
                      break;
                    }
                    case 'duplicate':{
                      await this._duplicateTabItem(index);
                      break;
                    }
                    default: throw Exception();
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
              await ControlHelper.showBottomSheetAsModal<Void>(context, CustomModalBottomSheet(
                title: 'Setting',
                contentBuilder: (context, setState) => [
                  const SettingPanel(),
                ],
              ));
            },
          ),
          CustomNavigationDrawerItem(
            selected: false,
            icon: IconSymbols.info,
            label: 'About',
            action: const [],
            onPressed: () async {
              Navigator.pop(context);
              await ControlHelper.showBottomSheetAsModal<Void>(context, CustomModalBottomSheet(
                title: 'About',
                contentBuilder: (context, setState) => [
                  const AboutPanel(),
                ],
              ));
            },
          ),
          const SizedBox(height: 16),
        ],
      ),
      body: Stack(
        children: [
          Offstage(
            offstage: this._tabIndex != -1,
            child: const BlankPage(),
          ),
          ...this._tabList.mapIndexed((index, value) => Offstage(
            offstage: this._tabIndex != index,
            child: value.$3,
          )),
        ],
      ),
    );
  }

}
