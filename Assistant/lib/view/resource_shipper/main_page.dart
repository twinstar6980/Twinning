import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/control_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/json_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/view/home/common.dart';
import '/view/resource_shipper/configuration.dart';
import '/view/resource_shipper/option_item.dart';
import '/view/modding_worker/forward_helper.dart' as modding_worker;
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

  late List<OptionGroupConfiguration>          _optionConfiguration;
  late Boolean                                 _parallelForward;
  late Boolean                                 _enableFilter;
  late Boolean                                 _enableBatch;
  late List<(String, Boolean?)>                _resource;
  late List<List<(Boolean, Boolean, Boolean)>> _optionMatch;
  late List<Boolean>                           _optionExpanded;
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
    await refreshState(this.setState);
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
  modulePageOpenView() async {
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._optionConfiguration = ConfigurationHelper.parseDataFromJson(await JsonHelper.deserializeFile(setting.data.resourceShipper.optionConfiguration));
    this._optionExpanded = this._optionConfiguration.map((value) => true).toList();
    await this._refreshMatch();
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
    var optionEnableFilter = null as Boolean?;
    var optionEnableBatch = null as Boolean?;
    var optionResource = null as List<(String,)>?;
    var option = CommandLineReader(optionView);
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
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
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
    await refreshState(this.setState);
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = CommandLineWriter();
    if (option.check('-parallel_forward')) {
      option.nextBoolean(this._parallelForward);
    }
    if (option.check('-enable_filter')) {
      option.nextBoolean(this._enableFilter);
    }
    if (option.check('-enable_batch')) {
      option.nextBoolean(this._enableBatch);
    }
    if (option.check('-resource')) {
      for (var item in this._resource) {
        option.nextString(item.$1);
      }
    }
    return option.done();
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._optionConfiguration = [];
    this._parallelForward = setting.data.resourceShipper.parallelForward;
    this._enableFilter = setting.data.resourceShipper.enableFilter;
    this._enableBatch = setting.data.resourceShipper.enableBatch;
    this._resource = [];
    this._optionMatch = [];
    this._optionExpanded = [];
    this._optionListScrollController = ScrollController();
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
    this._optionListScrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return CustomModulePageRegion(
      onDropFile: (item) async {
        await this._appendResource(item);
      },
      content: Column(
        children: [
          ListView.builder(
            padding: EdgeInsets.fromLTRB(0, 8, 0, 8),
            controller: this._optionListScrollController,
            itemCount: this._optionConfiguration.length,
            itemBuilder: (context, index) => OptionGroupItem(
              configuration: this._optionConfiguration[index],
              match: this._optionMatch[index],
              enableFilter: this._enableFilter,
              enableBatch: this._enableBatch,
              onSelect: this._forwardResource,
              expanded: this._optionExpanded[index],
              onToggle: () async {
                this._optionExpanded[index] = !this._optionExpanded[index];
                await refreshState(this.setState);
              },
            ),
          ).withScrollbar(
            controller: this._optionListScrollController,
          ).withExpanded(),
        ],
      ),
      bottom: CustomBottomBarContent(
        primary: Badge.count(
          count: this._resource.length,
          child: FloatingActionButton(
            tooltip: 'Resource',
            elevation: 0,
            focusElevation: 0,
            hoverElevation: 0,
            highlightElevation: 0,
            disabledElevation: 0,
            child: Icon(IconSymbols.attach_file),
            onPressed: () async {
              await ControlHelper.showBottomSheetAsModal<Void>(context, CustomModalBottomSheet(
                title: 'Resource',
                contentBuilder: (context, setStateForPanel) => [
                  SizedBox(height: 12),
                  Row(
                    children: [
                      SizedBox(width: 16),
                      FilledButton.icon(
                        icon: Icon(IconSymbols.tab_close),
                        label: Text(
                          'Remove All',
                          overflow: TextOverflow.ellipsis,
                        ),
                        onPressed: () async {
                          if (await ControlHelper.showDialogForConfirm(context)) {
                            await this._removeResource(this._resource.map((value) => value.$1).toList());
                            await refreshState(setStateForPanel);
                          }
                        },
                      ).withExpanded(),
                      SizedBox(width: 12),
                      FilledButton.icon(
                        icon: Icon(IconSymbols.note_stack_add),
                        label: Text(
                          'Append New',
                          overflow: TextOverflow.ellipsis,
                        ),
                        onPressed: () async {
                          var item = <String>[];
                          var canContinue = await ControlHelper.showDialogAsModal<Boolean>(context, CustomModalDialog(
                            title: 'Append New',
                            contentBuilder: (context, setStateForPanelInner) => [
                              CustomTextField(
                                keyboardType: TextInputType.multiline,
                                inputFormatters: [],
                                decoration: InputDecoration(
                                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                                  filled: false,
                                  border: OutlineInputBorder(),
                                ),
                                value: ConvertHelper.makeStringListToStringWithLine(item),
                                onChanged: (value) async {
                                  item = ConvertHelper.parseStringListFromStringWithLine(value).map(StorageHelper.regularize).toList();
                                  await refreshState(setStateForPanelInner);
                                },
                              ),
                            ],
                            actionBuilder: (context) => [
                              TextButton(
                                child: Text('Cancel'),
                                onPressed: () => Navigator.pop(context, false),
                              ),
                              TextButton(
                                child: Text('Continue'),
                                onPressed: () => Navigator.pop(context, true),
                              ),
                            ],
                          )) ?? false;
                          if (canContinue) {
                            await this._appendResource(item);
                            await refreshState(setStateForPanel);
                          }
                        },
                      ).withExpanded(),
                      SizedBox(width: 16),
                    ],
                  ),
                  SizedBox(height: 12),
                  Row(
                    children: [
                      SizedBox(width: 16),
                      FilledButton.tonalIcon(
                        icon: Icon(IconSymbols.note_add),
                        label: Text(
                          'Pick File',
                          overflow: TextOverflow.ellipsis,
                        ),
                        onPressed: () async {
                          var item = await StorageHelper.pickLoadFile(context, '@ResourceShipper.Resource');
                          if (item != null) {
                            await this._appendResource([item]);
                            await refreshState(setStateForPanel);
                          }
                        },
                      ).withExpanded(),
                      SizedBox(width: 12),
                      FilledButton.tonalIcon(
                        icon: Icon(IconSymbols.create_new_folder),
                        label: Text(
                          'Pick Directory',
                          overflow: TextOverflow.ellipsis,
                        ),
                        onPressed: () async {
                          var item = await StorageHelper.pickLoadDirectory(context, '@ResourceShipper.Resource');
                          if (item != null) {
                            await this._appendResource([item]);
                            await refreshState(setStateForPanel);
                          }
                        },
                      ).withExpanded(),
                      SizedBox(width: 16),
                    ],
                  ),
                  SizedBox(height: 12),
                  Divider(height: 1, indent: 16, endIndent: 16),
                  SizedBox(height: 8),
                  ...this._resource.map((value) => Tooltip(
                    message: value.$1,
                    child: ListTile(
                      key: ObjectKey(value),
                      contentPadding: EdgeInsets.fromLTRB(24, 0, 24, 0),
                      leading: Icon(switch (value.$2) {
                        null  => IconSymbols.hide_source,
                        false => IconSymbols.draft,
                        true  => IconSymbols.folder,
                      }),
                      title: Text(
                        StorageHelper.name(value.$1),
                        overflow: TextOverflow.ellipsis,
                      ),
                      onTap: () async {
                        await this._removeResource([value.$1]);
                        await refreshState(setStateForPanel);
                      },
                    ),
                  )),
                  SizedBox(height: 8),
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
            tooltip: 'Enable Filter',
            isSelected: this._enableFilter,
            icon: Icon(IconSymbols.filter_alt),
            selectedIcon: Icon(IconSymbols.filter_alt, fill: 1),
            onPressed: () async {
              this._enableFilter = !this._enableFilter;
              await refreshState(this.setState);
            },
          ),
          SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Enable Batch',
            isSelected: this._enableBatch,
            icon: Icon(IconSymbols.stacks),
            selectedIcon: Icon(IconSymbols.stacks, fill: 1),
            onPressed: () async {
              this._enableBatch = !this._enableBatch;
              await refreshState(this.setState);
            },
          ),
        ],
      ),
    );
  }

}
