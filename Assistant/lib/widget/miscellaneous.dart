import '/common.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/convert_helper.dart';
import '/utility/miscellaneous_helper.dart';
import '/utility/platform_integration_manager.dart';
import '/widget/common.dart';
import '/widget/container.dart';
import '/widget/control.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart' as material;
import 'package:super_drag_and_drop/super_drag_and_drop.dart' as lib;

// ----------------

// #region setting

class SettingListLabel extends StatelessWidget {

  const SettingListLabel({
    super.key,
    required this.label,
    required this.action,
  });

  // ----------------

  final String  label;
  final Widget? action;

  // ----------------

  @override
  build(context) {
    return StyledListTile.standardCustom(
      dense: true,
      padding: .fromLTRB(24, 0, 24, 0),
      content: StyledText.custom(
        this.label,
        typography: .titleSmall,
        color: .primary,
      ),
      trailing: this.action,
    );
  }

}

class SettingListItem extends StatelessWidget {

  const SettingListItem({
    super.key,
    this.enabled = true,
    required this.icon,
    required this.label,
    required this.comment,
    required this.onPressed,
    required this.panelBuilder,
  });

  // ----------------

  final Boolean                                                                                          enabled;
  final IconData                                                                                         icon;
  final String                                                                                           label;
  final Iterable<Widget>                                                                                 comment;
  final Void Function(BuildContext context)?                                                             onPressed;
  final Iterable<Widget> Function(BuildContext context, Void Function(Void Function() action) setState)? panelBuilder;

  // ----------------

  @override
  build(context) {
    assertTest((this.onPressed == null) != (this.panelBuilder == null));
    return StyledListTile.standardCustom(
      enabled: this.enabled,
      padding: .fromLTRB(24, 0, 24, 0),
      leading: IconView.of(this.icon),
      content: TextStyleInheritedArea.of(
        style: StyledTypography.bodyMedium.query(context).copyWith(
          color: this.enabled ? null : StyledColor.disabled.query(context),
        ),
        align: .end,
        child: FlexContainer.horizontal([
          StyledText.custom(
            this.label,
            typography: .bodyLarge,
            color: this.enabled ? null : .disabled,
            align: .start,
          ).withFlexExpanded(),
          Gap.horizontal(16),
          ...this.comment,
        ]),
      ),
      onPressed: (context) async {
        if (this.onPressed != null) {
          this.onPressed!(context);
        }
        if (this.panelBuilder != null) {
          await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
            title: this.label,
            contentBuilder: this.panelBuilder!,
            actionBuilder: null,
          ));
        }
      },
    );
  }

}

extension SettingListItemExtension on SettingListItem {

  static SettingListItem buildForBooleanVariable({
    required BuildContext                                        context,
    required Void Function(Void Function() action)               setStateForOuter,
    required IconData                                            icon,
    required String                                              label,
    required ({String negative, String positive, String action}) comment,
    required Boolean Function()                                  getValue,
    required Void Function(Boolean value)                        setValue,
    required Void Function()                                     onUpdate,
  }) {
    return SettingListItem(
      icon: icon,
      label: label,
      comment: [
        StyledText.inherit(!getValue() ? comment.negative : comment.positive),
      ],
      onPressed: null,
      panelBuilder: (context, setStateForPanel) => [
        StyledListTile.standardTight(
          leading: StyledSwitch.standard(
            value: getValue(),
            onChanged: (context, value) async {
              setValue(value);
              await refreshState(setStateForPanel);
              await refreshState(setStateForOuter);
              onUpdate();
            },
          ),
          content: StyledText.inherit(comment.action),
        ),
      ],
    );
  }

  static SettingListItem buildForEnumerationVariable<TValue>({
    required BuildContext                          context,
    required Void Function(Void Function() action) setStateForOuter,
    required IconData                              icon,
    required String                                label,
    required List<TValue>                          optionValue,
    required String Function(TValue value)         renderValue,
    required TValue Function()                     getValue,
    required Void Function(TValue value)           setValue,
    required Void Function()                       onUpdate,
  }) {
    return SettingListItem(
      icon: icon,
      label: label,
      comment: [
        StyledText.inherit(renderValue(getValue())),
      ],
      onPressed: null,
      panelBuilder: (context, setStateForPanel) => [
        ...optionValue.map((item) => StyledListTile.standardTight(
          leading: StyledRadio.standard(
            value: getValue() == item,
            onChanged: (context) async {
              setValue(item);
              await refreshState(setStateForPanel);
              await refreshState(setStateForOuter);
              onUpdate();
            },
          ),
          content: StyledText.inherit(renderValue(item)),
        )),
      ],
    );
  }

