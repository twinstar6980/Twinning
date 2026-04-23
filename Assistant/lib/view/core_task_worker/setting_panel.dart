import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/miscellaneous_helper.dart';
import '/widget/export.dart';
import '/view/core_task_worker/setting.dart';
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
                      var target = (await MiscellaneousHelper.pickStorageItem(context, '${ModuleHelper.query(.coreTaskWorker).identifier}.kernel', [.loadFile], false, null, null)).firstOrNull;
                      if (target != null) {
                        this.data.kernel = target;
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: this.data.kernel.emit(),
                onChanged: (context, value) async {
                  this.data.kernel = .of(value);
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
                      var target = (await MiscellaneousHelper.pickStorageItem(context, '${ModuleHelper.query(.coreTaskWorker).identifier}.script', [.loadFile], false, null, null)).firstOrNull;
                      if (target != null) {
                        this.data.script = target;
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: this.data.script.emit(),
                onChanged: (context, value) async {
                  this.data.script = .of(value);
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
            StyledText.inherit(this.data.argument.isEmpty ? 'Empty' : 'Defined'),
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
                      var target = await MiscellaneousHelper.pickStorageItem(context, '${ModuleHelper.query(.coreTaskWorker).identifier}.argument', [.loadFile, .loadDirectory, .saveFile], true, null, null);
                      if (!target.isEmpty) {
                        this.data.argument = [...this.data.argument, ...target.map((it) => it.emit())];
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
            StyledText.inherit(this.data.messageFont.isEmpty ? 'Default' : 'Custom'),
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
                      var target = await MiscellaneousHelper.pickStorageItem(context, '${ModuleHelper.query(.coreTaskWorker).identifier}.message_font', [.loadFile], true, null, null);
                      if (!target.isEmpty) {
                        this.data.messageFont = [...this.data.messageFont, ...target];
                        await refreshState(setStateForPanel);
                        await refreshState(setState);
                        this.onUpdate();
                      }
                    },
                  ),
                ],
                value: ConvertHelper.makeStringListToStringWithLine(this.data.messageFont.map((it) => it.emit()).toList()),
                onChanged: (context, value) async {
                  this.data.messageFont = ConvertHelper.parseStringListFromStringWithLine(value).map(StoragePath.of).toList();
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
