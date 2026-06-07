import '/common.dart';
import '/module.dart';
import '/widget/export.dart';
import '/view/kairosoft_game_manager/setting.dart';
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
        SettingListItemExtension.buildForStoragePathListVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.format_list_bulleted,
          label: 'Repository',
          comment: (negative: 'Empty', positive: 'Defined'),
          getValue: () => this.data.repository,
          setValue: (value) => this.data.repository = value,
          pickerTag: '${ModuleHelper.query(.kairosoftGameManager).identifier}.repository',
          pickerType: [.loadDirectory],
          onUpdate: this.onUpdate,
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
