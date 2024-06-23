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

  late List<OptionGroupConfiguration>          _optionConfiguration;
  late Boolean                                 _parallelForward;
  late Boolean                                 _enableFilter;
  late Boolean                                 _enableBatch;
  late List<(String, Boolean?)>                _resource;
  late List<List<(Boolean, Boolean, Boolean)>> _optionMatch;
  late ScrollController                        _optionListScrollController;

  Future<Void> _refreshMatch(
  ) async {
    this._optionMatch.clear();
    for (var group in this._optionConfiguration) {
      var groupMatch = <(Boolean, Boolean, Boolean)>[];
      for (var item in group.item) {
        var singleMatched = false;
        var batchMatched = false;
        var nameMatched = false;
        if (this._resource.length != 0) {
          singleMatched = true;
          batchMatched = true;
          nameMatched = true;
          var nameRule = RegExp(item.filter.name);
          for (var resource in this._resource) {
            singleMatched &= switch (item.filter.type) {
              FilterType.any       => resource.$2 != null,
              FilterType.file      => resource.$2 == false,
              FilterType.directory => resource.$2 == true,
            };
            batchMatched &= item.batchable && resource.$2 == true;
            nameMatched &= nameRule.hasMatch(resource.$1);
          }
        }
        groupMatch.add((singleMatched, batchMatched, nameMatched));
      }
      this._optionMatch.add(groupMatch);
    }
    this.setState(() {});
    return;
  }

  Future<Void> _appendResource(
    List<String> list,
  ) async {
    for (var item in list) {
      if (this._resource.any((value) => value.$1 == item)) {
        continue;
      }
      var itemType = null as Boolean?;
      if (await StorageHelper.existFile(item)) {
        itemType = false;
      }
      if (await StorageHelper.existDirectory(item)) {
        itemType = true;
      }
      this._resource.add((item, itemType));
    }
    await this._refreshMatch();
    return;
  }

  Future<Void> _removeResource(
    List<String> list,
  ) async {
    for (var item in list) {
      this._resource.removeWhere((value) => value.$1 == item);
    }
    await this._refreshMatch();
    return;
  }

  Future<Void> _clearResource(
  ) async {
    this._resource.clear();
    await this._refreshMatch();
    return;
  }

  Future<Void> _forwardResource(
    String?              method,
    Map<String, Object>? argument,
  ) async {
    var actualMethod = method == null ? null : modding_worker.ForwardHelper.makeMethodForBatchable(method, this._enableBatch);
    var actualCommand = this._resource.map((value) => modding_worker.ForwardHelper.makeArgumentForCommand(value.$1, actualMethod, argument)).toList();
    await modding_worker.ForwardHelper.forwardMany(this.context, actualCommand, this._parallelForward);
    return;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._optionConfiguration = [];
    this._parallelForward = setting.data.mResourceForwarder.mParallelForward;
    this._enableFilter = setting.data.mResourceForwarder.mEnableFilter;
    this._enableBatch = setting.data.mResourceForwarder.mEnableBatch;
    this._resource = [];
    this._optionMatch = [];
    this._optionListScrollController = ScrollController();
    ControlHelper.postTask(() async {
      this._optionConfiguration = ConfigurationHelper.parseDataFromJson(await JsonHelper.deserializeFile(setting.data.mResourceForwarder.mOptionConfiguration));
      await this._refreshMatch();
      var optionParallelForward = null as Boolean?;
      var optionEnableFilter = null as Boolean?;
      var optionEnableBatch = null as Boolean?;
      var optionResource = null as List<(String,)>?;
      var option = CommandLineReader(this.widget.option);
      if (option.check('-parallel_forward')) {
        optionParallelForward = option.nextBoolean();
      }
      if (option.check('-enable_filter')) {
        optionEnableFilter = option.nextBoolean();
      }
      if (option.check('-enable_batch')) {
        optionEnableBatch = option.nextBoolean();
      }
      if (option.check('-resource')) {
        optionResource = [];
        while (!option.done()) {
          optionResource.add((
            option.nextString(),
          ));
        }
      }
      assertTest(option.done());
      if (optionParallelForward != null) {
        this._parallelForward = optionParallelForward;
      }
      if (optionEnableFilter != null) {
        this._enableFilter = optionEnableFilter;
      }
      if (optionEnableBatch != null) {
        this._enableBatch = optionEnableBatch;
      }
      if (optionResource != null) {
        await this._appendResource(optionResource.map((item) => StorageHelper.regularize(item.$1)).toList());
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
              child: ListView.builder(
                padding: const EdgeInsets.fromLTRB(0, 4, 0, 4),
                controller: this._optionListScrollController,
                itemCount: this._optionConfiguration.length,
                itemBuilder: (context, index) => OptionGroupItem(
                  key: ObjectKey(this._optionConfiguration[index]),
                  configuration: this._optionConfiguration[index],
                  match: this._optionMatch[index],
                  enableFilter: this._enableFilter,
                  enableBatch: this._enableBatch,
                  onSelect: this._forwardResource,
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
          const SizedBox(width: 16),
          const Expanded(child: SizedBox()),
          const SizedBox(width: 16),
          MenuAnchor(
            alignmentOffset: const Offset(0, 8),
            menuChildren: [
              ...this._resource.reversed.map((value) => Tooltip(
                message: value.$1,
                child: MenuItemButton(
                  closeOnActivate: false,
                  onPressed: () async {
                    await this._removeResource([value.$1]);
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
                closeOnActivate: false,
                onPressed: () async {
                  await this._clearResource();
                },
                leadingIcon: const Icon(IconSymbols.delete),
                child: const Text(
                  'Clear All',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
              MenuItemButton(
                closeOnActivate: false,
                onPressed: () async {
                  var item = await StorageHelper.pickLoadFile(context, 'ResourceForwarder.Resource');
                  if (item != null) {
                    await this._appendResource([item]);
                  }
                },
                leadingIcon: const Icon(IconSymbols.draft),
                child: const Text(
                  'Add File',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
              MenuItemButton(
                closeOnActivate: false,
                onPressed: () async {
                  var item = await StorageHelper.pickLoadDirectory(context, 'ResourceForwarder.Resource');
                  if (item != null) {
                    await this._appendResource([item]);
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
              count: this._resource.length,
              child: FloatingActionButton(
                tooltip: 'Resource',
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
