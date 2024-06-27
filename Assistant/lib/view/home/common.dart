import '/common.dart';
import '/utility/control_helper.dart';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:super_drag_and_drop/super_drag_and_drop.dart';

// ----------------

class CustomTitleBar extends StatelessWidget {

  const CustomTitleBar({
    super.key,
    required this.title,
  });

  // ----------------

  final String title;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Stack(
      alignment: Alignment.center,
      children: [
        if (Platform.isWindows || Platform.isLinux || Platform.isMacOS)
          const DragToMoveArea(
            child: SizedBox.expand(),
          ),
        Row(
          children: [
            const SizedBox(width: 8),
            IconButton(
              tooltip: 'Navigation',
              icon: const Icon(IconSymbols.menu),
              onPressed: () async {
                Scaffold.of(context).openDrawer();
              },
            ),
            const SizedBox(width: 16),
            Expanded(
              child: IgnorePointer(
                child: Text(
                  this.title,
                  overflow: TextOverflow.ellipsis,
                  style: theme.textTheme.titleLarge,
                ),
              ),
            ),
            if (Platform.isWindows || Platform.isLinux || Platform.isMacOS)
              Row(
                children: [
                  const SizedBox(width: 8),
                  IconButton(
                    icon: const Icon(IconSymbols.remove),
                    onPressed: () async {
                      if (await windowManager.isMinimized()) {
                        await windowManager.restore();
                      }
                      else {
                        await windowManager.minimize();
                      }
                    },
                  ),
                  const SizedBox(width: 4),
                  IconButton(
                    icon: const Icon(IconSymbols.crop_square, size: 20),
                    onPressed: () async {
                      if (await windowManager.isMaximized()) {
                        await windowManager.unmaximize();
                      }
                      else {
                        await windowManager.maximize();
                      }
                    },
                  ),
                  const SizedBox(width: 4),
                  IconButton(
                    icon: const Icon(IconSymbols.close),
                    onPressed: () async {
                      if (true) {
                        await windowManager.close();
                      }
                    },
                  ),
                ],
              ),
            const SizedBox(width: 8),
          ],
        ),
      ],
    );
  }

}

// ----------------

class CustomNavigationDrawerLabel extends StatelessWidget {

  const CustomNavigationDrawerLabel({
    super.key,
    required this.label,
  });

  // ----------------

  final String label;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Container(
      padding: const EdgeInsets.fromLTRB(28, 8, 28, 8),
      child: Text(
        this.label,
        overflow: TextOverflow.ellipsis,
        style: theme.textTheme.titleSmall?.copyWith(
          color: theme.colorScheme.primary,
        ),
      ),
    );
  }

}

class CustomNavigationDrawerItem extends StatelessWidget {

  const CustomNavigationDrawerItem({
    super.key,
    required this.selected,
    required this.icon,
    required this.label,
    required this.action,
    required this.onPressed,
  });

  // ----------------

  final Boolean         selected;
  final IconData        icon;
  final String          label;
  final List<Widget>    action;
  final Void Function() onPressed;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 12),
      height: 56,
      child: TextButton(
        style: ButtonStyle(
          padding: const WidgetStatePropertyAll(EdgeInsets.zero),
          backgroundColor: WidgetStatePropertyAll(!this.selected ? null : theme.colorScheme.secondaryContainer),
          foregroundColor: WidgetStatePropertyAll(!this.selected ? theme.colorScheme.onSurface : theme.colorScheme.surfaceTint),
        ),
        onPressed: this.onPressed,
        child: Row(
          children: [
            const SizedBox(width: 16),
            Icon(this.icon),
            const SizedBox(width: 12),
            Expanded(
              child: Text(
                this.label,
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.titleSmall?.copyWith(
                  color: !this.selected ? theme.colorScheme.onSurface : theme.colorScheme.surfaceTint,
                ),
              ),
            ),
            const SizedBox(width: 12),
            ...this.action,
            const SizedBox(width: 12),
          ],
        ),
      ),
    );
  }

}

class CustomNavigationDrawerDivider extends StatelessWidget {

  const CustomNavigationDrawerDivider({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return const Divider(height: 16, indent: 28, endIndent: 28);
  }

}

// ----------------

class CustomSettingLabel extends StatelessWidget {

