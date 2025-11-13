import '/common.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/resource_shipper/setting.dart';
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
          label: 'Option Configuration',
          comment: [
            StyledText.inherit(!StorageHelper.existFileSync(this.data.optionConfiguration) ? 'Invalid' : 'Available'),
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
                        location: '@ResourceShipper.OptionConfiguration',
                      );
                      if (target != null) {
                        this.data.optionConfiguration = target;
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: this.data.optionConfiguration,
                onChanged: (context, value) async {
                  this.data.optionConfiguration = StorageHelper.regularize(value);
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
        SettingListItem(
          icon: IconSet.filter_alt,
          label: 'Enable Filter',
          comment: [
            StyledText.inherit(!this.data.enableFilter ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.enableFilter,
                onChanged: (context, value) async {
                  this.data.enableFilter = value;
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
              content: StyledText.inherit('Enable'),
            ),
          ],
        ),
        SettingListItem(
          icon: IconSet.stacks,
          label: 'Enable Batch',
          comment: [
            StyledText.inherit(!this.data.enableBatch ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.enableBatch,
                onChanged: (context, value) async {
                  this.data.enableBatch = value;
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
