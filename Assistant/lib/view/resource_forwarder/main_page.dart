import '/common.dart';
import '/setting.dart';
import '/utility/control_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/json_helper.dart';
import '/utility/command_line_reader.dart';
import '/view/home/common.dart';
import '/view/resource_forwarder/configuration.dart';
import '/view/resource_forwarder/option_item.dart';
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

  late ScrollController                        _optionListScrollController;
  late List<OptionGroupConfiguration>          _optionConfiguration;
  late Boolean                                 _parallelExecute;
  late Boolean                                 _enableFilter;
  late Boolean                                 _enableBatch;
  late Boolean                                 _remainInput;
  late List<(String, Boolean?)>                _input;
  late List<List<(Boolean, Boolean, Boolean)>> _match;

  Future<Void> _refreshMatch(
  ) async {
    this._match.clear();
    for (var group in this._optionConfiguration) {
      var groupMatch = <(Boolean, Boolean, Boolean)>[];
      for (var item in group.item) {
        var singleMatched = false;
        var batchMatched = false;
        var nameMatched = false;
        if (this._input.length != 0) {
          singleMatched = true;
          batchMatched = true;
          nameMatched = true;
          var nameRule = RegExp(item.filter.name);
          for (var input in this._input) {
            singleMatched &= switch (item.filter.type) {
              OptionFilterFileObjectType.any       => input.$2 != null,
              OptionFilterFileObjectType.file      => input.$2 == false,
              OptionFilterFileObjectType.directory => input.$2 == true,
            };
            batchMatched &= item.batchable && input.$2 == true;
            nameMatched &= nameRule.hasMatch(input.$1);
          }
        }
        groupMatch.add((singleMatched, batchMatched, nameMatched));
      }
      this._match.add(groupMatch);
    }
    this.setState(() {});
    return;
  }

  Future<Void> _appendInput(
    List<String> list,
  ) async {
    for (var item in list) {
      if (this._input.any((value) => (value.$1 == item))) {
        continue;
      }
      var itemType = null as Boolean?;
      if (await StorageHelper.existFile(item)) {
        itemType = false;
      }
      if (await StorageHelper.existDirectory(item)) {
        itemType = true;
      }
      this._input.add((item, itemType));
    }
    await this._refreshMatch();
    return;
  }

  Future<Void> _removeInput(
    List<String> list,
  ) async {
    for (var item in list) {
      this._input.removeWhere((value) => (value.$1 == item));
    }
    await this._refreshMatch();
    return;
  }

  Future<Void> _clearInput(
  ) async {
    this._input.clear();
    await this._refreshMatch();
    return;
  }

  Future<Void> _executeForward(
    String?              method,
    Map<String, Object>? argument,
  ) async {
    var actualMethod = method == null ? null : '${method}${!this._enableBatch ? '' : '.batch'}';
    var actualCommand = this._input.map((value) => (modding_worker.ForwardHelper.makeArgumentForCommand(value.$1, actualMethod, argument))).toList();
    await modding_worker.ForwardHelper.forwardMany(this.context, actualCommand, this._parallelExecute);
    if (!this._remainInput) {
      await this._clearInput();
    }
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._optionListScrollController = ScrollController();
    this._optionConfiguration = [];
    this._parallelExecute = setting.data.mResourceForwarder.mParallelExecute;
    this._enableFilter = setting.data.mResourceForwarder.mEnableFilter;
    this._enableBatch = setting.data.mResourceForwarder.mEnableBatch;
    this._remainInput = setting.data.mResourceForwarder.mRemainInput;
    this._input = [];
    this._match = [];
    ControlHelper.postTask(() async {
      this._optionConfiguration = ConfigurationHelper.parseDataFromJson(await JsonHelper.deserializeFile(setting.data.mResourceForwarder.mOptionConfiguration));
      await this._refreshMatch();
      var optionParallelExecute = null as Boolean?;
      var optionEnableFilter = null as Boolean?;
      var optionEnableBatch = null as Boolean?;
      var optionRemainInput = null as Boolean?;
      var optionInput = null as List<String>?;
      var option = CommandLineReader(this.widget.option);
      if (option.check('-parallel_execute')) {
        optionParallelExecute = option.nextBoolean();
      }
      if (option.check('-enable_filter')) {
        optionEnableFilter = option.nextBoolean();
      }
      if (option.check('-enable_batch')) {
        optionEnableBatch = option.nextBoolean();
      }
      if (option.check('-remain_input')) {
        optionRemainInput = option.nextBoolean();
      }
      if (option.check('-input')) {
        optionInput = option.nextStringList();
      }
      assertTest(option.done());
      if (optionParallelExecute != null) {
        this._parallelExecute = optionParallelExecute;
      }
      if (optionEnableFilter != null) {
        this._enableFilter = optionEnableFilter;
      }
      if (optionEnableBatch != null) {
        this._enableBatch = optionEnableBatch;
      }
      if (optionRemainInput != null) {
        this._remainInput = optionRemainInput;
      }
      if (optionInput != null) {
        await this._appendInput(optionInput.map(StorageHelper.regularize).toList());
      }
      this.setState(() {});
    });
    return;
  }

  @override
  dispose() {
    this._optionListScrollController.dispose();
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
              controller: this._optionListScrollController,
              child: ListView(
                padding: const EdgeInsets.fromLTRB(4, 4, 4, 4),
                controller: this._optionListScrollController,
                children: this._optionConfiguration.mapIndexed((optionGroupIndex, optionGroup) => OptionGroupItem(
                  configuration: optionGroup,
                  match: this._match[optionGroupIndex],
                  enableFilter: this._enableFilter,
                  enableBatch: this._enableBatch,
                  onForward: this._executeForward,
                )).toList(),
              ),
            ),
          ),
        ],
      ),
      bottom: Row(
        children: [
          IconButton.filledTonal(
            tooltip: 'Parallel Execute',
            isSelected: this._parallelExecute,
            icon: const Icon(IconSymbols.shuffle),
            selectedIcon: const Icon(IconSymbols.shuffle, fill: 1),
            onPressed: () async {
              this._parallelExecute = !this._parallelExecute;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Enable Filter',
            isSelected: this._enableFilter,
            icon: const Icon(IconSymbols.filter_alt),
            selectedIcon: const Icon(IconSymbols.filter_alt, fill: 1),
            onPressed: () async {
              this._enableFilter = !this._enableFilter;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Enable Batch',
            isSelected: this._enableBatch,
            icon: const Icon(IconSymbols.stacks),
            selectedIcon: const Icon(IconSymbols.stacks, fill: 1),
            onPressed: () async {
              this._enableBatch = !this._enableBatch;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Remain Input',
            isSelected: this._remainInput,
            icon: const Icon(IconSymbols.keep),
            selectedIcon: const Icon(IconSymbols.keep_rounded, fill: 1),
            onPressed: () async {
              this._remainInput = !this._remainInput;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 16),
          const Expanded(child: SizedBox()),
          const SizedBox(width: 16),
          MenuAnchor(
            alignmentOffset: const Offset(0, 8),
            style: const MenuStyle(
              visualDensity: VisualDensity.standard,
            ),
            menuChildren: [
              ...this._input.reversed.map((value) => Tooltip(
                message: value.$1,
                child: MenuItemButton(
                  closeOnActivate: false,
                  onPressed: () async {
                    await this._removeInput([value.$1]);
                  },
                  leadingIcon: Icon(switch (value.$2) {
                    null  => IconSymbols.do_not_disturb_on,
                    false => IconSymbols.draft,
                    true  => IconSymbols.folder,
                  }),
                  child: Text(
                    StorageHelper.name(value.$1),
                    overflow: TextOverflow.ellipsis,
                  ),
                ),
              )),
              const Divider(),
              MenuItemButton(
                onPressed: () async {
                  var item = await StorageHelper.pickOpenFile(context, 'ResourceForwarder.Input');
                  if (item != null) {
                    await this._appendInput([item]);
                  }
                },
                leadingIcon: const Icon(IconSymbols.draft),
                child: const Text(
                  'Add File',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
              MenuItemButton(
                onPressed: () async {
                  var item = await StorageHelper.pickOpenDirectory(context, 'ResourceForwarder.Input');
                  if (item != null) {
                    await this._appendInput([item]);
                  }
                },
                leadingIcon: const Icon(IconSymbols.folder),
                child: const Text(
                  'Add Directory',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
            builder: (context, controller, child) => Badge.count(
              count: this._input.length,
              child: FloatingActionButton(
                tooltip: 'Input',
                elevation: 0,
                focusElevation: 0,
                hoverElevation: 0,
                highlightElevation: 0,
                disabledElevation: 0,
                onPressed: () async {
                  if (controller.isOpen) {
                    controller.close();
                  }
                  else {
                    controller.open();
                  }
                },
                child: const Icon(IconSymbols.attach_file),
              ),
            ),
          ),
        ],
      )
    );
  }

}
