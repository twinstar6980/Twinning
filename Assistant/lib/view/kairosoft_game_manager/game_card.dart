import '/common.dart';
import '/widget/export.dart';
import '/view/kairosoft_game_manager/game_helper.dart';
import '/view/kairosoft_game_manager/game_record_panel.dart';
import 'package:flutter/widgets.dart';

// ----------------

class GameCard extends StatefulWidget {

  const GameCard({
    super.key,
    required this.information,
  });

  // ----------------

  final GameInformation information;

  // ----------------

  @override
  createState() => _GameCardState();

}

class _GameCardState extends State<GameCard> {

  late Boolean _programModifyRunning;

  // ----------------

  @override
  initState() {
    super.initState();
    this._programModifyRunning = false;
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

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
            child: this.widget.information.packageIcon == null
              ? IconView.of(IconSet.broken_image, size: 48)
              : ImageView.of(
                fit: .cover,
                source: this.widget.information.packageIcon!,
              ),
          ),
          Gap.horizontal(12),
          FlexContainer.vertical([
            FlexContainer.horizontal([
              StyledText.custom(
                this.widget.information.packageName,
                typography: .labelLarge,
                color: .primary,
              ).withFlexExpanded(),
            ]),
            Gap.vertical(4),
            FlexContainer.horizontal([
              StyledText.custom(
                '${this.widget.information.packageIdentifier} - ${this.widget.information.packageVersion}',
                typography: .labelSmall,
                color: .onSurfaceVariant,
              ).withFlexExpanded(),
            ]),
            Gap.vertical(8),
            StyledDivider.minimal(),
            Gap.vertical(8),
            FlexContainer.horizontal([
              Gap.expanded(),
              StyledBadge.standard(
                visible: this.widget.information.recordState == .decrypted || this.widget.information.recordState == .invalid,
                label: StyledText.inherit(switch (this.widget.information.recordState) {
                  .decrypted => '!',
                  .invalid   => '?',
                  _          => '',
                }),
                child: StyledButton.filledTonal(
                  tooltip: 'Record',
                  content: IconView.of(IconSet.view_timeline),
                  onPressed: (context) async {
                    await StyledModalBottomSheetExtension.show<String>(context, StyledModalBottomSheet.standard(
                      title: 'Record',
                      contentBuilder: (context, setStateForPanel) => [
                        GameRecordPanel(
                          information: this.widget.information,
                          onUpdate: () async {
                            await refreshState(setState);
                          },
                        ),
                      ],
                    ));
                  },
                ),
              ),
              Gap.horizontal(8),
              StyledBadge.standard(
                visible: this.widget.information.programState == .modified && !this._programModifyRunning,
                label: StyledText.inherit(switch (this.widget.information.programState) {
                  .modified => '!',
                  _         => '',
                }),
                child: StyledButton.filledTonal(
                  enabled: !this._programModifyRunning,
                  tooltip: 'Program',
                  content: !this._programModifyRunning
                    ? IconView.of(IconSet.deployed_code)
                    : BoxContainer.of(
                      constraints: .tightFor(width: 18, height: 18),
                      child: StyledProgress.circular(),
                    ),
                  onPressed: (context) async {
                    var action = await StyledModalDialogExtension.show<String>(context, StyledModalDialog.standard(
                      title: 'Program',
                      contentBuilder: (context, setState) => [
                        ...<({String value, String text, IconData icon, Boolean enabled})>[
                          (value: 'restore', text: 'Restore', icon: IconSet.settings_backup_restore, enabled: this.widget.information.programState == .modified),
                          (value: 'modify', text: 'Modify', icon: IconSet.handyman, enabled: this.widget.information.programState == .original || this.widget.information.programState == .modified),
                        ].map((it) => StyledListTile.standard(
                          enabled: it.enabled,
                          leading: IconView.of(it.icon),
                          content: StyledText.inherit(it.text),
                          onPressed: (context) => Navigator.pop(context, it.value),
                        )),
                      ],
                      actionBuilder: null,
                    ));
                    if (action != null) {
                      switch (action) {
                        case 'restore': {
                          await GameProgramHelper.restoreBackup(this.widget.information);
                          await refreshState(setState);
                          await StyledSnackExtension.show(this.context, 'done!');
                          break;
                        }
                        case 'modify': {
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
                            this._programModifyRunning = true;
                            await refreshState(setState);
                            try {
                              await GameProgramHelper.modify(this.widget.information, disableRecordEncryption, enableDebugMode, this.context);
                            }
                            finally {
                              this._programModifyRunning = false;
                              await refreshState(setState);
                              await StyledSnackExtension.show(this.context, 'done!');
                            }
                          }
                          break;
                        }
                        default: throw UnreachableException();
                      }
                    }
                  },
                ),
              ),
              Gap.horizontal(8),
              StyledButton.filled(
                icon: IconView.of(IconSet.joystick),
                content: StyledText.inherit('Action'),
                onPressed: (context) async {
                  var action = await StyledModalDialogExtension.show<String>(context, StyledModalDialog.standard(
                    title: 'Action',
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
                        await GameActionHelper.reload(this.widget.information);
                        await refreshState(setState);
                        await StyledSnackExtension.show(this.context, 'done!');
                        break;
                      }
                      case 'reveal': {
                        await GameActionHelper.reveal(this.widget.information);
                        break;
                      }
                      case 'launch': {
                        await GameActionHelper.launch(this.widget.information);
                        await StyledSnackExtension.show(this.context, 'done!');
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
