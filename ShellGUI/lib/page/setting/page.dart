import '/common.dart';
import '/utility/permission_helper.dart';
import '/setting.dart';
import 'dart:io';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class SettingPage extends StatefulWidget {

  const SettingPage({
    super.key,
  });

  @override
  createState() => _SettingPageState();

  // ----------------

}

class _SettingPageState extends State<SettingPage> {

  String? _applicationSharedDirectory = null;
  Boolean _hasStoragePermission = false;

  // ----------------

  @override
  initState() {
    super.initState();
    (() async {
      this._applicationSharedDirectory = await queryApplicationSharedDirectory();
      this._hasStoragePermission = await PermissionHelper.checkStoragePermission();
      this.setState(() {});
    })();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return Container(
      padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
      child: Column(
        children: [
          Expanded(
            child: ListView(
              children: [
                const SizedBox(height: 8),
                ListTile(
                  dense: true,
                  title: Text(
                    'Appearance',
                    style: theme.textTheme.titleSmall?.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.auto_mode_outlined),
                  title: const Text('Theme Mode'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          ['System', 'Light', 'Dark'][setting.data.mThemeMode.index],
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Theme Mode'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...ThemeMode.values.map(
                              (mode) => ListTile(
                                title: Text(['System', 'Light', 'Dark'][mode.index]),
                                leading: Radio<ThemeMode>(
                                  value: mode,
                                  groupValue: setting.data.mThemeMode,
                                  onChanged: (value) {
                                    value!;
                                    setting.data.mThemeMode = value;
                                    setting.update();
                                  },
                                ),
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.color_lens_outlined),
                  title: const Text('Theme Color'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        setting.data.mThemeColorInheritFromSystem
                        ? Text(
                          'System',
                          style: theme.textTheme.bodyMedium,
                        )
                        : Row(
                          children: [
                            Icon(
                              Icons.light_mode,
                              color: setting.data.mThemeColorLight,
                            ),
                            const SizedBox(width: 8),
                            Icon(
                              Icons.dark_mode,
                              color: setting.data.mThemeColorDark,
                            ),
                          ],
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Theme Color'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Row(
                              children: [
                                Switch(
                                  value: setting.data.mThemeColorInheritFromSystem,
                                  onChanged: (value) {
                                    setting.data.mThemeColorInheritFromSystem = value;
                                    setting.update();
                                  },
                                ),
                                const SizedBox(width: 12),
                                Text('Inherit From System', style: theme.textTheme.titleMedium),
                              ],
                            ),
                            const SizedBox(height: 8),
                            ListTile(
                              title: Focus(
                                onFocusChange: (value) {
                                  if (!value) {
                                    setting.update();
                                  }
                                },
                                child: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  maxLines: 1,
                                  keyboardType: TextInputType.text,
                                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9a-fA-F]'))],
                                  initialValue: setting.data.mThemeColorLight.withOpacity(0.0).value.toRadixString(16).padRight(6, '0'),
                                  onChanged: (value) {
                                    setting.data.mThemeColorLight = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withOpacity(1.0);
                                  },
                                ),
                              ),
                              leading: const Icon(Icons.light_mode_outlined),
                              trailing: Icon(Icons.circle, color: setting.data.mThemeColorLight),
                            ),
                            ListTile(
                              title: Focus(
                                onFocusChange: (value) {
                                  if (!value) {
                                    setting.update();
                                  }
                                },
                                child: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  maxLines: 1,
                                  keyboardType: TextInputType.text,
                                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9a-fA-F]'))],
                                  initialValue: setting.data.mThemeColorDark.withOpacity(0.0).value.toRadixString(16).padRight(6, '0'),
                                  onChanged: (value) {
                                    setting.data.mThemeColorDark = Color(Integer.tryParse(value, radix: 16) ?? 0x000000).withOpacity(1.0);
                                  },
                                ),
                              ),
                              leading: const Icon(Icons.dark_mode_outlined),
                              trailing: Icon(Icons.circle, color: setting.data.mThemeColorDark),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.font_download_outlined),
                  title: const Text('Primary Font'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          '${setting.data.mPrimaryFont.length}',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Primary Font'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Focus(
                              onFocusChange: (value) {
                                if (!value) {
                                  setting.update();
                                }
                              },
                              child: TextFormField(
                                decoration: const InputDecoration(
                                  isDense: true,
                                ),
                                maxLines: null,
                                keyboardType: TextInputType.multiline,
                                inputFormatters: const [],
                                initialValue: convertStringListToTextWithLine(setting.data.mPrimaryFont),
                                onChanged: (value) {
                                  setting.data.mPrimaryFont = convertStringListFromTextWithLine(value);
                                },
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.text_format_outlined),
                  title: const Text('Console Font'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !setting.data.mConsoleFontUseLargerSize ? 'Regular' : 'Larger',
                          style: theme.textTheme.bodyMedium,
                        ),
                        const SizedBox(width: 8),
                        Text(
                          '${setting.data.mConsoleFont.length}',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Console Font'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Row(
                              children: [
                                Switch(
                                  value: setting.data.mConsoleFontUseLargerSize,
                                  onChanged: (value) {
                                    setting.data.mConsoleFontUseLargerSize = value;
                                    setting.update();
                                  },
                                ),
                                const SizedBox(width: 12),
                                Text('Use Larger Size', style: theme.textTheme.titleMedium),
                              ],
                            ),
                            const SizedBox(height: 8),
                            Focus(
                              onFocusChange: (value) {
                                if (!value) {
                                  setting.update();
                                }
                              },
                              child: TextFormField(
                                decoration: const InputDecoration(
                                  isDense: true,
                                ),
                                maxLines: null,
                                keyboardType: TextInputType.multiline,
                                inputFormatters: const [],
                                initialValue: convertStringListToTextWithLine(setting.data.mConsoleFont),
                                onChanged: (value) {
                                  setting.data.mConsoleFont = convertStringListFromTextWithLine(value);
                                },
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  enabled: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
                  leading: const Icon(Icons.pivot_table_chart_outlined),
                  title: const Text('Window Position'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          setting.data.mWindowPositionAlignToCenter ? 'Center' : '${setting.data.mWindowPositionX}x${setting.data.mWindowPositionY}',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Window Position'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Row(
                              children: [
                                Switch(
                                  value: setting.data.mWindowPositionAlignToCenter,
                                  onChanged: (value) {
                                    setting.data.mWindowPositionAlignToCenter = value;
                                    setting.update();
                                  },
                                ),
                                const SizedBox(width: 12),
                                Text('Align To Center', style: theme.textTheme.titleMedium),
                              ],
                            ),
                            const SizedBox(height: 8),
                            ListTile(
                              title: Focus(
                                onFocusChange: (value) {
                                  if (!value) {
                                    setting.update();
                                  }
                                },
                                child: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  maxLines: 1,
                                  keyboardType: TextInputType.number,
                                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                                  initialValue: setting.data.mWindowPositionX.toString(),
                                  onChanged: (value) {
                                    setting.data.mWindowPositionX = Integer.tryParse(value) ?? 0;
                                  },
                                ),
                              ),
                              leading: SizedBox(
                                width: 16,
                                child: Container(
                                  alignment: Alignment.center,
                                  child: Text('X', style: theme.textTheme.titleMedium),
                                ),
                              ),
                            ),
                            ListTile(
                              title: Focus(
                                onFocusChange: (value) {
                                  if (!value) {
                                    setting.update();
                                  }
                                },
                                child: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  maxLines: 1,
                                  keyboardType: TextInputType.number,
                                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                                  initialValue: setting.data.mWindowPositionY.toString(),
                                  onChanged: (value) {
                                    setting.data.mWindowPositionY = Integer.tryParse(value) ?? 0;
                                  },
                                ),
                              ),
                              leading: SizedBox(
                                width: 16,
                                child: Container(
                                  alignment: Alignment.center,
                                  child: Text('Y', style: theme.textTheme.titleMedium),
                                ),
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  enabled: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
                  leading: const Icon(Icons.fit_screen_outlined),
                  title: const Text('Window Size'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          setting.data.mWindowSizeAdhereToDefault ? 'Default' : '${setting.data.mWindowSizeWidth}x${setting.data.mWindowSizeHeight}',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Window Size'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Row(
                              children: [
                                Switch(
                                  value: setting.data.mWindowSizeAdhereToDefault,
                                  onChanged: (value) {
                                    setting.data.mWindowSizeAdhereToDefault = value;
                                    setting.update();
                                  },
                                ),
                                const SizedBox(width: 12),
                                Text('Adhere To Default', style: theme.textTheme.titleMedium),
                              ],
                            ),
                            const SizedBox(height: 8),
                            ListTile(
                              title: Focus(
                                onFocusChange: (value) {
                                  if (!value) {
                                    setting.update();
                                  }
                                },
                                child: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  maxLines: 1,
                                  keyboardType: TextInputType.number,
                                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                                  initialValue: setting.data.mWindowSizeWidth.toString(),
                                  onChanged: (value) {
                                    setting.data.mWindowSizeWidth = Integer.tryParse(value) ?? 0;
                                  },
                                ),
                              ),
                              leading: SizedBox(
                                width: 16,
                                child: Container(
                                  alignment: Alignment.center,
                                  child: Text('W', style: theme.textTheme.titleMedium),
                                ),
                              ),
                            ),
                            ListTile(
                              title: Focus(
                                onFocusChange: (value) {
                                  if (!value) {
                                    setting.update();
                                  }
                                },
                                child: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  maxLines: 1,
                                  keyboardType: TextInputType.number,
                                  inputFormatters: [FilteringTextInputFormatter.allow(RegExp(r'[0-9]'))],
                                  initialValue: setting.data.mWindowSizeHeight.toString(),
                                  onChanged: (value) {
                                    setting.data.mWindowSizeHeight = Integer.tryParse(value) ?? 0;
                                  },
                                ),
                              ),
                              leading: SizedBox(
                                width: 16,
                                child: Container(
                                  alignment: Alignment.center,
                                  child: Text('H', style: theme.textTheme.titleMedium),
                                ),
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  dense: true,
                  title: Text(
                    'Console',
                    style: theme.textTheme.titleSmall?.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.data_usage_outlined),
                  title: const Text('Kernel'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          setting.data.mConsoleKernel.isEmpty ? '?' : '!',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Kernel'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Focus(
                              onFocusChange: (value) {
                                if (!value) {
                                  setting.update();
                                }
                              },
                              child: TextFormField(
                                decoration: const InputDecoration(
                                  isDense: true,
                                ),
                                maxLines: null,
                                keyboardType: TextInputType.text,
                                inputFormatters: const [],
                                initialValue: setting.data.mConsoleKernel,
                                onChanged: (value) {
                                  setting.data.mConsoleKernel = value;
                                },
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.data_object_outlined),
                  title: const Text('Script'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          setting.data.mConsoleScript.isEmpty ? '?' : '!',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Script'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Focus(
                              onFocusChange: (value) {
                                if (!value) {
                                  setting.update();
                                }
                              },
                              child: TextFormField(
                                decoration: const InputDecoration(
                                  isDense: true,
                                ),
                                maxLines: null,
                                keyboardType: TextInputType.text,
                                inputFormatters: const [],
                                initialValue: setting.data.mConsoleScript,
                                onChanged: (value) {
                                  setting.data.mConsoleScript = value;
                                },
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.list_alt_outlined),
                  title: const Text('Argument'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          '${setting.data.mConsoleArgument.length}',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Argument'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Focus(
                              onFocusChange: (value) {
                                if (!value) {
                                  setting.update();
                                }
                              },
                              child: TextFormField(
                                decoration: const InputDecoration(
                                  isDense: true,
                                ),
                                maxLines: null,
                                keyboardType: TextInputType.multiline,
                                inputFormatters: const [],
                                initialValue: convertStringListToTextWithLine(setting.data.mConsoleArgument),
                                onChanged: (value) {
                                  setting.data.mConsoleArgument = convertStringListFromTextWithLine(value);
                                },
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  dense: true,
                  title: Text(
                    'Storage',
                    style: theme.textTheme.titleSmall?.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.folder_special_outlined),
                  title: const Text('Shared Directory'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          this._applicationSharedDirectory == null ? 'Unavailable' : 'Available',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Shared Directory'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            TextFormField(
                              decoration: const InputDecoration(
                                isDense: true,
                                border: OutlineInputBorder(),
                              ),
                              maxLines: null,
                              keyboardType: TextInputType.text,
                              inputFormatters: const [],
                              initialValue: this._applicationSharedDirectory,
                              readOnly: true,
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  enabled: Platform.isAndroid,
                  leading: const Icon(Icons.storage_outlined),
                  title: const Text('Storage Permission'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !this._hasStoragePermission ? 'Denied' : 'Granted',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    this._hasStoragePermission = await PermissionHelper.requestStoragePermission();
                    this.setState(() {});
                  },
                ),
                ListTile(
                  enabled: Platform.isAndroid,
                  leading: const Icon(Icons.folder_copy_outlined),
                  title: const Text('Fallback Directory'),
                  trailing: SizedBox(
                    width: 120,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !Directory(setting.data.mFallbackDirectory).existsSync() ? 'Invalid' : 'Valid',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    await showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Fallback Directory'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            Focus(
                              onFocusChange: (value) {
                                if (!value) {
                                  setting.update();
                                }
                              },
                              child: TextFormField(
                                decoration: const InputDecoration(
                                  isDense: true,
                                ),
                                maxLines: null,
                                keyboardType: TextInputType.text,
                                inputFormatters: const [],
                                initialValue: setting.data.mFallbackDirectory,
                                onChanged: (value) {
                                  setting.data.mFallbackDirectory = value;
                                },
                              ),
                            ),
                          ],
                        ),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context, 'OK'),
                            child: const Text('OK'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                const SizedBox(height: 16),
              ],
            ),
          ),
        ],
      ),
    );
  }

}
