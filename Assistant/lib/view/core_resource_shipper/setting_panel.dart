import '/common.dart';
import '/widget/export.dart';
import '/view/core_resource_shipper/setting.dart';
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
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.shuffle,
          label: 'Parallel Forward',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.parallelForward,
          setValue: (value) => this.data.parallelForward = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.filter_alt,
          label: 'Enable Filter',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.enableFilter,
          setValue: (value) => this.data.enableFilter = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.stacks,
          label: 'Enable Batch',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.enableBatch,
          setValue: (value) => this.data.enableBatch = value,
          onUpdate: this.onUpdate,
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
