import '/common.dart';
import '/widget/export.dart';
import '/view/kairosoft_game_manager/game_helper.dart';
import '/view/kairosoft_game_manager/game_program_panel.dart';
import '/view/kairosoft_game_manager/game_record_panel.dart';
import 'package:flutter/widgets.dart';

// ----------------
class GameCard extends StatelessWidget {

  const GameCard({
    super.key,
    required this.information,
  });

  // ----------------

  final GameInformation information;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => StyledCard.elevated(
        margin: .symmetric(horizontal: 16, vertical: 6),
        padding: .symmetric(horizontal: 16, vertical: 12),
        content: FlexContainer.horizontal(crossAlign: .start, [
          BoxContainer.of(
          constraints: .tightFor(width: 48, height: 48),
            borderRadius: .circular(8),
            child: ImageView.of(
              fit: .cover,
              source: this.information.packageIcon!,
            ),
          ),
          Gap.horizontal(12),
          FlexContainer.vertical([
            FlexContainer.horizontal([
              StyledText.custom(
                this.information.packageName,
                typography: .labelLarge,
                color: .primary,
              ).withFlexExpanded(),
            ]),
            Gap.vertical(4),
            FlexContainer.horizontal([
              StyledText.custom(
                '${this.information.packageIdentifier ?? ''} - ${this.information.packageVersion ?? ''}',
                typography: .labelSmall,
                color: .onSurfaceVariant,
              ).withFlexExpanded(),
            ]),
            Gap.vertical(8),
            StyledDivider.minimal(),
            Gap.vertical(8),
            FlexContainer.horizontal([
              Gap.expanded(),
              StyledIconButton.filledTonal(
                tooltip: 'Record',
                selected: this.information.recordState == .decrypted,
                icon: FlexContainer.horizontal([
                  Gap.horizontal(8),
                  StyledText.inherit('${this.information.recordBackup.length}'),
                  Gap.horizontal(4),
                  IconView.of(IconSet.view_timeline),
                ]),
                onPressed: (context) async {
                  await StyledModalBottomSheetExtension.show<String>(context, StyledModalBottomSheet.standard(
                    title: 'Record',
                    contentBuilder: (context, setStateForPanel) => [
                      GameRecordPanel(
                        information: this.information,
                        onUpdate: () async {
                          await refreshState(setState);
                        },
                      ),
                    ],
                  ));
                },
              ),
              Gap.horizontal(8),
              StyledIconButton.filledTonal(
                tooltip: 'Program',
                selected: this.information.programState == .modified,
                icon: FlexContainer.horizontal([
                  Gap.horizontal(8),
                  StyledText.inherit('${this.information.programBackup.length}'),
                  Gap.horizontal(4),
                  IconView.of(IconSet.deployed_code),
                ]),
                onPressed: (context) async {
                  await StyledModalBottomSheetExtension.show<String>(context, StyledModalBottomSheet.standard(
                    title: 'Program',
                    contentBuilder: (context, setStateForPanel) => [
                      GameProgramPanel(
                        information: this.information,
                        onUpdate: () async {
                          await refreshState(setState);
                        },
                      ),
                    ],
                  ));
                },
              ),
              Gap.horizontal(8),
              StyledButton.filled(
                icon: IconView.of(IconSet.joystick),
                content: StyledText.inherit('Game'),
                onPressed: (context) async {
                  var action = await StyledModalDialogExtension.show<String>(context, StyledModalDialog.standard(
                    title: 'Game Action',
                    contentBuilder: (context, setState) => [
                      ...<({String value, String text, IconData icon})>[
                        (value: 'reload', text: 'Reload', icon: IconSet.refresh),
                        (value: 'reveal', text: 'Reveal', icon: IconSet.folder_open),
                        (value: 'launch', text: 'Launch', icon: IconSet.play_circle),
                      ].map((it) => StyledListTile.standard(
                        leading: IconView.of(it.icon),
                        content: StyledText.inherit(it.text),
                        onPressed: (context) => Navigator.pop(context, it.value),
                      )),
                    ],
                    actionBuilder: null,
                  ));
                  if (action != null) {
                    switch (action) {
                      case 'reload': {
                        await GameActionHelper.reload(information);
                        await refreshState(setState);
                        break;
                      }
                      case 'reveal': {
                        await GameActionHelper.reveal(information);
                        break;
                      }
                      case 'launch': {
                        await GameActionHelper.launch(information);
                        break;
                      }
                      default: throw UnreachableException();
                    }
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
