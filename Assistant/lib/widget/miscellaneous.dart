import '/common.dart';
import '/utility/storage_helper.dart';
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

  final Boolean                                                                                   enabled;
  final IconData                                                                                  icon;
  final String                                                                                    label;
  final Iterable<Widget>                                                                          comment;
  final Void Function(BuildContext context)?                                                      onPressed;
  final Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState)? panelBuilder;

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

Future<Boolean> showDialogForConfirm(
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

Future<Void> showDialogForRevealStoragePath(
  BuildContext context,
  String       title,
  String       path,
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
        value: path,
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

// #endregion

// #region storage

class StorageDropRegion extends StatelessWidget {

  const StorageDropRegion({
    super.key,
    required this.onDrop,
    required this.child,
  });

  // ----------------

  final Void Function(List<String> item)? onDrop;
  final Widget                            child;

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
          var result = <String>[];
          for (var item in event.session.items) {
            var progress = item.dataReader!.getValue(lib.Formats.fileUri, (uri) async {
              uri!;
              var path = Uri.decodeComponent(uri.path);
              assertTest(path.startsWith('/'));
              if (SystemChecker.isWindows) {
                path = path.substring(1);
                if (uri.authority != '') {
                  path = '//${uri.authority}/${path}';
                }
              }
              if (SystemChecker.isLinux) {
                assertTest(uri.authority == '');
              }
              if (SystemChecker.isMacintosh) {
                if (path.length > 1 && path[path.length - 1] == '/') {
                  path = path.substring(0, path.length - 1);
                }
                assertTest(uri.authority == '');
              }
              result.add(path);
            })!;
            while (progress.fraction.value != 1.0) {
              await Future.delayed(.zero);
            }
          }
          this.onDrop!(result);
          return;
        },
        child: this.child,
      );
  }

}

// ----------------

Future<String?> pickStorageItem({
  required BuildContext context,
  Boolean               allowLoadFile = false,
  Boolean               allowLoadDirectory = false,
  Boolean               allowSaveFile = false,
  required String       location,
  TextStyle?            textStyle = null, // TODO: remove?
}
) async {
  var type = null as String?;
  var allowedTypeCount = [allowLoadFile, allowLoadDirectory, allowSaveFile].where((value) => value).length;
  assertTest(allowedTypeCount != 0);
  if (allowedTypeCount == 1) {
    if (allowLoadFile) {
      type = 'load_file';
    }
    if (allowLoadDirectory) {
      type = 'load_directory';
    }
    if (allowSaveFile) {
      type = 'save_file';
    }
  }
  else {
    type = await StyledMenuExtension.show<String>(context, StyledMenu.standard(
      position: .under,
      children: [
        if (allowLoadFile)
          ('load_file', 'Load File'),
        if (allowLoadDirectory)
          ('load_directory', 'Load Directory'),
        if (allowSaveFile)
          ('save_file', 'Save File'),
      ].map((value) => StyledMenuItem.standard(
        value: value.$1,
        content: StyledText.custom(
          value.$2,
          style: textStyle,
        ),
      )),
    ));
  }
  return type == null ? null : await StorageHelper.pick(type, context, location, null);
}

// #endregion
