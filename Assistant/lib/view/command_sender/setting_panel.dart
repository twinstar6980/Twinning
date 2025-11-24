import '/common.dart';
import '/widget/export.dart';
import '/view/command_sender/setting.dart';
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
        SettingListItem(
          icon: IconSet.shuffle,
          label: 'Parallel Forward',
          comment: [
            StyledText.inherit(!this.data.parallelForward ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.parallelForward,
                onChanged: (context, value) async {
                  this.data.parallelForward = value;
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
              content: StyledText.inherit('Enable'),
            ),
          ],
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
