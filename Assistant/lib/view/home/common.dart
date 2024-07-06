import '/common.dart';
import '/utility/control_helper.dart';
import 'dart:io';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:super_drag_and_drop/super_drag_and_drop.dart';

// ----------------

class CustomModalDialog extends StatefulWidget {

  const CustomModalDialog({
    super.key,
    required this.title,
    required this.contentBuilder,
    required this.actionBuilder,
  });

  // ----------------

  final String                                                              title;
  final List<Widget> Function(BuildContext, Void Function(Void Function())) contentBuilder;
  final List<Widget> Function(BuildContext)?                                actionBuilder;

  // ----------------

  @override
  createState() => _CustomModalDialogState();

}

class _CustomModalDialogState extends State<CustomModalDialog> {

  late ScrollController _scrollController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._scrollController = ScrollController();
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._scrollController.dispose();
    this._scrollController = ScrollController();
    return;
  }

  @override
  dispose() {
    this._scrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return AlertDialog(
      title: Text(
        this.widget.title,
        overflow: TextOverflow.ellipsis,
      ),
      content: Scrollbar(
        interactive: true,
        controller: this._scrollController,
        child: SingleChildScrollView(
          controller: this._scrollController,
          child: StatefulBuilder(
            builder: (context, setState) => Column(
              mainAxisSize: MainAxisSize.min,
              children: this.widget.contentBuilder(context, setState),
            ),
          ),
        ),
      ),
      actions: this.widget.actionBuilder == null
        ? [
          TextButton(
            onPressed: () => Navigator.pop(context, null),
            child: const Text('Okay'),
          ),
        ]
        : this.widget.actionBuilder!(context),
    );
  }

}

// ----------------

class CustomModalBottomSheet extends StatefulWidget {

  const CustomModalBottomSheet({
    super.key,
    required this.title,
    required this.contentBuilder,
  });

  // ----------------

  final String                                                              title;
  final List<Widget> Function(BuildContext, Void Function(Void Function())) contentBuilder;

  // ----------------

  @override
  createState() => _CustomModalBottomSheetState();

}

class _CustomModalBottomSheetState extends State<CustomModalBottomSheet> {

  late ScrollController _scrollController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._scrollController = ScrollController();
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._scrollController.dispose();
    this._scrollController = ScrollController();
    return;
  }

  @override
  dispose() {
    this._scrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var theme = Theme.of(context);
    return Column(
      children: [
        Container(
          padding: const EdgeInsets.fromLTRB(24, 12, 24, 12),
          child: Row(
            children: [
              Expanded(
                child: Text(
                  this.widget.title,
                  overflow: TextOverflow.ellipsis,
                  style: theme.textTheme.titleMedium?.copyWith(
                    color: theme.colorScheme.primary,
                  ),
                ),
              ),
            ],
          ),
        ),
        const Divider(height: 1, indent: 12, endIndent: 12),
        Expanded(
          child: Scrollbar(
            interactive: true,
            controller: this._scrollController,
            child: SingleChildScrollView(
              controller: this._scrollController,
              child: StatefulBuilder(
                builder: (context, setState) => Column(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    ...this.widget.contentBuilder(context, setState),
                    SizedBox(height: MediaQuery.of(context).viewPadding.bottom),
                  ],
                ),
              ),
            ),
          ),
        ),
      ],
    );
  }

}

// ----------------

class CustomTitleBar extends StatelessWidget {

  const CustomTitleBar({
    super.key,
    required this.title,
    required this.leading,
  });

  // ----------------

  final String title;
  final Widget leading;

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
            this.leading,
            const SizedBox(width: 12),
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
      contentPadding: const EdgeInsets.fromLTRB(24, 0, 24, 0),
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
      contentPadding: const EdgeInsets.fromLTRB(24, 0, 24, 0),
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
          await ControlHelper.showCustomModalDialog<Void>(context, CustomModalDialog(
            title: this.label,
            contentBuilder: this.panelBuilder!,
            actionBuilder: null,
          ));
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
      children: [
        const SizedBox(width: 4),
        ...this.children,
        const SizedBox(width: 4),
      ],
    );
  }

}

class CustomTextFieldWithFocus extends StatefulWidget {

  const CustomTextFieldWithFocus({
    super.key,
    this.style = null,
    required this.keyboardType,
    required this.inputFormatters,
    required this.decoration,
    required this.value,
    required this.onChanged,
  });

  // ----------------

  final TextStyle?                  style;
  final TextInputType               keyboardType;
  final List<TextInputFormatter>    inputFormatters;
  final InputDecoration             decoration;
  final String                      value;
  final Void Function(String value) onChanged;

  // ----------------

  @override
  createState() => _CustomTextFieldWithFocusState();

}

class _CustomTextFieldWithFocusState extends State<CustomTextFieldWithFocus> {

  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._valueController = TextEditingController();
    this._valueController.text = this.widget.value;
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._valueController.text = this.widget.value;
    return;
  }

  @override
  dispose() {
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return Focus(
      onFocusChange: (value) async {
        if (!value) {
          this.widget.onChanged(this._valueController.text);
        }
      },
      child: TextField(
        style: this.widget.style,
        keyboardType: this.widget.keyboardType,
        maxLines: this.widget.keyboardType == TextInputType.multiline ? null : 1,
        inputFormatters: this.widget.inputFormatters,
        decoration: this.widget.decoration,
        controller: this._valueController,
      ),
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

class CustomModulePageLayout extends StatelessWidget {

  const CustomModulePageLayout({
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

abstract class CustomModulePageState {

  // #region action

  Future<Void> modulePageApplyOption(
    List<String> optionView,
  );

  Future<List<String>> modulePageCollectOption(
  );

  // ----------------

  Future<Boolean> modulePageRequestClose(
  );

  // #endregion

}
