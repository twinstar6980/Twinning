import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/json_helper.dart';
import '/widget/export.dart';
import '/view/home/blank_page.dart';
import '/view/home/launcher_panel.dart';
import '/view/home/setting_panel.dart';
import '/view/home/module_page.dart';
import 'package:collection/collection.dart';
import 'package:provider/provider.dart';
import 'package:flutter/widgets.dart';

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
    while (Navigator.canPop(this.context)) {
      Navigator.pop(this.context);
    }
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var moduleSetting = ModuleHelper.query(configuration.type).querySetting(context);
    var moduleConfiguration = ModuleHelper.query(configuration.type).parseConfiguration((await JsonHelper.deserializeFile('${setting.data.moduleConfigurationDirectory}/${configuration.type.name}.json'))!);
    this._tabList.add((
      configuration.title,
      configuration.type,
      ModuleHelper.query(configuration.type).mainPage(
        moduleSetting,
        moduleConfiguration,
        configuration.option,
      ),
    ));
    if (this._tabIndex != -1) {
      await this._tabList[this._tabIndex].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageExitView();
    }
    this._tabIndex = this._tabList.length - 1;
    await refreshState(this.setState);
    await this._tabList[this._tabIndex].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageEnterView();
    await Future.delayed(.new(milliseconds: 10));
    return;
  }

  Future<Void> _removeTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    if (!await this._tabList[index].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageCloseView()) {
      return;
    }
    if (this._tabIndex == index) {
      await this._tabList[this._tabIndex].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageExitView();
    }
    this._tabList.removeAt(index);
    if (this._tabIndex > index) {
      this._tabIndex--;
    }
    else if (this._tabIndex == index) {
      if (this._tabIndex == this._tabList.length) {
        this._tabIndex--;
      }
      if (this._tabIndex != -1) {
        await this._tabList[this._tabIndex].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageEnterView();
      }
    }
    await refreshState(this.setState);
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
    await refreshState(this.setState);
    return;
  }

  Future<Void> _keepTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    var configuration = ModuleLauncherConfiguration(
      title: this._tabList[index].$1,
      type: this._tabList[index].$2,
      option: await this._tabList[index].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageCollectOption(),
    );
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    setting.data.moduleLauncher.pinned.add(configuration);
    await setting.save();
    return;
  }

  Future<Void> _duplicateTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    var configuration = ModuleLauncherConfiguration(
      title: this._tabList[index].$1,
      type: this._tabList[index].$2,
      option: await this._tabList[index].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageCollectOption(),
    );
    await this._insertTabItem(configuration);
    return;
  }

  Future<Void> _toggleTabItem(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._tabList.length);
    await this._tabList[this._tabIndex].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageExitView();
    this._tabIndex = index;
    await refreshState(this.setState);
    await this._tabList[this._tabIndex].$3.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageEnterView();
    return;
  }

  Future<Void> _showLauncherPanel(
  ) async {
    await StyledBottomSheetExtension.show<Void>(this.context, StyledModalBottomSheet.standard(
      title: 'Launcher',
      contentBuilder: (context, setStateForPanel) => [
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

  // ----------------

  @override
  initState() {
    super.initState();
    this._tabList = [];
    this._tabIndex = -1;
    {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      setting.state.homeShowLauncherPanel = this._showLauncherPanel;
      setting.state.homeInsertTabItem = this._insertTabItem;
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
    return StyledScaffold.standard(
      title: StyledTitleBar.standard(
        title: this._tabList.isEmpty ? 'Home' : this._tabList[this._tabIndex].$1,
        leading: StyledIconButton.standard(
          tooltip: 'Navigation',
          icon: IconView.of(IconSet.menu),
          onPressed: (context) async {
            await StyledNavigationDrawerExtension.show(context);
          },
        ),
      ),
      body: StackContainer.at(this._tabIndex + 1, [
        BlankPage(),
        ...this._tabList.mapIndexed((index, value) => value.$3),
      ]),
      drawer: StyledNavigationDrawer.standard(
        children: [
          Gap.vertical(16),
          NavigationDrawerLabel(
            label: 'Assistant',
          ),
          NavigationDrawerItem(
            selected: false,
            icon: IconSet.settings,
            label: 'Setting',
            action: [],
            onPressed: (context) async {
              Navigator.pop(context);
              await StyledFullDialogExtension.show<Void>(context, StyledFullDialog.standard(
                title: 'Setting',
                contentBuilder: (context, setStateForPanel) => [
                  SettingPanel(),
                ],
              ));
            },
          ),
          NavigationDrawerItem(
            selected: false,
            icon: IconSet.widgets,
            label: 'Launcher',
            action: [],
            onPressed: (context) async {
              Navigator.pop(context);
              await this._showLauncherPanel();
            },
          ),
          NavigationDrawerDivider(),
          if (this._tabList.isEmpty) ...[
            Gap.vertical(16),
          ],
          ...this._tabList.mapIndexed((index, item) => NavigationDrawerItem(
            key: ObjectKey(item), // fix button ripple effect error when remove item
            selected: index == this._tabIndex,
            icon: ModuleHelper.query(item.$2).icon,
            label: item.$1,
            action: [
              StyledIconButton.standard(
                tooltip: 'Action',
                icon: IconView.of(IconSet.steppers),
                onPressed: (context) async {
                  var action = await StyledMenuExtension.show<String>(context, StyledMenu.standard(
                    position: .under,
                    children: [
                      ('remove', 'Remove', IconSet.cancel),
                      ('rename', 'Rename', IconSet.draw),
                      ('keep', 'Keep', IconSet.pinboard),
                      ('duplicate', 'Duplicate', IconSet.tab_duplicate),
                    ].map((value) => StyledMenuItem.standard(
                      value: value.$1,
                      leading: IconView.of(value.$3),
                      content: StyledText.inherit(value.$2),
                    )),
                  ));
                  if (action != null) {
                    switch (action) {
                      case 'remove': {
                        await this._removeTabItem(index);
                        break;
                      }
                      case 'rename': {
                        var title = item.$1;
                        var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
                          title: 'Tab Rename',
                          contentBuilder: (context, setStateForPanel) => [
                            StyledInput.outlined(
                              type: .multiline,
                              format: null,
                              hint: null,
                              prefix: null,
                              suffix: null,
                              value: title,
                              onChanged: (context, value) async {
                                title = value;
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
                          await this._renameTabItem(index, title);
                        }
                        break;
                      }
                      case 'keep': {
                        await this._keepTabItem(index);
                        break;
                      }
                      case 'duplicate': {
                        await this._duplicateTabItem(index);
                        break;
                      }
                      default: throw UnreachableException();
                    }
                  }
                },
              ),
            ],
            onPressed: (context) async {
              Navigator.pop(context);
              await this._toggleTabItem(index);
            },
          )),
          Gap.vertical(16),
        ],
      ),
    );
  }

}