  static SettingListItem buildForStringListVariable({
    required BuildContext                          context,
    required Void Function(Void Function() action) setStateForOuter,
    required IconData                              icon,
    required String                                label,
    required ({String negative, String positive})  comment,
    required List<String> Function()               getValue,
    required Void Function(List<String> value)     setValue,
    required String?                               pickerTag,
    required List<StoragePickType>?                pickerType,
    required Void Function()                       onUpdate,
  }) {
    return SettingListItem(
      icon: icon,
      label: label,
      comment: [
        StyledText.inherit(getValue().isEmpty ? comment.negative : comment.positive),
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
              if (pickerTag != null && pickerType != null) ...[
                StyledIconButton.standard(
                  tooltip: 'Pick',
                  icon: IconView.of(IconSet.open_in_new),
                  onPressed: (context) async {
                    var target = await MiscellaneousHelper.pickStorageItem(context, pickerTag, pickerType, true, null, null);
                    if (!target.isEmpty) {
                      setValue([...getValue(), ...target.map((it) => it.emit())]);
                      await refreshState(setStateForPanel);
                      await refreshState(setStateForOuter);
                      onUpdate();
                    }
                  },
                ),
              ],
            ],
            value: ConvertHelper.makeStringListToStringWithLine(getValue()),
            onChanged: (context, value) async {
              setValue(ConvertHelper.parseStringListFromStringWithLine(value));
              await refreshState(setStateForPanel);
              await refreshState(setStateForOuter);
              onUpdate();
            },
          ),
        ),
      ],
    );
  }

  static SettingListItem buildForStoragePathVariable({
    required BuildContext                          context,
    required Void Function(Void Function() action) setStateForOuter,
    required IconData                              icon,
    required String                                label,
    required ({String negative, String positive})  comment,
    required StoragePath Function()                getValue,
    required Void Function(StoragePath value)      setValue,
    required Boolean Function(StoragePath value)   checkValue,
    required String                                pickerTag,
    required List<StoragePickType>                 pickerType,
    required Void Function()                       onUpdate,
  }) {
    return SettingListItem(
      icon: icon,
      label: label,
      comment: [
        StyledText.inherit(!checkValue(getValue()) ? comment.negative : comment.positive),
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
                  var target = (await MiscellaneousHelper.pickStorageItem(context, pickerTag, pickerType, false, null, null)).firstOrNull;
                  if (target != null) {
                    setValue(target);
                    await refreshState(setStateForPanel);
                    await refreshState(setStateForOuter);
                    onUpdate();
                  }
                },
              ),
            ],
            value: getValue().emit(),
            onChanged: (context, value) async {
              setValue(.of(value));
              await refreshState(setStateForPanel);
              await refreshState(setStateForOuter);
              onUpdate();
            },
          ),
        ),
      ],
    );
  }

  static SettingListItem buildForStoragePathListVariable({
    required BuildContext                           context,
    required Void Function(Void Function() action)  setStateForOuter,
    required IconData                               icon,
    required String                                 label,
    required ({String negative, String positive})   comment,
    required List<StoragePath> Function()           getValue,
    required Void Function(List<StoragePath> value) setValue,
    required String                                 pickerTag,
    required List<StoragePickType>                  pickerType,
    required Void Function()                        onUpdate,
  }) {
    return SettingListItem(
      icon: icon,
      label: label,
      comment: [
        StyledText.inherit(getValue().isEmpty ? comment.negative : comment.positive),
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
                  var target = await MiscellaneousHelper.pickStorageItem(context, pickerTag, pickerType, true, null, null);
                  if (!target.isEmpty) {
                    setValue([...getValue(), ...target]);
                    await refreshState(setStateForPanel);
                    await refreshState(setStateForOuter);
                    onUpdate();
                  }
                },
              ),
            ],
            value: ConvertHelper.makeStringListToStringWithLine(getValue().map((it) => it.emit()).toList()),
            onChanged: (context, value) async {
              setValue(ConvertHelper.parseStringListFromStringWithLine(value).map(StoragePath.of).toList());
              await refreshState(setStateForPanel);
              await refreshState(setStateForOuter);
              onUpdate();
            },
          ),
        ),
      ],
    );
  }

}

// #endregion

// #region drawer

class NavigationDrawerLabel extends StatelessWidget {

  const NavigationDrawerLabel({
    super.key,
    required this.label,
  });

  // ----------------

  final String label;

  // ----------------

  @override
  build(context) {
    return BoxContainer.of(
      padding: .fromLTRB(28, 8, 28, 8),
      child: StyledText.custom(
        this.label,
        typography: .titleSmall,
        color: .primary,
      ),
    );
  }

}

class NavigationDrawerItem extends StatelessWidget {

  const NavigationDrawerItem({
    super.key,
    required this.selected,
    required this.icon,
    required this.label,
    required this.action,
    required this.onPressed,
  });

  // ----------------

  final Boolean                             selected;
  final IconData                            icon;
  final String                              label;
  final Iterable<Widget>                    action;
  final Void Function(BuildContext context) onPressed;

