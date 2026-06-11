import '/common.dart'; // ignore: unused_import
import '/widget/export.dart';
import '/utility/convert_helper.dart';
import '/view/kairosoft_game_manager/game_helper.dart';
import 'package:flutter/widgets.dart';

// ----------------

class GameProgramPanel extends StatelessWidget {

  const GameProgramPanel({
    super.key,
    required this.information,
    required this.onUpdate,
  });

  // ----------------

  final GameInformation information;
  final Void Function() onUpdate;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => FlexContainer.vertical([
        Gap.vertical(12),
        FlexContainer.horizontal([
          Gap.horizontal(16),
          StyledButton.filled(
            icon: IconView.of(IconSet.save),
            content: StyledText.inherit('Backup'),
            onPressed: (context) async {
              await GameProgramHelper.createBackup(this.information, ConvertHelper.makeDateTimeToString(DateTime.now()));
              await refreshState(setState);
              this.onUpdate();
            },
          ).withFlexExpanded(),
          Gap.horizontal(12),
          StyledButton.filledTonal(
            icon: IconView.of(IconSet.handyman),
            content: StyledText.inherit('Modify'),
            onPressed: (context) async {
              var disableRecordEncryption = false;
              var enableDebugMode = false;
              var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
                title: 'Modify',
                contentBuilder: (context, setStateForPanel) => [
                  StyledListTile.standard(
                    dense: true,
                    leading: StyledCheck.standard(
                      value: disableRecordEncryption,
                      onChanged: (context, value) async {
                      },
                    ).withImpenetrableArea(
                    ),
                    content: StyledText.inherit(tooltip: true, 'Disable Record Encryption'),
                    trailing: null,
                    onPressed: (context) async {
                      disableRecordEncryption = !disableRecordEncryption;
                      await refreshState(setStateForPanel);
                    },
                  ),
                  StyledListTile.standard(
                    dense: true,
                    leading: StyledCheck.standard(
                      value: enableDebugMode,
                      onChanged: (context, value) async {
                      },
                    ).withImpenetrableArea(
                    ),
                    content: StyledText.inherit(tooltip: true, 'Enable Debug Mode'),
                    trailing: null,
                    onPressed: (context) async {
                      enableDebugMode = !enableDebugMode;
                      await refreshState(setStateForPanel);
                    },
                  ),
                ],
                actionBuilder: (context) => MoreModalDialogExtension.createButtonForContinue(context),
              )) ?? false;
              if (canContinue) {
                await GameProgramHelper.modify(this.information, disableRecordEncryption, enableDebugMode);
                await refreshState(setState);
                this.onUpdate();
              }
            },
          ).withFlexExpanded(),
          Gap.horizontal(16),
        ]),
        Gap.vertical(12),
        StyledDivider.minimal(indent: 16),
        Gap.vertical(8),
        ...this.information.programBackup.reversed.map((it) => StyledListTile.standardCustom(
          key: ObjectKey(it),
          padding: .fromLTRB(24, 0, 24, 0),
          leading: null,
          content: FlexContainer.horizontal([
            StyledText.custom(
              it,
              tooltip: true,
              tooltipText: it,
            ).withFlexExpanded(),
            Gap.horizontal(16),
              StyledIconButton.standard(
              tooltip: 'Remove',
              icon: IconView.of(IconSet.remove),
              onPressed: (context) async {
                if (await MoreModalDialogExtension.showForConfirm(context)) {
                  await GameProgramHelper.deleteBackup(this.information, it);
                  await refreshState(setState);
                  this.onUpdate();
                }
              },
            ),
            Gap.horizontal(4),
            StyledIconButton.standard(
              tooltip: 'Rename',
              icon: IconView.of(IconSet.edit),
              onPressed: (context) async {
                var identifier = await MoreModalDialogExtension.showForRename(context, it, .new(r'^(?!\.)[^<>:"/\\|?*]+(?<!\.)$'));
                if (identifier != null) {
                  await GameProgramHelper.renameBackup(this.information, it, identifier);
                  await refreshState(setState);
                  this.onUpdate();
                }
              },
            ),
          ]),
          onPressed: (context) async {
            if (await MoreModalDialogExtension.showForConfirm(context)) {
              await GameProgramHelper.restoreBackup(this.information, it);
              await refreshState(setState);
              this.onUpdate();
            }
          },
        )),
        Gap.vertical(8),
      ]),
    );
  }

}
