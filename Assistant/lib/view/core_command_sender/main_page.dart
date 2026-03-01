import '/common.dart';
import '/utility/wrapper.dart';
import '/utility/json_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/core_command_sender/setting.dart';
import '/view/core_command_sender/configuration.dart';
import '/view/core_command_sender/value_expression.dart';
import '/view/core_command_sender/method_item.dart';
import '/view/core_command_sender/command_panel.dart';
import '/view/core_task_worker/forward_helper.dart' as core_task_worker;
import 'package:collection/collection.dart';
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

  late Boolean                                                                                                                                                                                   _parallelForward;
  late List<Boolean>                                                                                                                                                                             _methodExpanded;
  late List<({MethodGroupConfiguration groupConfiguration, MethodConfiguration itemConfiguration, Wrapper<Boolean> batch, List<Wrapper<ValueExpression?>> argument, Wrapper<Boolean> expanded})> _command;
  late ScrollController                                                                                                                                                                          _commandListScrollController;

  Future<Void> _appendCommand(
    String              method,
    Boolean             batch,
    Map<String, Object> argument,
    Boolean             expanded,
  ) async {
    var groupConfiguration = this.widget.configuration.method.firstWhere((value) => method.startsWith('${value.identifier}.'));
    var itemConfiguration = groupConfiguration.item.firstWhere((value) => method == value.identifier);
    this._command.add((
      groupConfiguration: groupConfiguration,
      itemConfiguration: itemConfiguration,
      batch: .new(batch),
      argument: ConfigurationHelper.parseArgumentValueListJson(itemConfiguration.argument, argument),
      expanded: .new(expanded),
    ));
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
      var method = core_task_worker.ForwardHelper.makeMethodMaybeBatch(item.itemConfiguration.identifier, item.batch.value);
      var argument = ConfigurationHelper.makeArgumentValueListJson(item.itemConfiguration.argument, item.argument);
      actualCommand.add(core_task_worker.ForwardHelper.makeArgumentForCommand(null, method, argument));
    }
    await core_task_worker.ForwardHelper.forwardMany(this.context, actualCommand, this._parallelForward);
    return;
  }

  // ----------------

  @override
  modulePageOpenView() async {
    this._methodExpanded = this.widget.configuration.method.map((value) => false).toList();
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
    var optionCommand = null as List<({String method, Boolean batch, Map<String, Object> argument, Boolean expanded})>?;
    var option = CommandLineReader(optionView);
    if (option.check('-parallel_forward')) {
      optionParallelForward = option.nextBoolean();
    }
    if (option.check('-command')) {
      optionCommand = [];
      while (!option.done()) {
        optionCommand.add((
          method: option.nextString(),
          batch: option.nextBoolean(),
          argument: option.nextString().selfLet((it) => JsonHelper.deserializeText(it)!.as<Map<dynamic, dynamic>>().cast<String, Object>()),
          expanded: option.nextBoolean(),
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
        await this._appendCommand(item.method, item.batch, item.argument, item.expanded);
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
        option.nextString(item.itemConfiguration.identifier);
        option.nextBoolean(item.batch.value);
        option.nextString(item.argument.selfLet((it) => JsonHelper.serializeText(ConfigurationHelper.makeArgumentValueListJson(item.itemConfiguration.argument, it), indented: false)));
        option.nextBoolean(item.expanded.value);
      }
    }
    return option.done();
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._parallelForward = this.widget.setting.parallelForward;
    this._methodExpanded = [];
    this._command = [];
    this._commandListScrollController = .new();
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
    this._commandListScrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return ModulePageRegion(
      onStorageDrop: null,
      content: FlexContainer.vertical([
        ListContainer.of(
          padding: .fromLTRB(16, 6, 16, 6),
          controller: this._commandListScrollController,
          itemCount: this._command.length,
          itemBuilder: (context, index) => BoxContainer.of(
            padding: .fromLTRB(0, 6, 0, 6),
            child: CommandPanel(
              key: ObjectKey(this._command[index]),
              groupConfiguration: this._command[index].groupConfiguration,
              itemConfiguration: this._command[index].itemConfiguration,
              batch: this._command[index].batch,
              argument: this._command[index].argument,
              expanded: this._command[index].expanded,
              onRemove: () async {
                await this._removeCommand(index);
              },
              onForward: () async {
                await this._forwardCommand([index]);
              },
            ),
          ),
        ).withStyledScrollBar(
          controller: this._commandListScrollController,
        ).withFlexExpanded(),
      ]),
      bottom: StyledBottomBar.standard(
        primary: StyledBadge.standard(
          label: StyledText.inherit('${this.widget.configuration.method.fold<Integer>(0, (currentValue, item) => currentValue + item.item.length)}'),
          child: StyledFloatingButton.standard(
            tooltip: 'Method',
            icon: IconView.of(IconSet.format_list_bulleted_add),
            onPressed: (context) async {
              await StyledBottomSheetExtension.show<Void>(context, StyledModalBottomSheet.standard(
                title: 'Method',
                contentBuilder: (context, setStateForPanel) => [
                  ListContainer.of(
                    shrink: true,
                    padding: .fromLTRB(0, 8, 0, 8),
                    itemCount: this.widget.configuration.method.length,
                    itemBuilder: (context, index) => MethodGroupItem(
                      configuration: this.widget.configuration.method[index],
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
          StyledIconButton.filledTonal(
            tooltip: 'Parallel Forward',
            selected: this._parallelForward,
            icon: IconView.of(IconSet.shuffle),
            iconOnSelected: IconView.of(IconSet.shuffle, fill: 1),
            onPressed: (context) async {
              this._parallelForward = !this._parallelForward;
              await refreshState(this.setState);
            },
          ),
          Gap.horizontal(8),
          StyledIconButton.filledTonal(
            tooltip: 'Forward',
            icon: IconView.of(IconSet.send, fill: 1),
            onPressed: (context) async {
              await this._forwardCommand(this._command.mapIndexed((index, value) => index).toList());
            },
          ),
        ],
      ),
    );
  }

}
