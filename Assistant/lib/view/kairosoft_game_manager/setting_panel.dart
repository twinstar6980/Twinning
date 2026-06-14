import '/common.dart';
import '/module.dart';
import '/utility/storage_helper.dart';
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
        SettingListItemExtension.buildForStoragePathVariable(
          context: context,
          setStateForOuter: setState,
          icon: IconSet.cards_stack,
          label: 'Repository Of Windows Steam',
          comment: (negative: 'Invalid', positive: 'Available'),
          getValue: () => this.data.repositoryOfWindowsSteam,
          setValue: (value) => this.data.repositoryOfWindowsSteam = value,
          checkValue: (value) async => await StorageHelper.existDirectory(value),
          pickerTag: '${ModuleHelper.query(.kairosoftGameManager).identifier}.repository_of_windows_steam',
          pickerType: [.loadDirectory],
          onUpdate: this.onUpdate,
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
