import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/json_helper.dart';
import '/widget/export.dart';
import '/view/home/blank_page.dart';
import '/view/home/launcher_panel.dart';
import '/view/home/onboarding_panel.dart';
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

  late List<({GlobalKey key, String title, ModuleType type, Widget page})> _pageList;
  late Integer                                                             _pageIndex;

  Future<Void> _insertPage(
    GlobalKey                   key,
    ModuleLauncherConfiguration configuration,
    Boolean                     slience,
  ) async {
    if (!slience) {
      while (Navigator.canPop(this.context)) {
        Navigator.pop(this.context);
      }
    }
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var moduleInformation = ModuleHelper.query(configuration.type);
    var moduleSetting = moduleInformation.querySetting(context);
    var moduleConfiguration = moduleInformation.parseConfiguration((await JsonHelper.decodeFile(setting.data.moduleConfigurationDirectory.join('${moduleInformation.identifier}.json')))!);
    this._pageList.add((
      key: key,
      title: configuration.title,
      type: configuration.type,
      page: moduleInformation.buildMainPage(
        key,
        moduleSetting,
        moduleConfiguration,
        configuration.option,
      ),
    ));
    if (!slience) {
      if (this._pageIndex != -1) {
        await this._pageList[this._pageIndex].key.currentState!.as<ModulePageState>().modulePageExitView();
      }
      this._pageIndex = this._pageList.length - 1;
    }
    await refreshState(this.setState);
    if (!slience) {
      await this._pageList[this._pageIndex].key.currentState!.as<ModulePageState>().modulePageEnterView();
    }
    await Future.delayed(.new(milliseconds: 10));
    return;
  }

  Future<Void> _removePage(
    GlobalKey key,
  ) async {
    var index = this._pageList.indexWhere((it) => it.key == key);
    assertTest(index != -1);
    if (!await this._pageList[index].key.currentState!.as<ModulePageState>().modulePageCloseView()) {
      return;
    }
    if (this._pageIndex == index) {
      await this._pageList[this._pageIndex].key.currentState!.as<ModulePageState>().modulePageExitView();
    }
    this._pageList.removeAt(index);
    if (this._pageIndex > index) {
      this._pageIndex--;
    }
    else if (this._pageIndex == index) {
      if (this._pageIndex == this._pageList.length) {
        this._pageIndex--;
      }
      if (this._pageIndex != -1) {
        await this._pageList[this._pageIndex].key.currentState!.as<ModulePageState>().modulePageEnterView();
      }
    }
    await refreshState(this.setState);
    return;
  }

  Future<Void> _renamePage(
    GlobalKey key,
    String    title,
  ) async {
    var index = this._pageList.indexWhere((it) => it.key == key);
    assertTest(index != -1);
    this._pageList[index] = (
      key: this._pageList[index].key,
      title: title,
      type: this._pageList[index].type,
      page: this._pageList[index].page,
    );
    await refreshState(this.setState);
    return;
  }

  Future<Void> _memorizePage(
    GlobalKey key,
  ) async {
    var index = this._pageList.indexWhere((it) => it.key == key);
    assertTest(index != -1);
    var configuration = ModuleLauncherConfiguration(
      title: this._pageList[index].title,
      type: this._pageList[index].type,
      option: await this._pageList[index].key.currentState!.as<ModulePageState>().modulePageCollectOption(),
    );
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    setting.data.moduleLauncher.pinned.add(configuration);
    await setting.save();
    return;
  }

  Future<Void> _duplicatePage(
    GlobalKey key,
  ) async {
    var index = this._pageList.indexWhere((it) => it.key == key);
    assertTest(index != -1);
    var configuration = ModuleLauncherConfiguration(
      title: this._pageList[index].title,
      type: this._pageList[index].type,
      option: await this._pageList[index].key.currentState!.as<ModulePageState>().modulePageCollectOption(),
    );
    await this._insertPage(.new(), configuration, false);
    return;
  }

  Future<Void> _togglePage(
    GlobalKey key,
  ) async {
    var index = this._pageList.indexWhere((it) => it.key == key);
    assertTest(index != -1);
    await this._pageList[this._pageIndex].key.currentState!.as<ModulePageState>().modulePageExitView();
    this._pageIndex = index;
    await refreshState(this.setState);
    await this._pageList[this._pageIndex].key.currentState!.as<ModulePageState>().modulePageEnterView();
    return;
  }

  Future<Void> _showLauncher(
  ) async {
    await StyledModalBottomSheetExtension.show<Void>(this.context, StyledModalBottomSheet.standard(
      title: 'Launcher',
      contentBuilder: (context, setStateForPanel) => [
        LauncherPanel(
          onLaunch: (configuration) async {
            Navigator.pop(context);
            await this._insertPage(.new(), configuration, false);
          },
        ),
      ],
    ));
    return;
  }

  Future<Void> _showOnboarding(
  ) async {
    await StyledModalDialogExtension.show<String>(this.context, StyledModalDialog.standard(
      title: 'Onboarding',
      contentBuilder: (context, setStateForPanel) => [
        OnboardingPanel(
          onDone: () async {
            Navigator.pop(context);
          },
        ),
      ],
      actionBuilder: null,
    ));
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._pageList = [];
    this._pageIndex = -1;
    {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      setting.state.homeShowOnboarding = this._showOnboarding;
      setting.state.homeShowLauncher = this._showLauncher;
      setting.state.homeInsertPage = this._insertPage;
      setting.state.homeRemovePage = this._removePage;
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
      title: StorageDropRegion(
        onDrop: (item) async {
          var setting = Provider.of<SettingProvider>(this.context, listen: false);
          await setting.state.handleForward!(item);
        },
        child: StyledTitleBar.standard(
          title: this._pageList.isEmpty ? 'Home' : this._pageList[this._pageIndex].title,
          leading: StyledIconButton.standard(
            tooltip: 'Navigation',
            icon: IconView.of(IconSet.menu),
            onPressed: (context) async {
              await StyledNavigationDrawerExtension.show(context);
            },
          ),
          trailing: null,
        ),
      ),
      body: StackContainer.at(this._pageIndex + 1, [
        BlankPage(),
        ...this._pageList.mapIndexed((index, value) => value.page),
      ]),
      drawer: StyledNavigationDrawer.standard(
        content: [
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
              await this._showLauncher();
            },
          ),
          NavigationDrawerDivider(),
          if (this._pageList.isEmpty) ...[
            Gap.vertical(16),
          ],
          ...this._pageList.mapIndexed((index, item) => NavigationDrawerItem(
            key: ObjectKey(item), // fix button ripple effect error when remove item
            selected: index == this._pageIndex,
            icon: ModuleHelper.query(item.type).icon,
            label: item.title,
            action: [
              StyledIconButton.standard(
                tooltip: 'Action',
                icon: IconView.of(IconSet.steppers),
                onPressed: (context) async {
                  var action = await StyledMenuExtension.show<String>(context, StyledMenu.standard(
                    position: .under,
                    content: <({String value, String text, IconData icon})>[
                      (value: 'remove', text: 'Remove', icon: IconSet.tab_close_right),
                      (value: 'rename', text: 'Rename', icon: IconSet.label),
                      (value: 'memorize', text: 'Memorize', icon: IconSet.bookmark_add),
                      (value: 'duplicate', text: 'Duplicate', icon: IconSet.copy_all),
                    ].map((value) => StyledMenuItem.standard(
                      value: value.value,
                      leading: IconView.of(value.icon),
                      content: StyledText.inherit(tooltip: true, value.text),
                    )),
                  ));
                  if (action != null) {
                    switch (action) {
                      case 'remove': {
                        await this._removePage(item.key);
                        break;
                      }
                      case 'rename': {
                        var title = await MoreModalDialogExtension.showForRename(context, item.title, null);
                        if (title != null) {
                          await this._renamePage(item.key, title);
                        }
                        break;
                      }
                      case 'memorize': {
                        await this._memorizePage(item.key);
                        break;
                      }
                      case 'duplicate': {
                        await this._duplicatePage(item.key);
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
              await this._togglePage(item.key);
            },
          )),
          Gap.vertical(16),
        ],
      ),
    );
  }

}
