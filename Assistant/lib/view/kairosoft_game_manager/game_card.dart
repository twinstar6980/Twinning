import 'package:assistant/utility/process_helper.dart';
import 'package:assistant/utility/storage_helper.dart';
import 'package:flutter/material.dart';
import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/kairosoft_game_helper.dart';
import '/widget/export.dart';
import 'package:flutter/widgets.dart';

// ----------------

enum GameAction {
  gameReload,
  gameReveal,
  gameLaunch,
  programRestore,
  programModify,
  recordRestore,
  recordBackup,
  recordModify,
  recordEncrypt,
  recordDecrypt,
  recordExport,
  recordImport,
}
Future<Void> doAction(
  GameInformation information,
  String          action,
) async {
  switch (action) {
    case 'reload_game': {
      break;
    }
    case 'reveal_game': {
      await StorageHelper.reveal(information.path);
      break;
    }
    case 'launch_game': {
      await ProcessHelper.runProcess(information.path + '/KairoGames.exe', [], null);
      break;
    }
    case 'restore_program': {
      break;
    }
    case 'modify_program': {
      break;
    }
    case 'encrypt_record': {
      break;
    }
    case 'decrypt_record': {
      break;
    }
    case 'export_record': {
      break;
    }
    case 'import_record': {
      break;
    }
    default: throw UnreachableException();
  }
  return;
}

class GameCard extends StatelessWidget {

  const GameCard({
    super.key,
    required this.information,
    required this.onX,
  });

  // ----------------

  final GameInformation information;
  final Void Function() onX;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => StyledListTile.standard(
        leading: BoxContainer.of(
          constraints: .tightFor(width: 48, height: 48),
          borderRadius: .circular(8),
          child: ImageView.of(
            fit: .cover,
            source: this.information.icon!,
          ),
        ),
        content: FlexContainer.vertical([
          Gap.vertical(12),
          FlexContainer.horizontal([
            StyledText.custom(
              this.information.name,
              typography: .titleMedium,
            ).withFlexExpanded(),
          ]),
          Gap.vertical(8),
          FlexContainer.horizontal([
            StyledText.custom(
              '${this.information.identifier ?? ''} (${this.information.version ?? ''})',
              typography: .labelSmall,
              color: .onSurfaceVariant,
            ).withFlexExpanded(),
          ]),
          Gap.vertical(12),
        ]),
        trailing: FlexContainer.vertical(crossAlign: .end, [
          Gap.expanded(),
          FlexContainer.horizontal(mainStretch: false, [
            StyledText.inherit(this.information.program.name),
            Gap.horizontal(8),
            StyledBadge.standard(),
          ]),
          Gap.expanded(),
          FlexContainer.horizontal(mainStretch: false, [
            StyledText.inherit(this.information.record.name),
            Gap.horizontal(8),
            StyledBadge.standard(),
          ]),
          Gap.expanded(),
        ]),
        onPressed: (context) async {
          var action = await StyledModalDialogExtension.show<String>(context, StyledModalDialog.standard(
            title: 'Action',
            contentBuilder: (context, setState) => [
              ...<({String value, String text, IconData icon})?>[
                (value: 'reload_game', text: 'Reload Game', icon: IconSet.refresh),
                (value: 'reveal_game', text: 'Reveal Game', icon: IconSet.folder_open),
                (value: 'launch_game', text: 'Launch Game', icon: IconSet.play_circle),
                null,
                (value: 'restore_program', text: 'Restore Program', icon: IconSet.reset_wrench),
                (value: 'modify_program', text: 'Modify Program', icon: IconSet.handyman),
                null,
                (value: 'encrypt_record', text: 'Encrypt Record', icon: IconSet.lock),
                (value: 'decrypt_record', text: 'Decrypt Record', icon: IconSet.key_vertical),
                null,
                (value: 'export_record', text: 'Export Record', icon: IconSet.save),
                (value: 'import_record', text: 'Import Record', icon: IconSet.outbox),
              ].map((it) => it == null ? StyledDivider.minimal() : StyledListTile.standard(
                leading: IconView.of(it.icon),
                content: StyledText.inherit(it.text),
                onPressed: (context) => Navigator.pop(context, it.value),
              )),
            ],
            actionBuilder: null,
          ));
          if (action != null) {
            await doAction(information, action);
            await refreshState(setState);
          }
        },
      ),
    );
    return StatefulBuilder(
      builder: (context, setState) => StyledCard.elevated(
        margin: .symmetric(horizontal: 16, vertical: 12),
        padding: .symmetric(horizontal: 16, vertical: 12),
        content: FlexContainer.horizontal(crossAlign: .start, [
          IconView.of(IconSet.abc, size: 48),
          Gap.horizontal(12),
          FlexContainer.vertical([
            FlexContainer.horizontal([
              StyledText.custom(
                this.information.name,
                typography: .labelLarge,
                color: .primary,
              ).withFlexExpanded(),
            ]),
            Gap.vertical(4),
            FlexContainer.horizontal([
              StyledText.custom(
                '${this.information.identifier ?? ''} - ${this.information.version ?? ''}',
                typography: .labelSmall,
                color: .onSurfaceVariant,
              ).withFlexExpanded(),
            ]),
            Gap.vertical(8),
            StyledDivider.minimal(),
            Gap.vertical(8),
            FlexContainer.horizontal([
              Gap.expanded(),
              StyledButton.filled(
                icon: IconView.of(IconSet.local_fire_department),
                content: StyledText.inherit('Action'),
                onPressed: (context) async {
                  var action = await StyledMenuExtension.show(context, StyledMenu.standard(
                    position: .under,
                    content: [
                      StyledMenuItem.standard(
                        value: 'launch',
                        leading: IconView.of(IconSet.play_circle),
                        content: StyledText.inherit('Launch Game'),
                      ),
                    ],
                  ));
                  switch (action) {
                    case null: {
                      break;
                    }
                    case '': {
                      break;
                    }
                    default: throw UnreachableException();
                  }
                },
              ),
            ]),
          ]).withFlexExpanded(),
        ]),
      ),
    );
  }

}
