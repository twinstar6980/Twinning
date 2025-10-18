import '/common.dart';
import '/setting.dart';
import '/utility/wrapper.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/utility/json_helper.dart';
import '/utility/control_helper.dart';
import '/view/home/common.dart';
import '/view/command_sender/value_expression.dart';
import '/view/command_sender/configuration.dart';
import '/view/command_sender/method_item.dart';
import '/view/command_sender/command_panel.dart';
import '/view/modding_worker/forward_helper.dart' as modding_worker;
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.option,
  });

  // ----------------

  final List<String> option;

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> implements CustomModulePageState {

  late List<MethodGroupConfiguration>                                                                                             _methodConfiguration;
  late Boolean                                                                                                                    _parallelForward;
  late List<Boolean>                                                                                                              _methodExpanded;
  late List<(MethodGroupConfiguration, MethodConfiguration, Wrapper<Boolean>, List<Wrapper<ValueExpression?>>, Wrapper<Boolean>)> _command;
  late ScrollController                                                                                                           _commandListScrollController;

  Future<Void> _appendCommand(
    String              method,
    Boolean             batch,
    Map<String, Object> argument,
    Boolean             expanded,
  ) async {
    var groupConfiguration = this._methodConfiguration.firstWhere((value) => method.startsWith('${value.id}.'));
    var itemConfiguration = groupConfiguration.item.firstWhere((value) => method == value.id);
    this._command.add((groupConfiguration, itemConfiguration, Wrapper(batch), ConfigurationHelper.parseArgumentValueListJson(itemConfiguration.argument, argument), Wrapper(expanded)));
    await refreshState(this.setState);
    this._commandListScrollController.jumpTo(this._commandListScrollController.position.maxScrollExtent);
    return;
  }

  Future<Void> _removeCommand(
    Integer index,
  ) async {
    this._command.removeAt(index);
    await refreshState(this.setState);
    return;
  }

  Future<Void> _forwardCommand(
    List<Integer> index,
  ) async {
    var actualCommand = <List<String>>[];
    for (var itemIndex in index) {
      var item = this._command[itemIndex];
      var method = modding_worker.ForwardHelper.makeMethodMaybeBatch(item.$2.id, item.$3.value);
      var argument = ConfigurationHelper.makeArgumentValueListJson(item.$2.argument, item.$4);
      actualCommand.add(modding_worker.ForwardHelper.makeArgumentForCommand(null, method, argument));
    }
    await modding_worker.ForwardHelper.forwardMany(this.context, actualCommand, this._parallelForward);
    return;
  }

  // ----------------

  @override
  modulePageOpenView() async {
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._methodConfiguration = ConfigurationHelper.parseDataFromJson(await JsonHelper.deserializeFile(setting.data.commandSender.methodConfiguration));
    this._methodExpanded = this._methodConfiguration.map((value) => false).toList();
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
    var optionParallelForward = null as Boolean?;
    var optionCommand = null as List<(String, Boolean, Map<String, Object>, Boolean)>?;
    var option = CommandLineReader(optionView);
    if (option.check('-parallel_forward')) {
      optionParallelForward = option.nextBoolean();
    }
    if (option.check('-command')) {
      optionCommand = [];
      while (!option.done()) {
        optionCommand.add((
          option.nextString(),
          option.nextBoolean(),
          option.nextString().selfLet((it) => JsonHelper.deserializeText(it)!.as<Map<dynamic, dynamic>>().cast<String, Object>()),
          option.nextBoolean(),
        ));
      }
    }
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
    if (optionParallelForward != null) {
      this._parallelForward = optionParallelForward;
    }
    if (optionCommand != null) {
      for (var item in optionCommand) {
        await this._appendCommand(item.$1, item.$2, item.$3, item.$4);
      }
    }
    await refreshState(this.setState);
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = CommandLineWriter();
    if (option.check('-parallel_forward')) {
      option.nextBoolean(this._parallelForward);
    }
    if (option.check('-command')) {
      for (var item in this._command) {
        option.nextString(item.$2.id);
        option.nextBoolean(item.$3.value);
        option.nextString(item.$4.selfLet((it) => JsonHelper.serializeText(ConfigurationHelper.makeArgumentValueListJson(item.$2.argument, it), indented: false)));
        option.nextBoolean(item.$5.value);
      }
    }
    return option.done();
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._methodConfiguration = [];
    this._parallelForward = setting.data.commandSender.parallelForward;
    this._methodExpanded = [];
    this._command = [];
    this._commandListScrollController = ScrollController();
    ControlHelper.postTask(() async {
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
    this._commandListScrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return CustomModulePageRegion(
      onDropFile: null,
      content: Column(
        children: [
          ListView.builder(
            padding: EdgeInsets.fromLTRB(16, 6, 16, 6),
            controller: this._commandListScrollController,
            itemCount: this._command.length,
            itemBuilder: (context, index) => Container(
              padding: EdgeInsets.fromLTRB(0, 6, 0, 6),
              child: CommandPanel(
                key: ObjectKey(this._command[index]),
                groupConfiguration: this._command[index].$1,
                itemConfiguration: this._command[index].$2,
                batch: this._command[index].$3,
                argument: this._command[index].$4,
                expanded: this._command[index].$5,
                onRemove: () async {
                  await this._removeCommand(index);
                },
                onForward: () async {
                  await this._forwardCommand([index]);
                },
              ),
            ),
          ).withScrollbar(
            controller: this._commandListScrollController,
          ).withExpanded(),
        ],
      ),
      bottom: CustomBottomBarContent(
        primary: Badge.count(
          count: this._methodConfiguration.fold(0, (currentValue, item) => currentValue + item.item.length),
          child: FloatingActionButton(
            tooltip: 'Method',
            elevation: 0,
            focusElevation: 0,
            hoverElevation: 0,
            highlightElevation: 0,
            disabledElevation: 0,
            child: Icon(IconSymbols.format_list_bulleted_add),
            onPressed: () async {
              await ControlHelper.showBottomSheetAsModal<Void>(context, CustomModalBottomSheet(
                title: 'Method',
                contentBuilder: (context, setStateForPanel) => [
                  ListView.builder(
                    shrinkWrap: true,
                    physics: NeverScrollableScrollPhysics(),
                    padding: EdgeInsets.fromLTRB(0, 8, 0, 8),
                    itemCount: this._methodConfiguration.length,
                    itemBuilder: (context, index) => MethodGroupItem(
                      configuration: this._methodConfiguration[index],
                      onSelect: (method) async {
                        Navigator.pop(context);
                        await this._appendCommand(method, false, {}, true);
                      },
                      expanded: this._methodExpanded[index],
                      onToggle: () async {
                        this._methodExpanded[index] = !this._methodExpanded[index];
                        await refreshState(setStateForPanel);
                      },
                    ),
                  ),
                ],
              ));
            },
          ),
        ),
        secondary: [
          IconButton.filledTonal(
            tooltip: 'Parallel Forward',
            isSelected: this._parallelForward,
            icon: Icon(IconSymbols.shuffle),
            selectedIcon: Icon(IconSymbols.shuffle, fill: 1),
            onPressed: () async {
              this._parallelForward = !this._parallelForward;
              await refreshState(this.setState);
            },
          ),
          SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Forward',
            icon: Icon(IconSymbols.send, fill: 1),
            onPressed: () async {
              await this._forwardCommand(this._command.mapIndexed((index, value) => index).toList());
            },
          ),
        ],
      ),
    );
  }

}
