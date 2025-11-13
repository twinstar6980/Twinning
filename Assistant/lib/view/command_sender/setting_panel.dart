import '/common.dart';
import '/utility/storage_helper.dart';
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
          icon: IconSet.description,
          label: 'Method Configuration',
          comment: [
            StyledText.inherit(!StorageHelper.existFileSync(this.data.methodConfiguration) ? 'Invalid' : 'Available'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              content: StyledInput.outlined(
                type: .text,
                format: null,
                hint: null,
                prefix: null,
                suffix: [
                  StyledIconButton.standard(
                    tooltip: 'Pick',
                    icon: Icon(IconSet.open_in_new),
                    onPressed: (context) async {
                      var target = await pickStorageItem(
                        context: context,
                        allowLoadFile: true,
                        location: '@CommandSender.MethodConfiguration',
                      );
                      if (target != null) {
                        this.data.methodConfiguration = target;
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: this.data.methodConfiguration,
                onChanged: (context, value) async {
                  this.data.methodConfiguration = StorageHelper.regularize(value);
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
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
