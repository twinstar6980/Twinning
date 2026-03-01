import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/core_resource_shipper/setting.dart';
import '/view/core_resource_shipper/configuration.dart';
import '/view/core_resource_shipper/option_item.dart';
import '/view/core_task_worker/forward_helper.dart' as core_task_worker;
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

  late Boolean                                                                                                    _parallelForward;
  late Boolean                                                                                                    _enableFilter;
  late Boolean                                                                                                    _enableBatch;
  late List<({String name, Boolean? type})>                                                                       _resource;
  late List<List<({Boolean singleEnabled, Boolean singleFiltered, Boolean batchEnabled, Boolean batchFiltered})>> _optionMatch;
  late List<Boolean>                                                                                              _optionExpanded;
  late ScrollController                                                                                           _optionListScrollController;

  Future<Void> _refreshMatch(
  ) async {
    this._optionMatch.clear();
    for (var group in this.widget.configuration.option) {
      var groupMatch = <({Boolean singleEnabled, Boolean singleFiltered, Boolean batchEnabled, Boolean batchFiltered})>[];
      for (var item in group.item) {
        var singleEnabled = true;
        var batchEnabled = item.batch;
        if (item.filter != null) {
          for (var resource in this._resource) {
            singleEnabled &= switch (item.filter!.type) {
              .any       => resource.type != null,
              .file      => resource.type == false,
              .directory => resource.type == true,
            };
            batchEnabled &= resource.type == true;
          }
        }
        var singleFiltered = false;
        var batchFiltered = false;
        if (item.filter == null && this._resource.length == 0) {
          singleFiltered |= true;
          batchFiltered |= true;
        }
        if (item.filter != null && this._resource.length != 0) {
          var nameRule = RegExp(item.filter!.name);
          singleFiltered |= this._resource.every((resource) => nameRule.hasMatch(resource.name));
          batchFiltered |= true;
        }
        groupMatch.add((
          singleEnabled: singleEnabled,
          singleFiltered: singleFiltered,
          batchEnabled: batchEnabled,
          batchFiltered: batchFiltered,
        ));
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
      if (this._resource.any((value) => value.name == item)) {
        continue;
      }
      var itemType = null as Boolean?;
      if (await StorageHelper.existFile(item)) {
        itemType = false;
      }
      if (await StorageHelper.existDirectory(item)) {
        itemType = true;
      }
      this._resource.add((name: item, type: itemType));
    }
    await this._refreshMatch();
    return;
  }

  Future<Void> _removeResource(
    List<String> list,
  ) async {
    for (var item in list) {
      this._resource.removeWhere((value) => value.name == item);
    }
    await this._refreshMatch();
    return;
  }

  Future<Void> _forwardResource(
    String?              method,
    Map<String, Object>? argument,
  ) async {
    var actualInput = this._resource.isNotEmpty ? this._resource.map((value) => value.name).toList() : <String?>[null];
    var actualMethod = method == null ? null : core_task_worker.ForwardHelper.makeMethodMaybeBatch(method, this._enableBatch);
    var actualCommand = actualInput.map((value) => core_task_worker.ForwardHelper.makeArgumentForCommand(value, actualMethod, argument)).toList();
    await core_task_worker.ForwardHelper.forwardMany(this.context, actualCommand, this._parallelForward);
    return;
  }

  // ----------------

  @override
  modulePageOpenView() async {
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
    var optionResource = null as List<({String name})>?;
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
          name: option.nextString(),
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
      await this._appendResource(optionResource.map((item) => StorageHelper.regularize(item.name)).toList());
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
        option.nextString(item.name);
      }
    }
    return option.done();
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._parallelForward = this.widget.setting.parallelForward;
    this._enableFilter = this.widget.setting.enableFilter;
    this._enableBatch = this.widget.setting.enableBatch;
    this._resource = [];
    this._optionMatch = this.widget.configuration.option.map((value) => value.item.map((valueItem) => (singleEnabled: false, singleFiltered: false, batchEnabled: false, batchFiltered: false)).toList()).toList();
    this._optionExpanded = this.widget.configuration.option.map((value) => true).toList();
    this._optionListScrollController = .new();
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
    this._optionListScrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return ModulePageRegion(
      onStorageDrop: (item) async {
        await this._appendResource(item);
        return;
      },
      content: FlexContainer.vertical([
        ListContainer.of(
          padding: .fromLTRB(0, 8, 0, 8),
          controller: this._optionListScrollController,
          itemCount: this.widget.configuration.option.length,
          itemBuilder: (context, index) => OptionGroupItem(
            configuration: this.widget.configuration.option[index],
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
        ).withStyledScrollBar(
          controller: this._optionListScrollController,
        ).withFlexExpanded(),
      ]),
      bottom: StyledBottomBar.standard(
        primary: StyledBadge.standard(
          label: StyledText.inherit('${this._resource.length}'),
          child: StyledFloatingButton.standard(
            tooltip: 'Resource',
            icon: IconView.of(IconSet.attach_file),
            onPressed: (context) async {
              await StyledBottomSheetExtension.show<Void>(context, StyledModalBottomSheet.standard(
                title: 'Resource',
                contentBuilder: (context, setStateForPanel) => [
                  Gap.vertical(12),
                  FlexContainer.horizontal([
                    Gap.horizontal(16),
                    StyledButton.filled(
                      icon: IconView.of(IconSet.tab_close),
                      content: StyledText.inherit('Remove All'),
                      onPressed: (context) async {
                        if (await MoreModalDialogExtension.showForConfirm(context)) {
                          await this._removeResource(this._resource.map((value) => value.name).toList());
                          await refreshState(setStateForPanel);
                        }
                      },
                    ).withFlexExpanded(),
                    Gap.horizontal(12),
                    StyledButton.filled(
                      icon: IconView.of(IconSet.note_stack_add),
                      content: StyledText.inherit('Append New'),
                      onPressed: (context) async {
                        var item = <String>[];
                        var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
                          title: 'Append New',
                          contentBuilder: (context, setStateForPanelInner) => [
                            StyledInput.outlined(
                              type: .multiline,
                              format: null,
                              hint: null,
                              prefix: null,
                              suffix: null,
                              value: ConvertHelper.makeStringListToStringWithLine(item),
                              onChanged: (context, value) async {
                                item = ConvertHelper.parseStringListFromStringWithLine(value).map(StorageHelper.regularize).toList();
                                await refreshState(setStateForPanelInner);
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
                          await this._appendResource(item);
                          await refreshState(setStateForPanel);
                        }
                      },
                    ).withFlexExpanded(),
                    Gap.horizontal(16),
                  ]),
                  Gap.vertical(12),
                  FlexContainer.horizontal([
                    Gap.horizontal(16),
                    StyledButton.filledTonal(
                      icon: IconView.of(IconSet.note_add),
                      content: StyledText.inherit('Pick File'),
                      onPressed: (context) async {
                        var target = await StorageHelper.pickLoadFile(context, '@${ModuleHelper.query(.coreResourceShipper).identifier}.resource');
                        if (target != null) {
                          await this._appendResource([target]);
                          await refreshState(setStateForPanel);
                        }
                      },
                    ).withFlexExpanded(),
                    Gap.horizontal(12),
                    StyledButton.filledTonal(
                      icon: IconView.of(IconSet.create_new_folder),
                      content: StyledText.inherit('Pick Directory'),
                      onPressed: (context) async {
                        var target = await StorageHelper.pickLoadDirectory(context, '@${ModuleHelper.query(.coreResourceShipper).identifier}.resource');
                        if (target != null) {
                          await this._appendResource([target]);
                          await refreshState(setStateForPanel);
                        }
                      },
                    ).withFlexExpanded(),
                    Gap.horizontal(16),
                  ]),
                  Gap.vertical(12),
                  StyledDivider.minimal(indent: 16),
                  Gap.vertical(8),
                  ...this._resource.map((value) => StyledListTile.standardCustom(
                    key: ObjectKey(value),
                    padding: .fromLTRB(24, 0, 24, 0),
                    leading: IconView.of(switch (value.type) {
                      null  => IconSet.hide_source,
                      false => IconSet.draft,
                      true  => IconSet.folder,
                    }),
                    content: StyledText.custom(
                      StorageHelper.name(value.name),
                      tooltip: true,
                      tooltipText: value.name,
                    ),
                    onPressed: (context) async {
                      await this._removeResource([value.name]);
                      await refreshState(setStateForPanel);
                    },
                  )),
                  Gap.vertical(8),
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
            tooltip: 'Enable Filter',
            selected: this._enableFilter,
            icon: IconView.of(IconSet.filter_alt),
            iconOnSelected: IconView.of(IconSet.filter_alt, fill: 1),
            onPressed: (context) async {
              this._enableFilter = !this._enableFilter;
              await refreshState(this.setState);
            },
          ),
          Gap.horizontal(8),
          StyledIconButton.filledTonal(
            tooltip: 'Enable Batch',
            selected: this._enableBatch,
            icon: IconView.of(IconSet.stacks),
            iconOnSelected: IconView.of(IconSet.stacks, fill: 1),
            onPressed: (context) async {
              this._enableBatch = !this._enableBatch;
              await refreshState(this.setState);
            },
          ),
        ],
      ),
    );
  }

}
