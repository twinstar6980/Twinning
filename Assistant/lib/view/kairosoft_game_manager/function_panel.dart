import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/kairosoft_game_helper.dart';
import '/widget/export.dart';
import 'dart:typed_data';
import 'package:flutter/widgets.dart';

// ----------------

class FunctionPanel extends StatelessWidget {

  const FunctionPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  // ----------------

  final Map<String, Object> data;
  final Void Function()     onUpdate;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => StyledCard.elevated(
        margin: .symmetric(horizontal: 16, vertical: 12),
        content: FlexContainer.vertical([
          Gap.vertical(8),
          FlexContainer.horizontal([
            Gap.horizontal(16),
            StyledButton.filledTonal(
              icon: IconView.of(switch (this.data['type']!.as<GameFunctionType>()) {
                .modifyProgram => IconSet.build,
                .encryptRecord => IconSet.lock_open,
                .importRecord => IconSet.archive,
                .exportRecord => IconSet.unarchive,
              }),
              content: FlexContainer.horizontal([
                StyledText.inherit(switch (this.data['type']!.as<GameFunctionType>()) {
                  .modifyProgram => 'Modify Program',
                  .encryptRecord => 'Encrypt Record',
                  .importRecord => 'Import Record',
                  .exportRecord => 'Export Record',
                }).withFlexExpanded(),
              ]),
              onPressed: (context) async {
                var type = await StyledModalDialogExtension.show<GameFunctionType>(context, StyledModalDialog.standard(
                  title: 'Function',
                  contentBuilder: (context, setStateForPanel) => GameFunctionType.values.map((value) => StyledListTile.standard(
                    content: StyledText.inherit(switch (value) {
                      .modifyProgram => 'Modify Program',
                      .encryptRecord => 'Encrypt Record',
                      .importRecord => 'Import Record',
                      .exportRecord => 'Export Record',
                    }),
                    onPressed: (context) async {
                      Navigator.pop(context, value);
                    },
                  )),
                  actionBuilder: null,
                ));
                if (type != null) {
                  this.data['type'] = type;
                  await refreshState(setState);
                }
              },
            ).withFlexExpanded(),
            Gap.horizontal(8),
            StyledButton.filled(
              icon: IconView.of(IconSet.motion_play),
              content: StyledText.inherit('Run'),
              onPressed: (context) async {
                switch (this.data['type']!.as<GameFunctionType>()) {
                  case .modifyProgram: {
                    GameProgramHelper.modifyBundle(
                      this.data['program.target']!.as(),
                      this.data['program.disable_record_encryption']!.as(),
                      this.data['program.enable_debug_mode']!.as(),
                      null,
                      (s) => print(s),
                    );
                    break;
                  }
                  case .encryptRecord: {
                    GameRecordHelper.encrypt(
                      this.data['record.target_directory']!.as(),
                      this.data['record.key']!.as(),
                    );
                    break;
                  }
                  case .importRecord: {
                    GameRecordHelper.import(
                      this.data['record.target_directory']!.as(),
                      this.data['record.archive_file']!.as(),
                      this.data['record.key']!.as(),
                    );
                    break;
                  }
                  case .exportRecord: {
                    GameRecordHelper.export(
                      this.data['record.target_directory']!.as(),
                      this.data['record.archive_file']!.as(),
                      this.data['record.key']!.as(),
                    );
                    break;
                  }
                }
              },
            ),
            Gap.horizontal(16),
          ]),
          Gap.vertical(8),
          StyledDivider.minimal(),
          Gap.vertical(8),
          FlexContainer.horizontal([
            Gap.horizontal(16),
            FlexContainer.vertical([
              if (this.data['type']?.as<GameFunctionType>() == .modifyProgram) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Target',
                  prefix: IconView.of(IconSet.folder),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'Pick',
                      icon: IconView.of(IconSet.open_in_new),
                      onPressed: (context) async {
                        var target = await pickStorageItem(
                          context: context,
                          allowLoadFile: true,
                          allowLoadDirectory: true,
                          location: '@${ModuleType.resource_shipper.name}.function.program.target',
                        );
                        if (target != null) {
                          this.data['program.target'] = target;
                          await refreshState(setState);
                        }
                      },
                    ),
                  ],
                  value: this.data['program.target']?.as() ?? '',
                  onChanged: (context, value) async {
                    this.data['program.target'] = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ).withStorageDropRegion(
                  onDrop: (item) async {
                    this.data['program.target'] = item.first;
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data['type']?.as<GameFunctionType>() == .modifyProgram) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Boolean',
                  prefix: IconView.of(IconSet.lock_open),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'No',
                      selected: this.data['program.disable_record_encryption']! == false,
                      icon: IconView.of(IconSet.do_not_disturb_on),
                      iconOnSelected: IconView.of(IconSet.do_not_disturb_on, fill: 1),
                      onPressed: (context) async {
                        this.data['program.disable_record_encryption'] = false;
                        await refreshState(setState);
                      },
                    ),
                    Gap.horizontal(4),
                    StyledIconButton.standard(
                      tooltip: 'Yes',
                      selected: this.data['program.disable_record_encryption']! == true,
                      icon: IconView.of(IconSet.check_circle),
                      iconOnSelected: IconView.of(IconSet.check_circle, fill: 1),
                      onPressed: (context) async {
                        this.data['program.disable_record_encryption'] = true;
                        await refreshState(setState);
                      },
                    ),
                  ],
                  value: ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.data['program.disable_record_encryption']!.as()),
                  onChanged: (context, value) async {
                    if (value == 'n' || value == 'y') {
                      this.data['program.disable_record_encryption'] = value == 'y';
                    }
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data['type']?.as<GameFunctionType>() == .modifyProgram) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Boolean',
                  prefix: IconView.of(IconSet.bug_report),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'No',
                      selected: this.data['program.enable_debug_mode']! == false,
                      icon: IconView.of(IconSet.do_not_disturb_on),
                      iconOnSelected: IconView.of(IconSet.do_not_disturb_on, fill: 1),
                      onPressed: (context) async {
                        this.data['program.enable_debug_mode'] = false;
                        await refreshState(setState);
                      },
                    ),
                    Gap.horizontal(4),
                    StyledIconButton.standard(
                      tooltip: 'Yes',
                      selected: this.data['program.enable_debug_mode']! == true,
                      icon: IconView.of(IconSet.check_circle),
                      iconOnSelected: IconView.of(IconSet.check_circle, fill: 1),
                      onPressed: (context) async {
                        this.data['program.enable_debug_mode'] = true;
                        await refreshState(setState);
                      },
                    ),
                  ],
                  value: ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.data['program.enable_debug_mode']!.as()),
                  onChanged: (context, value) async {
                    if (value == 'n' || value == 'y') {
                      this.data['program.enable_debug_mode'] = value == 'y';
                    }
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data['type']?.as<GameFunctionType>() == .exportRecord) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Target Directory',
                  prefix: IconView.of(IconSet.folder),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'Pick',
                      icon: IconView.of(IconSet.open_in_new),
                      onPressed: (context) async {
                        var target = await pickStorageItem(
                          context: context,
                          allowLoadFile: true,
                          allowLoadDirectory: true,
                          location: '@${ModuleType.resource_shipper.name}.function.record.target_directory',
                        );
                        if (target != null) {
                          this.data['record.target_directory'] = target;
                          await refreshState(setState);
                        }
                      },
                    ),
                  ],
                  value: this.data['record.target_directory']?.as() ?? '',
                  onChanged: (context, value) async {
                    this.data['record.target_directory'] = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ).withStorageDropRegion(
                  onDrop: (item) async {
                    this.data['record.target_directory'] = item.first;
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data['type']?.as<GameFunctionType>() == .exportRecord) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Archive File',
                  prefix: IconView.of(IconSet.folder_zip),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'Pick',
                      icon: IconView.of(IconSet.open_in_new),
                      onPressed: (context) async {
                        var target = await pickStorageItem(
                          context: context,
                          allowLoadFile: true,
                          allowLoadDirectory: true,
                          location: '@${ModuleType.resource_shipper.name}.function.record.archive_file',
                        );
                        if (target != null) {
                          this.data['record.archive_file'] = target;
                          await refreshState(setState);
                        }
                      },
                    ),
                  ],
                  value: this.data['record.archive_file']?.as() ?? '',
                  onChanged: (context, value) async {
                    this.data['record.archive_file'] = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ).withStorageDropRegion(
                  onDrop: (item) async {
                    this.data['record.archive_file'] = item.first;
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data['type']?.as<GameFunctionType>() == .exportRecord) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Key',
                  prefix: IconView.of(IconSet.key),
                  suffix: null,
                  value: this.data['record.key']?.as<Uint8List>().selfLet((it) => it.map((value) => value.toRadixString(16).padLeft(2, '0')).join(' ')) ?? '',
                  onChanged: (context, value) async {
                    // this.data['record.key'] = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
            ]).withFlexExpanded(),
            Gap.horizontal(16),
          ]).withFlexExpanded(),
          Gap.vertical(8),
          StyledDivider.minimal(),
          Gap.vertical(8),
          FlexContainer.horizontal([
            Gap.horizontal(16),
            Gap.horizontal(16),
          ]).withFlexExpanded(),
          Gap.vertical(8),
        ]),
      ).withFlexExpanded(),
    );
  }

}
