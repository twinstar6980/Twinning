import '/common.dart';
import '/widget/export.dart';
import '/view/core_command_sender/setting.dart';
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
        Gap.vertical(8),
      ]),
    );
  }

}
