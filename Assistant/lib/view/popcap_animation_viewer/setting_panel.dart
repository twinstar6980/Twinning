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
        SettingListItem(
          icon: IconSet.ads_click,
          label: 'Immediate Select',
          comment: [
            StyledText.inherit(!this.data.immediateSelect ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.immediateSelect,
                onChanged: (context, value) async {
                  this.data.immediateSelect = value;
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
          icon: IconSet.autoplay,
          label: 'Automatic Play',
          comment: [
            StyledText.inherit(!this.data.automaticPlay ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.automaticPlay,
                onChanged: (context, value) async {
                  this.data.automaticPlay = value;
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
          icon: IconSet.repeat,
          label: 'Repeat Play',
          comment: [
            StyledText.inherit(!this.data.repeatPlay ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.repeatPlay,
                onChanged: (context, value) async {
                  this.data.repeatPlay = value;
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
          icon: IconSet.settings_backup_restore,
          label: 'Reverse Play',
          comment: [
            StyledText.inherit(!this.data.reversePlay ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.reversePlay,
                onChanged: (context, value) async {
                  this.data.reversePlay = value;
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
          icon: IconSet.lock_clock,
          label: 'Keep Speed',
          comment: [
            StyledText.inherit(!this.data.keepSpeed ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.keepSpeed,
                onChanged: (context, value) async {
                  this.data.keepSpeed = value;
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
          icon: IconSet.frame_source,
          label: 'Show Boundary',
          comment: [
            StyledText.inherit(!this.data.showBoundary ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.showBoundary,
                onChanged: (context, value) async {
                  this.data.showBoundary = value;
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
