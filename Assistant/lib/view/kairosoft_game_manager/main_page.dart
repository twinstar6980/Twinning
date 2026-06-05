import '/common.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/utility/kairosoft_game_helper.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/kairosoft_game_manager/setting.dart';
import '/view/kairosoft_game_manager/configuration.dart';
import '/view/kairosoft_game_manager/game_card.dart';
import 'package:flutter/widgets.dart';

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.setting,
    required this.configuration,
    required this.option,
  });

  // ----------------

  final Setting       setting;
  final Configuration configuration;
  final List<String>  option;

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> implements ModulePageState {

  late List<GameInformation> _gameInformation;

  // ----------------

  @override
  modulePageOpenView() async {
    if (!this.widget.setting.repository.isEmpty) {
      this._gameInformation = await GameRepositoryHelper.loadSteamRepository(this.widget.setting.repository.first);
    }
    return;
  }

  @override
  modulePageCloseView() async {
    return true;
  }

  @override
  modulePageEnterView() async {
    return;
  }

  @override
  modulePageExitView() async {
    return;
  }

  @override
  modulePageApplyOption(optionView) async {
    var option = CommandLineReader(optionView);
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
    await refreshState(this.setState);
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = CommandLineWriter();
    return option.done();
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._gameInformation = [];
    postTask(() async {
      await this.modulePageOpenView();
      await this.modulePageApplyOption(this.widget.option);
    });
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
    return ModulePageRegion(
      onStorageDrop: (item) async {
        return;
      },
      content: FlexContainer.vertical([
        ListContainer.of(
          itemCount: this._gameInformation.length,
          itemBuilder: (context, index) => GameCard(
            information: this._gameInformation[index],
            onX: () {},
          ),
        ).withFlexExpanded(),
      ]),
      bottom: StyledBottomBar.standard(
        primary: null,
        secondary: [
        ],
      ),
    );
  }

}
