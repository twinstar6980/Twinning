// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '/setting.dart';

// ----------------

class SettingPage extends StatefulWidget {

  const SettingPage({
    super.key,
  });

  @override
  State<SettingPage> createState() => _SettingPageState();

  // ----------------

}

class _SettingPageState extends State<SettingPage> {

  @override
  Widget build(BuildContext context) {
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
                    'Customize',
                    style: theme.textTheme.titleSmall!.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.auto_mode),
                  title: const Text('Theme Mode'),
                  trailing: SizedBox(
                    width: 64,
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
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Theme Mode'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...ThemeMode.values.map(
                              (e) => ListTile(
                                title: Text(['System', 'Light', 'Dark'][e.index]),
                                leading: Radio<ThemeMode>(
                                  value: e,
                                  groupValue: setting.data.mThemeMode,
                                  onChanged: (value) {
                                    setting.data.mThemeMode = value!;
                                    setting.notify();
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
                  leading: const Icon(Icons.color_lens),
                  title: const Text('Theme Color'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
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
                  ),
                  onTap: () {
                    showDialog<String>(
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
                                    setting.notify();
                                  },
                                ),
                                const SizedBox(width: 12),
                                Text('Inherit From System', style: theme.textTheme.titleMedium),
                              ],
                            ),
                            ...[0, 1].map(
                              (e) => ListTile(
                                title: TextFormField(
                                  decoration: const InputDecoration(
                                    isDense: true,
                                  ),
                                  initialValue: [setting.data.mThemeColorLight, setting.data.mThemeColorDark][e].withOpacity(0.0).value.toRadixString(16).padRight(6, '0'),
                                  onChanged: (value) {
                                    var color = Color(value.isEmpty ? 0 : Integer.parse(value, radix: 16)).withOpacity(1.0);
                                    if (e == 0) {
                                      setting.data.mThemeColorLight = color;
                                    } else {
                                      setting.data.mThemeColorDark = color;
                                    }
                                    setting.notify();
                                  },
                                ),
                                leading: Icon([Icons.light_mode_outlined, Icons.dark_mode_outlined][e]),
                                trailing: Icon(
                                  Icons.circle,
                                  color: [setting.data.mThemeColorLight, setting.data.mThemeColorDark][e],
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
                  leading: const Icon(Icons.text_increase),
                  title: const Text('Font Size'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !setting.data.mUseLargerFontInConsole ? 'Regular' : 'Larger',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Font Size'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...[false, true].map(
                              (e) => ListTile(
                                title: Text(!e ? 'Regular' : 'Larger'),
                                leading: Radio<Boolean>(
                                  value: e,
                                  groupValue: setting.data.mUseLargerFontInConsole,
                                  onChanged: (value) {
                                    setting.data.mUseLargerFontInConsole = value!;
                                    setting.notify();
                                  },
                                ),
                              ),
                            ),
                          ],
                        ),
                      ),
                    );
                  },
                ),
                ListTile(
                  dense: true,
                  title: Text(
                    'ToolKit',
                    style: theme.textTheme.titleSmall!.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.data_usage),
                  title: const Text('Core'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          setting.data.mCore.isEmpty ? '?' : '~',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () async {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Core'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            TextFormField(
                              decoration: const InputDecoration(
                                isDense: true,
                              ),
                              initialValue: setting.data.mCore,
                              onChanged: (value) {
                                setting.data.mCore = value;
                                setting.notify();
                              },
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
                  leading: const Icon(Icons.data_object),
                  title: const Text('Script'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          setting.data.mScript.isEmpty ? '?' : '~',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Script'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            TextFormField(
                              decoration: const InputDecoration(
                                isDense: true,
                              ),
                              initialValue: setting.data.mScript,
                              onChanged: (value) {
                                setting.data.mScript = value;
                                setting.notify();
                              },
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
                  leading: const Icon(Icons.list),
                  title: const Text('Argument'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          '${setting.data.mArgument.length}',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Argument'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            TextFormField(
                              maxLines: null,
                              decoration: const InputDecoration(
                                isDense: true,
                              ),
                              initialValue: setting.data.mArgument.join('\n') + (setting.data.mArgument.isNotEmpty && setting.data.mArgument.last.isEmpty ? '\n' : ''),
                              onChanged: (value) {
                                var valueList = value.split('\n');
                                if (valueList.isNotEmpty && valueList.last.isEmpty) {
                                  valueList.removeLast();
                                }
                                setting.data.mArgument = valueList;
                                setting.notify();
                              },
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
                  leading: const Icon(Icons.exit_to_app),
                  title: const Text('Behavior After Command Succeed'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !setting.data.mBehaviorAfterCommandSucceed ? 'Stay' : 'Exit',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('Behavior After Command Succeed'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...[false, true].map(
                              (e) => ListTile(
                                title: Text(!e ? 'Stay' : 'Exit'),
                                leading: Radio<Boolean>(
                                  value: e,
                                  groupValue: setting.data.mBehaviorAfterCommandSucceed,
                                  onChanged: (value) {
                                    setting.data.mBehaviorAfterCommandSucceed = value!;
                                    setting.notify();
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
                const SizedBox(height: 16),
                Row(children: [
                  Expanded(child: Container()),
                  OutlinedButton(
                    onPressed: () {
                      setting.reset();
                    },
                    child: const Text('Reset'),
                  ),
                  Expanded(child: Container()),
                ]),
                const SizedBox(height: 8),
              ],
            ),
          ),
        ],
      ),
    );
  }

}