  const CustomSettingLabel({
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
    var theme = Theme.of(context);
    return ListTile(
      dense: true,
      title: Text(
        this.label,
        overflow: TextOverflow.ellipsis,
        style: theme.textTheme.titleSmall?.copyWith(
          color: theme.colorScheme.primary,
        ),
      ),
      trailing: this.action,
    );
  }

}

class CustomSettingItem extends StatelessWidget {

  const CustomSettingItem({
    super.key,
    required this.enabled,
    required this.icon,
    required this.label,
    required this.content,
    required this.onTap,
    required this.panelBuilder,
  });

  // ----------------

  final Boolean                                                              enabled;
  final IconData                                                             icon;
  final String                                                               label;
  final List<Widget>                                                         content;
  final Void Function()?                                                     onTap;
  final List<Widget> Function(BuildContext, Void Function(Void Function()))? panelBuilder;

  // ----------------

  @override
  build(context) {
    return ListTile(
      enabled: this.enabled,
      leading: Icon(this.icon),
      title: Row(
        children: [
          Expanded(
            child: Text(
              this.label,
              overflow: TextOverflow.ellipsis,
            ),
          ),
          const SizedBox(width: 16),
          ...this.content,
        ],
      ),
      onTap: () async {
        assertTest((this.onTap == null) != (this.panelBuilder == null));
        if (this.onTap != null) {
          this.onTap!();
        }
        if (this.panelBuilder != null) {
          await ControlHelper.showCustomModalDialog(
            context: context,
            title: this.label,
            contentBuilder: this.panelBuilder!,
            actionBuilder: null,
          );
        }
      },
    );
  }

}

// ----------------

class CustomTextFieldSuffixWidget extends StatelessWidget {

  const CustomTextFieldSuffixWidget({
    super.key,
    required this.children,
  });

  // ----------------

  final List<Widget> children;

  // ----------------

  @override
  build(context) {
    return Row(
      mainAxisSize: MainAxisSize.min,
      children: this.children,
    );
  }

}

// ----------------

class CustomFileDropRegion extends StatelessWidget {

  const CustomFileDropRegion({
    super.key,
    required this.onDrop,
    required this.child,
  });

  // ----------------

  final Void Function(List<String>)? onDrop;
  final Widget                       child;

  // ----------------

  @override
  build(context) {
    return !(Platform.isWindows || Platform.isLinux || Platform.isMacOS)
      ? this.child
      : DropRegion(
        formats: const [Formats.fileUri],
        onDropOver: (event) async {
          if (this.onDrop != null && event.session.items.every((item) => item.canProvide(Formats.fileUri))) {
            return DropOperation.link;
          }
          return DropOperation.none;
        },
        onPerformDrop: (event) async {
              print('done ${this.hashCode}');
          var result = <String>[];
          for (var item in event.session.items) {
            var progress = item.dataReader!.getValue(Formats.fileUri, (uri) async {
              uri!;
              var path = Uri.decodeComponent(uri.path);
              assertTest(path.startsWith('/'));
              if (Platform.isWindows) {
                path = path.substring(1);
                if (uri.authority != '') {
                  path = '//${uri.authority}/${path}';
                }
              }
              if (Platform.isLinux) {
                assertTest(uri.authority == '');
              }
              if (Platform.isMacOS) {
                if (path.length > 1 && path[path.length - 1] == '/') {
                  path = path.substring(0, path.length - 1);
                }
                assertTest(uri.authority == '');
              }
              result.add(path);
            })!;
            while (progress.fraction.value != 1.0) {
              await Future.delayed(const Duration());
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

class CustomModulePage extends StatelessWidget {

  const CustomModulePage({
    super.key,
    required this.onDropFile,
    required this.content,
    required this.bottom,
  });

  // ----------------

  final Void Function(List<String>)? onDropFile;
  final Widget                       content;
  final Widget                       bottom;

  // ----------------

  @override
  build(context) {
    return CustomFileDropRegion(
      onDrop: this.onDropFile,
      child: Column(
        children: [
          Expanded(
            child: this.content,
          ),
          BottomAppBar(
            child: this.bottom,
          ),
        ],
      ),
    );
  }

}
