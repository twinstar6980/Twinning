import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/modding_worker/setting.dart';
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
          icon: IconSet.adjust,
          label: 'Kernel',
          comment: [
            StyledText.inherit(!StorageHelper.existFileSync(this.data.kernel) ? 'Invalid' : 'Available'),
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
                    icon: IconView.of(IconSet.open_in_new),
                    onPressed: (context) async {
                      var target = await pickStorageItem(
                        context: context,
                        allowLoadFile: true,
                        location: '@${ModuleType.modding_worker.name}.kernel',
                      );
                      if (target != null) {
                        this.data.kernel = target;
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: this.data.kernel,
                onChanged: (context, value) async {
                  this.data.kernel = StorageHelper.regularize(value);
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
        SettingListItem(
          icon: IconSet.data_object,
          label: 'Script',
          comment: [
            StyledText.inherit(!StorageHelper.existFileSync(this.data.script) ? 'Invalid' : 'Available'),
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
                    icon: IconView.of(IconSet.open_in_new),
                    onPressed: (context) async {
                      var target = await pickStorageItem(
                        context: context,
                        allowLoadFile: true,
                        location: '@${ModuleType.modding_worker.name}.script',
                      );
                      if (target != null) {
                        this.data.script = target;
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: this.data.script,
                onChanged: (context, value) async {
                  this.data.script = StorageHelper.regularize(value);
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
        SettingListItem(
          icon: IconSet.format_list_bulleted,
          label: 'Argument',
          comment: [
            StyledText.inherit(!this.data.argument.isNotEmpty ? 'Empty' : 'Defined'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              content: StyledInput.outlined(
                type: .multiline,
                format: null,
                hint: null,
                prefix: null,
                suffix: [
                  StyledIconButton.standard(
                    tooltip: 'Pick',
                    icon: IconView.of(IconSet.open_in_new),
                    onPressed: (context) async {
                      var target = await pickStorageItem(
                        context: context,
                        allowLoadFile: true,
                        allowLoadDirectory: true,
                        allowSaveFile: true,
                        location: '@${ModuleType.modding_worker.name}.argument',
                      );
                      if (target != null) {
                        this.data.argument = [...this.data.argument, target];
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: ConvertHelper.makeStringListToStringWithLine(this.data.argument),
                onChanged: (context, value) async {
                  this.data.argument = ConvertHelper.parseStringListFromStringWithLine(value);
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
        SettingListItem(
          icon: IconSet.touch_app,
          label: 'Immediate Launch',
          comment: [
            StyledText.inherit(!this.data.immediateLaunch ? 'Disabled' : 'Enabled'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              leading: StyledSwitch.standard(
                value: this.data.immediateLaunch,
                onChanged: (context, value) async {
                  this.data.immediateLaunch = value;
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
          icon: IconSet.text_fields,
          label: 'Message Font',
          comment: [
            StyledText.inherit(!this.data.messageFont.isNotEmpty ? 'Default' : 'Custom'),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              content: StyledInput.outlined(
                type: .multiline,
                format: null,
                hint: null,
                prefix: null,
                suffix: [
                  StyledIconButton.standard(
                    tooltip: 'Pick',
                    icon: IconView.of(IconSet.open_in_new),
                    onPressed: (context) async {
                      var target = await pickStorageItem(
                        context: context,
                        allowLoadFile: true,
                        location: '@${ModuleType.modding_worker.name}.message_font',
                      );
                      if (target != null) {
                        this.data.messageFont = [...this.data.messageFont, target];
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: ConvertHelper.makeStringListToStringWithLine(this.data.messageFont),
                onChanged: (context, value) async {
                  this.data.messageFont = ConvertHelper.parseStringListFromStringWithLine(value).map(StorageHelper.regularize).toList();
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
