import '/common.dart';
import '/setting.dart';
import '/utility/wrapper.dart';
import '/utility/command_line_reader.dart';
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

  @override
  createState() => _MainPageState();

  // ----------------

  final List<String> option;

}

class _MainPageState extends State<MainPage> {

  late List<MethodGroupConfiguration>                                                                           _methodConfiguration;
  late Boolean                                                                                                  _parallelForward;
  late List<(MethodGroupConfiguration, MethodConfiguration, Wrapper<Boolean>, List<Wrapper<ValueExpression?>>)> _command;
  late ScrollController                                                                                         _commandListScrollController;

  Future<Void> _appendCommand(
    String              methodId,
    Boolean             enableBatch,
    Map<String, Object> argumentValue,
  ) async {
    var groupConfiguration = this._methodConfiguration.firstWhere((value) => (methodId.startsWith('${value.id}.')));
    var itemConfiguration = groupConfiguration.item.firstWhere((value) => (methodId == value.id));
    this._command.add((groupConfiguration, itemConfiguration, Wrapper<Boolean>(enableBatch), ConfigurationHelper.parseArgumentValueListJson(itemConfiguration.argument, argumentValue)));
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
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._methodConfiguration = [];
    this._parallelForward = setting.data.mCommandSender.mParallelForward;
    this._command = [];
    this._commandListScrollController = ScrollController();
    ControlHelper.postTask(() async {
      this._methodConfiguration = ConfigurationHelper.parseDataFromJson(await JsonHelper.deserializeFile(setting.data.mCommandSender.mMethodConfiguration));
      var optionParallelForward = null as Boolean?;
      var optionCommand = null as List<(String, Boolean, Map<String, Object>)>?;
      var option = CommandLineReader(this.widget.option);
      if (option.check('-parallel_forward')) {
        optionParallelForward = option.nextBoolean();
      }
      if (option.check('-command')) {
        optionCommand = [];
        while (!option.done()) {
          optionCommand.add((
            option.nextString(),
            option.nextBoolean(),
            JsonHelper.deserializeText(option.nextString())!.asType<Map>().cast<String, Object>(),
          ));
        }
      }
      assertTest(option.done());
      if (optionParallelForward != null) {
        this._parallelForward = optionParallelForward;
      }
      if (optionCommand != null) {
        for (var optionCommandItem in optionCommand) {
          await this._appendCommand(optionCommandItem.$1, optionCommandItem.$2, optionCommandItem.$3);
        }
      }
      this.setState(() {});
    });
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
    return CustomModulePage(
      content: Column(
        children: [
          Expanded(
            child: Scrollbar(
              interactive: true,
              controller: this._commandListScrollController,
              child: ListView.builder(
                padding: const EdgeInsets.fromLTRB(8, 4, 8, 4),
                controller: this._commandListScrollController,
                itemCount: this._command.length,
                itemBuilder: (context, index) => Container(
                  padding: const EdgeInsets.fromLTRB(0, 4, 0, 4),
                  child: CommandPanel(
                    key: ObjectKey(this._command[index]),
                    groupConfiguration: this._command[index].$1,
                    itemConfiguration: this._command[index].$2,
                    enableBatch: this._command[index].$3,
                    argumentValue: this._command[index].$4,
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
      bottom: Row(
        children: [
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
            isSelected: false,
            icon: const Icon(IconSymbols.send),
            onPressed: () async {
              await this._forwardCommand(this._command.mapIndexed((index, value) => index).toList());
            },
          ),
          const SizedBox(width: 16),
          const Expanded(child: SizedBox()),
          const SizedBox(width: 16),
          FloatingActionButton(
            tooltip: 'Method',
            elevation: 0,
            focusElevation: 0,
            hoverElevation: 0,
            highlightElevation: 0,
            disabledElevation: 0,
            onPressed: () async {
              await ControlHelper.showCustomModalBottomSheet(
                context: context,
                title: 'Method',
                contentBuilder: (context, setState) => [
                  ListView.builder(
                    shrinkWrap: true,
                    physics: const NeverScrollableScrollPhysics(),
                    padding: const EdgeInsets.fromLTRB(0, 4, 0, 4),
                    itemCount: this._methodConfiguration.length,
                    itemBuilder: (context, index) => MethodGroupItem(
                      key: ObjectKey(this._methodConfiguration[index]),
                      configuration: this._methodConfiguration[index],
                      onSelect: (method) async {
                        Navigator.pop(context);
                        await this._appendCommand(method, false, {});
                      },
                    ),
                  ),
                ],
              );
            },
            child: const Icon(IconSymbols.format_list_bulleted_add),
          ),
        ],
      )
    );
  }

}
