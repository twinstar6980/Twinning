import '/common.dart';
import '/widget/export.dart';
import '/view/popcap_animation_viewer/setting.dart';
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
          icon: IconSet.ads_click,
          label: 'Immediate Select',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.immediateSelect,
          setValue: (value) => this.data.immediateSelect = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.autoplay,
          label: 'Automatic Play',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.automaticPlay,
          setValue: (value) => this.data.automaticPlay = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.repeat,
          label: 'Repeat Play',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.repeatPlay,
          setValue: (value) => this.data.repeatPlay = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.settings_backup_restore,
          label: 'Reverse Play',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.reversePlay,
          setValue: (value) => this.data.reversePlay = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.lock_clock,
          label: 'Keep Speed',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.keepSpeed,
          setValue: (value) => this.data.keepSpeed = value,
          onUpdate: this.onUpdate,
        ),
        SettingListItemExtension.buildForBooleanVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.frame_source,
          label: 'Show Boundary',
          comment: (negative: 'Disabled', positive: 'Enabled', action: 'Enable'),
          getValue: () => this.data.showBoundary,
          setValue: (value) => this.data.showBoundary = value,
          onUpdate: this.onUpdate,
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