  // ----------------

  @override
  build(context) {
    return BoxContainer.of(
      constraints: .tightFor(height: 56),
      padding: .symmetric(horizontal: 12),
      child: material.TextButton(
        style: .new(
          padding: .all(.zero),
          backgroundColor: .all((!this.selected ? null : StyledColor.secondaryContainer)?.query(context)),
          foregroundColor: .all((!this.selected ? StyledColor.onSurface : StyledColor.surfaceTint).query(context)),
        ),
        child: FlexContainer.horizontal([
          Gap.horizontal(16),
          IconView.of(this.icon),
          Gap.horizontal(12),
          StyledText.custom(
            this.label,
            typography: .titleSmall,
            color: !this.selected ? .onSurface : .surfaceTint,
          ).withFlexExpanded(),
          Gap.horizontal(12),
          ...this.action,
          Gap.horizontal(12),
        ]),
        onPressed: () => this.onPressed(context),
      ),
    );
  }

}

class NavigationDrawerDivider extends StatelessWidget {

  const NavigationDrawerDivider({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return StyledDivider.standard(indent: 28);
  }

}

// #endregion

// #region dialog

extension MoreModalDialogExtension on StyledModalDialog {

  static Future<Boolean> showForConfirm(
    BuildContext context,
  ) async {
    return await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
      title: 'Confirm ?',
      contentBuilder: (context, setStateForPanel) => [],
      actionBuilder: (context) => [
        StyledButton.text(
          content: StyledText.inherit('Cancel'),
          onPressed: (context) => Navigator.pop(context, false),
        ),
        StyledButton.text(
          content: StyledText.inherit('Continue'),
          onPressed: (context) => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
  }

  static Future<Void> showForRevealStoragePath(
    BuildContext context,
    String       title,
    StoragePath  path,
  ) async {
    var canContinue = await StyledModalDialogExtension.show<Boolean>(context, StyledModalDialog.standard(
      title: title,
      contentBuilder: (context, setStateForPanel) => [
        StyledInput.outlined(
          type: .none,
          format: null,
          hint: null,
          prefix: null,
          suffix: null,
          value: path.emit(),
          onChanged: (context, value) async {
          },
        ),
      ],
      actionBuilder: (context) => [
        StyledButton.text(
          content: StyledText.inherit('Cancel'),
          onPressed: (context) => Navigator.pop(context, false),
        ),
        StyledButton.text(
          content: StyledText.inherit('Reveal'),
          onPressed: (context) => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
    if (canContinue) {
      await StorageHelper.reveal(path);
    }
    return;
  }

}

// #endregion

// #region storage

class StorageDropRegion extends StatelessWidget {

  const StorageDropRegion({
    super.key,
    required this.onDrop,
    required this.child,
  });

  // ----------------

  final Void Function(List<StoragePath> item)? onDrop;
  final Widget                                 child;

  // ----------------

  @override
  build(context) {
    return !(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh)
      ? this.child
      : lib.DropRegion(
        hitTestBehavior: .opaque,
        formats: [lib.Formats.fileUri],
        onDropOver: (event) async {
          if (this.onDrop != null && event.session.items.every((item) => item.canProvide(lib.Formats.fileUri))) {
            return .link;
          }
          return .none;
        },
        onPerformDrop: (event) async {
          var itemList = <Uri>[];
          for (var item in event.session.items) {
            var progress = item.dataReader!.getValue(lib.Formats.fileUri, (value) {
              value!;
              itemList.add(value);
              return;
            })!;
            while (progress.fraction.value != 1.0) {
              await Future.delayed(.zero);
            }
          }
          var result = <StoragePath>[];
          for (var uri in itemList) {
            var path = Uri.decodeComponent(uri.path);
            assertTest(path.startsWith('/'));
            if (SystemChecker.isWindows) {
              path = path.substring(1);
              if (uri.authority != '') {
                path = '//${uri.authority}/${path}';
              }
              path = (await PlatformIntegrationManager.instance.invokeOnWindowsQueryStorageLongPath(path)).destination;
            }
            if (SystemChecker.isLinux) {
              assertTest(uri.authority == '');
            }
            if (SystemChecker.isMacintosh) {
              assertTest(uri.authority == '');
              if (path.length > 1 && path[path.length - 1] == '/') {
                path = path.substring(0, path.length - 1);
              }
            }
            result.add(.of(path));
          }
          this.onDrop!(result);
          return;
        },
        child: this.child,
      );
  }

}

extension StorageDropRegionWidgetExtension on Widget {

  StorageDropRegion withStorageDropRegion({
    Void Function(List<StoragePath> item)? onDrop,
  }) {
    return .new(
      onDrop: onDrop,
      child: this,
    );
  }

}

// #endregion
