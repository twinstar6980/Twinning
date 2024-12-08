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
  late List<Boolean>                                                                                                              _methodCollapse;
  late List<(MethodGroupConfiguration, MethodConfiguration, Wrapper<Boolean>, List<Wrapper<ValueExpression?>>, Wrapper<Boolean>)> _command;
  late ScrollController                                                                                                           _commandListScrollController;

  Future<Void> _appendCommand(
    String              methodId,
    Boolean             enableBatch,
    Map<String, Object> argumentValue,
    Boolean             collapse,
  ) async {
    var groupConfiguration = this._methodConfiguration.firstWhere((value) => methodId.startsWith('${value.id}.'));
    var itemConfiguration = groupConfiguration.item.firstWhere((value) => methodId == value.id);
    this._command.add((groupConfiguration, itemConfiguration, Wrapper(enableBatch), ConfigurationHelper.parseArgumentValueListJson(itemConfiguration.argument, argumentValue), Wrapper(collapse)));
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    this._commandListScrollController.jumpTo(this._commandListScrollController.position.maxScrollExtent);
    return;
  }

  Future<Void> _removeCommand(
    Integer index,
  ) async {
    this._command.removeAt(index);
    this.setState(() {});
    return;
  }

  Future<Void> _forwardCommand(
    List<Integer> index,
  ) async {
    var actualCommand = <List<String>>[];
    for (var itemIndex in index) {
      var item = this._command[itemIndex];
      var method = modding_worker.ForwardHelper.makeMethodForBatchable(item.$2.id, item.$3.value);
      var argument = ConfigurationHelper.makeArgumentValueListJson(item.$2.argument, item.$4);
      actualCommand.add(modding_worker.ForwardHelper.makeArgumentForCommand(null, method, argument));
    }
    await modding_worker.ForwardHelper.forwardMany(this.context, actualCommand, this._parallelForward);
    return;
  }

  // ----------------

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
    assertTest(option.done());
    if (optionParallelForward != null) {
      this._parallelForward = optionParallelForward;
    }
    if (optionCommand != null) {
      for (var item in optionCommand) {
        await this._appendCommand(item.$1, item.$2, item.$3, item.$4);
      }
    }
    this.setState(() {});
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

  @override
  modulePageRequestClose() async {
    return true;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._methodConfiguration = [];
    this._parallelForward = setting.data.mCommandSender.mParallelForward;
    this._methodCollapse = [];
    this._command = [];
    this._commandListScrollController = ScrollController();
    ControlHelper.postTask(() async {
      this._methodConfiguration = ConfigurationHelper.parseDataFromJson(await JsonHelper.deserializeFile(setting.data.mCommandSender.mMethodConfiguration));
      this._methodCollapse = this._methodConfiguration.map((value) => true).toList();
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
          Expanded(
            child: Scrollbar(
              interactive: true,
              controller: this._commandListScrollController,
              child: ListView.builder(
                padding: const EdgeInsets.fromLTRB(16, 6, 16, 6),
                controller: this._commandListScrollController,
                itemCount: this._command.length,
                itemBuilder: (context, index) => Container(
                  padding: const EdgeInsets.fromLTRB(0, 6, 0, 6),
                  child: CommandPanel(
                    key: ObjectKey(this._command[index]),
                    groupConfiguration: this._command[index].$1,
                    itemConfiguration: this._command[index].$2,
                    enableBatch: this._command[index].$3,
                    argumentValue: this._command[index].$4,
                    collapse: this._command[index].$5,
                    onRemove: () async {
                      await this._removeCommand(index);
                    },
                    onForward: () async {
                      await this._forwardCommand([index]);
                    },
                  ),
                ),
              ),
            ),
          ),
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
            child: const Icon(IconSymbols.format_list_bulleted_add),
            onPressed: () async {
              await ControlHelper.showBottomSheetAsModal<Void>(context, CustomModalBottomSheet(
                title: 'Method',
                contentBuilder: (context, setState) => [
                  ListView.builder(
                    shrinkWrap: true,
                    physics: const NeverScrollableScrollPhysics(),
                    padding: const EdgeInsets.fromLTRB(0, 8, 0, 8),
                    itemCount: this._methodConfiguration.length,
                    itemBuilder: (context, index) => MethodGroupItem(
                      configuration: this._methodConfiguration[index],
                      onSelect: (method) async {
                        Navigator.pop(context);
                        await this._appendCommand(method, false, {}, false);
                      },
                      collapse: this._methodCollapse[index],
                      onToggle: () async {
                        this._methodCollapse[index] = !this._methodCollapse[index];
                        setState(() {});
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
            icon: const Icon(IconSymbols.shuffle),
            selectedIcon: const Icon(IconSymbols.shuffle, fill: 1),
            onPressed: () async {
              this._parallelForward = !this._parallelForward;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Forward',
            icon: const Icon(IconSymbols.send, fill: 1),
            onPressed: () async {
              await this._forwardCommand(this._command.mapIndexed((index, value) => index).toList());
            },
          ),
        ],
      ),
    );
  }

}
