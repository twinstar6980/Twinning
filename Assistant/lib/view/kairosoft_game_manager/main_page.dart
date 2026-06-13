import '/common.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/kairosoft_game_manager/setting.dart';
import '/view/kairosoft_game_manager/configuration.dart';
import '/view/kairosoft_game_manager/game_card.dart';
import '/view/kairosoft_game_manager/game_helper.dart';
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

  late Boolean               _gameLoading;
  late List<GameInformation> _gameInformation;
  late ScrollController      _gameListScrollController;

  Future<Void> _loadGame(
  ) async {
    this._gameInformation = [];
    this._gameLoading = true;
    await refreshState(this.setState);
    try {
      if (SystemChecker.isWindows) {
        this._gameInformation = await GameRepositoryHelper.loadWindowsSteamRepository(this.widget.setting.repositoryOfWindowsSteam);
      }
    }
    catch (e) {
      rethrow;
    }
    finally {
      this._gameLoading = false;
      await refreshState(this.setState);
    }
    return;
  }

  // ----------------

  @override
  modulePageOpenView() async {
    await this._loadGame();
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

  @override
  modulePageExecuteCommand(method, argument) async {
    var result = <String>[];
    switch (method) {
      default: throw UnsupportedException();
    }
    return result; // ignore: dead_code
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._gameLoading = false;
    this._gameInformation = [];
    this._gameListScrollController = .new();
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
    this._gameListScrollController.dispose();
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
          padding: .fromLTRB(0, 8, 0, 8),
          controller: this._gameListScrollController,
          itemCount: this._gameInformation.length,
          itemBuilder: (context, index) => GameCard(
            information: this._gameInformation[index],
          ),
        ).withStyledScrollBar(
          controller: this._gameListScrollController,
        ).withFlexExpanded(),
        FlexContainer.horizontal([
          StyledProgress.linear(
            value: !this._gameLoading ? 1.0 : null,
          ).withFlexExpanded(),
        ]),
      ]),
      bottom: StyledBottomBar.standard(
        primary: StyledBadge.standard(
          label: StyledText.inherit('${this._gameInformation.length}'),
          child: StyledFloatingButton.standard(
            tooltip: 'Reload',
            icon: IconView.of(IconSet.refresh),
            onPressed: (context) async {
              await _loadGame();
            },
          ),
        ),
        secondary: [
        ],
      ),
    );
  }

}
