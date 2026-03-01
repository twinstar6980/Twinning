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

  late List<({String title, ModuleType type, Widget page})> _pageList;
  late Integer                                              _pageIndex;

  Future<Void> _insertPage(
    ModuleLauncherConfiguration configuration,
  ) async {
    while (Navigator.canPop(this.context)) {
      Navigator.pop(this.context);
    }
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var moduleSetting = ModuleHelper.query(configuration.type).querySetting(context);
    var moduleConfiguration = ModuleHelper.query(configuration.type).parseConfiguration((await JsonHelper.deserializeFile('${setting.data.moduleConfigurationDirectory}/${ModuleHelper.query(configuration.type).identifier}.json'))!);
    this._pageList.add((
      title: configuration.title,
      type: configuration.type,
      page: ModuleHelper.query(configuration.type).mainPage(
        moduleSetting,
        moduleConfiguration,
        configuration.option,
      ),
    ));
    if (this._pageIndex != -1) {
      await this._pageList[this._pageIndex].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageExitView();
    }
    this._pageIndex = this._pageList.length - 1;
    await refreshState(this.setState);
    await this._pageList[this._pageIndex].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageEnterView();
    await Future.delayed(.new(milliseconds: 10));
    return;
  }

  Future<Void> _removePage(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._pageList.length);
    if (!await this._pageList[index].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageCloseView()) {
      return;
    }
    if (this._pageIndex == index) {
      await this._pageList[this._pageIndex].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageExitView();
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
        await this._pageList[this._pageIndex].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageEnterView();
      }
    }
    await refreshState(this.setState);
    return;
  }

  Future<Void> _renamePage(
    Integer index,
    String  title,
  ) async {
    assertTest(0 <= index && index < this._pageList.length);
    this._pageList[index] = (
      title: title,
      type: this._pageList[index].type,
      page: this._pageList[index].page,
    );
    await refreshState(this.setState);
    return;
  }

  Future<Void> _memorizePage(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._pageList.length);
    var configuration = ModuleLauncherConfiguration(
      title: this._pageList[index].title,
      type: this._pageList[index].type,
      option: await this._pageList[index].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageCollectOption(),
    );
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    setting.data.moduleLauncher.pinned.add(configuration);
    await setting.save();
    return;
  }

  Future<Void> _duplicatePage(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._pageList.length);
    var configuration = ModuleLauncherConfiguration(
      title: this._pageList[index].title,
      type: this._pageList[index].type,
      option: await this._pageList[index].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageCollectOption(),
    );
    await this._insertPage(configuration);
    return;
  }

  Future<Void> _togglePage(
    Integer index,
  ) async {
    assertTest(0 <= index && index < this._pageList.length);
    await this._pageList[this._pageIndex].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageExitView();
    this._pageIndex = index;
    await refreshState(this.setState);
    await this._pageList[this._pageIndex].page.key!.as<GlobalKey>().currentState!.as<ModulePageState>().modulePageEnterView();
    return;
  }

  Future<Void> _showLauncher(
  ) async {
    await StyledBottomSheetExtension.show<Void>(this.context, StyledModalBottomSheet.standard(
      title: 'Launcher',
      contentBuilder: (context, setStateForPanel) => [
        LauncherPanel(
          onLaunch: (configuration) async {
            Navigator.pop(context);
            await this._insertPage(configuration);
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
                      content: StyledText.inherit(value.text),
                    )),
                  ));
                  if (action != null) {
                    switch (action) {
                      case 'remove': {
                        await this._removePage(index);
                        break;
                      }
                      case 'rename': {
                        var title = item.title;
                        var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
                          title: 'Rename',
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
                          await this._renamePage(index, title);
                        }
                        break;
                      }
                      case 'memorize': {
                        await this._memorizePage(index);
                        break;
                      }
                      case 'duplicate': {
                        await this._duplicatePage(index);
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
              await this._togglePage(index);
            },
          )),
          Gap.vertical(16),
        ],
      ),
    );
  }

}
