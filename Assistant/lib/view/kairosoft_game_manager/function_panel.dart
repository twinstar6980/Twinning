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

  final FunctionPanelData data;
  final Void Function()   onUpdate;

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
              icon: IconView.of(switch (this.data.type) {
                .modifyProgram => IconSet.build,
                .encryptRecord => IconSet.lock_open,
                .importRecord => IconSet.archive,
                .exportRecord => IconSet.unarchive,
              }),
              content: FlexContainer.horizontal([
                StyledText.inherit(switch (this.data.type) {
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
                  this.data.type = type;
                  await refreshState(setState);
                }
              },
            ).withFlexExpanded(),
            Gap.horizontal(8),
            StyledButton.filled(
              icon: IconView.of(IconSet.motion_play),
              content: StyledText.inherit('Run'),
              onPressed: (context) async {
                switch (this.data.type) {
                  case .modifyProgram: {
                    GameProgramHelper.modifyBundle(
                      this.data.programTarget,
                      this.data.programDisableRecordEncryption,
                      this.data.programEnableDebugMode,
                      null,
                      (s) => print(s),
                    );
                    break;
                  }
                  case .encryptRecord: {
                    GameRecordHelper.encrypt(
                      this.data.recordTargetDirectory,
                      this.data.recordKey,
                    );
                    break;
                  }
                  case .importRecord: {
                    GameRecordHelper.import(
                      this.data.recordTargetDirectory,
                      this.data.recordArchiveFile,
                      this.data.recordKey,
                    );
                    break;
                  }
                  case .exportRecord: {
                    GameRecordHelper.export(
                      this.data.recordTargetDirectory,
                      this.data.recordArchiveFile,
                      this.data.recordKey,
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
              if (this.data.type == .modifyProgram) ...[
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
                          location: '@${ModuleType.core_resource_shipper.name}.function.program.target',
                        );
                        if (target != null) {
                          this.data.programTarget = target;
                          await refreshState(setState);
                        }
                      },
                    ),
                  ],
                  value: this.data.programTarget,
                  onChanged: (context, value) async {
                    this.data.programTarget = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ).withStorageDropRegion(
                  onDrop: (item) async {
                    this.data.programTarget = item.first;
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data.type == .modifyProgram) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Boolean',
                  prefix: IconView.of(IconSet.lock_open),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'No',
                      selected: this.data.programDisableRecordEncryption == false,
                      icon: IconView.of(IconSet.do_not_disturb_on),
                      iconOnSelected: IconView.of(IconSet.do_not_disturb_on, fill: 1),
                      onPressed: (context) async {
                        this.data.programDisableRecordEncryption = false;
                        await refreshState(setState);
                      },
                    ),
                    Gap.horizontal(4),
                    StyledIconButton.standard(
                      tooltip: 'Yes',
                      selected: this.data.programDisableRecordEncryption == true,
                      icon: IconView.of(IconSet.check_circle),
                      iconOnSelected: IconView.of(IconSet.check_circle, fill: 1),
                      onPressed: (context) async {
                        this.data.programDisableRecordEncryption = true;
                        await refreshState(setState);
                      },
                    ),
                  ],
                  value: ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.data.programDisableRecordEncryption),
                  onChanged: (context, value) async {
                    if (value == 'n' || value == 'y') {
                      this.data.programDisableRecordEncryption = value == 'y';
                    }
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data.type == .modifyProgram) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Boolean',
                  prefix: IconView.of(IconSet.bug_report),
                  suffix: [
                    StyledIconButton.standard(
                      tooltip: 'No',
                      selected: this.data.programEnableDebugMode == false,
                      icon: IconView.of(IconSet.do_not_disturb_on),
                      iconOnSelected: IconView.of(IconSet.do_not_disturb_on, fill: 1),
                      onPressed: (context) async {
                        this.data.programEnableDebugMode = false;
                        await refreshState(setState);
                      },
                    ),
                    Gap.horizontal(4),
                    StyledIconButton.standard(
                      tooltip: 'Yes',
                      selected: this.data.programEnableDebugMode == true,
                      icon: IconView.of(IconSet.check_circle),
                      iconOnSelected: IconView.of(IconSet.check_circle, fill: 1),
                      onPressed: (context) async {
                        this.data.programEnableDebugMode = true;
                        await refreshState(setState);
                      },
                    ),
                  ],
                  value: ConvertHelper.makeBooleanToStringOfConfirmationCharacter(this.data.programEnableDebugMode),
                  onChanged: (context, value) async {
                    if (value == 'n' || value == 'y') {
                      this.data.programEnableDebugMode = value == 'y';
                    }
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data.type == .exportRecord) ...[
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
                          location: '@${ModuleType.core_resource_shipper.name}.function.record.target_directory',
                        );
                        if (target != null) {
                          this.data.recordTargetDirectory = target;
                          await refreshState(setState);
                        }
                      },
                    ),
                  ],
                  value: this.data.recordTargetDirectory,
                  onChanged: (context, value) async {
                    this.data.recordTargetDirectory = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ).withStorageDropRegion(
                  onDrop: (item) async {
                    this.data.recordTargetDirectory = item.first;
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data.type == .exportRecord) ...[
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
                          location: '@${ModuleType.core_resource_shipper.name}.function.record.archive_file',
                        );
                        if (target != null) {
                          this.data.recordArchiveFile = target;
                          await refreshState(setState);
                        }
                      },
                    ),
                  ],
                  value: this.data.recordArchiveFile,
                  onChanged: (context, value) async {
                    this.data.recordArchiveFile = StorageHelper.regularize(value);
                    await refreshState(setState);
                  },
                ).withStorageDropRegion(
                  onDrop: (item) async {
                    this.data.recordArchiveFile = item.first;
                    await refreshState(setState);
                  },
                ),
                Gap.vertical(8),
              ],
              if (this.data.type == .exportRecord) ...[
                StyledInput.underlined(
                  type: .text,
                  format: null,
                  hint: 'Key',
                  prefix: IconView.of(IconSet.key),
                  suffix: null,
                  value: this.data.recordKey.selfLet((it) => it.map((value) => value.toRadixString(16).padLeft(2, '0')).join(' ')) ?? '',
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
      ),
    );
  }

}

class FunctionPanelData {
  GameFunctionType type;
  String           programTarget;
  Boolean          programDisableRecordEncryption;
  Boolean          programEnableDebugMode;
  String           recordTargetDirectory;
  String           recordArchiveFile;
  Uint8List        recordKey;
  FunctionPanelData({
    required this.type,
    required this.programTarget,
    required this.programDisableRecordEncryption,
    required this.programEnableDebugMode,
    required this.recordTargetDirectory,
    required this.recordArchiveFile,
    required this.recordKey,
  });
}
