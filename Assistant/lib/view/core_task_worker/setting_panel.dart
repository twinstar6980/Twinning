import '/common.dart';
import '/module.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/core_task_worker/setting.dart';
import 'package:flutter/widgets.dart';

// ----------------

class SettingPanel extends StatelessWidget {

  const SettingPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  // ----------------

  final Setting         data;
  final Void Function() onUpdate;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => FlexContainer.vertical([
        Gap.vertical(8),
        SettingListItemExtension.buildForStoragePathVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.adjust,
          label: 'Kernel',
          comment: (negative: 'Invalid', positive: 'Available'),
          getValue: () => this.data.kernel,
          setValue: (value) => this.data.kernel = value,
          checkValue: (value) => StorageHelper.existFileSync(value),
          pickerTag: '${ModuleHelper.query(.coreTaskWorker).identifier}.kernel',
          pickerType: [.loadFile],
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForStoragePathVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.data_object,
          label: 'Script',
          comment: (negative: 'Invalid', positive: 'Available'),
          getValue: () => this.data.script,
          setValue: (value) => this.data.script = value,
          checkValue: (value) => StorageHelper.existFileSync(value),
          pickerTag: '${ModuleHelper.query(.coreTaskWorker).identifier}.script',
          pickerType: [.loadFile],
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForStringListVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.format_list_bulleted,
          label: 'Argument',
          comment: (negative: 'Empty', positive: 'Defined'),
          getValue: () => this.data.argument,
          setValue: (value) => this.data.argument = value,
          pickerTag: '${ModuleHelper.query(.coreTaskWorker).identifier}.argument',
          pickerType: [.loadFile, .loadDirectory, .saveFile],
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.touch_app,
          label: 'Immediate Launch',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.immediateLaunch,
          setValue: (value) => this.data.immediateLaunch = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForStoragePathListVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.text_fields,
          label: 'Message Font',
          comment: (negative: 'Default', positive: 'Custom'),
          getValue: () => this.data.messageFont,
          setValue: (value) => this.data.messageFont = value,
          pickerTag: '${ModuleHelper.query(.coreTaskWorker).identifier}.message_font',
          pickerType: [.loadFile],
          onUpdate: this.onUpdate,
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
