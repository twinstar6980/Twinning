import '/common.dart';
import '/widget/export.dart';
import '/utility/convert_helper.dart';
import '/view/kairosoft_game_manager/game_helper.dart';
import 'package:flutter/widgets.dart';

// ----------------

class GameRecordPanel extends StatelessWidget {

  const GameRecordPanel({
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
              await GameRecordHelper.createBackup(this.information, ConvertHelper.makeDateTimeToString(DateTime.now()));
              await refreshState(setState);
              this.onUpdate();
            },
          ).withFlexExpanded(),
          Gap.horizontal(12),
          StyledButton.filledTonal(
            enabled: this.information.recordState == .original || this.information.recordState == .decrypted,
            icon: IconView.of(this.information.recordState == .decrypted ? IconSet.lock : IconSet.lock_open),
            content: StyledText.inherit(this.information.recordState == .decrypted ? 'Encrypt' : 'Decrypt'),
            onPressed: (context) async {
              if (this.information.recordState == .original) {
                await GameRecordHelper.encrypt(this.information);
              }
              else if (this.information.recordState == .decrypted) {
                await GameRecordHelper.encrypt(this.information);
              }
              await refreshState(setState);
              this.onUpdate();
            },
          ).withFlexExpanded(),
          Gap.horizontal(16),
        ]),
        Gap.vertical(12),
        StyledDivider.minimal(indent: 16),
        Gap.vertical(8),
        ...this.information.recordBackup.reversed.map((it) => StyledListTile.standardCustom(
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
                if (await MoreModalDialogExtension.showForConfirm(context, 'Remove')) {
                  await GameRecordHelper.deleteBackup(this.information, it);
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
                  await GameRecordHelper.renameBackup(this.information, it, identifier);
                  await refreshState(setState);
                  this.onUpdate();
                }
              },
            ),
          ]),
          onPressed: (context) async {
            if (await MoreModalDialogExtension.showForConfirm(context, 'Restore')) {
              await GameRecordHelper.restoreBackup(this.information, it);
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
