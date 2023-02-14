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
                    '个性化',
                    style: theme.textTheme.titleSmall!.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.auto_mode),
                  title: const Text('主题模式'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          ['系统', '亮色', '暗色'][setting.data.mThemeMode.index],
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('主题模式'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...ThemeMode.values.map(
                              (e) => ListTile(
                                title: Text(['系统', '亮色', '暗色'][e.index]),
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
                            child: const Text('完成'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.color_lens),
                  title: const Text('主题配色'),
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
                        title: const Text('主题配色'),
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
                                Text('继承系统主题配色', style: theme.textTheme.titleMedium),
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
                            child: const Text('完成'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.text_increase),
                  title: const Text('字体尺寸'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !setting.data.mUseLargerFontInConsole ? '常规' : '较大',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('字体尺寸'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...[false, true].map(
                              (e) => ListTile(
                                title: Text(!e ? '常规' : '较大'),
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
                    '工具设置',
                    style: theme.textTheme.titleSmall!.copyWith(color: theme.colorScheme.primary),
                  ),
                ),
                ListTile(
                  leading: const Icon(Icons.data_usage),
                  title: const Text('核心'),
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
                        title: const Text('核心'),
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
                            child: const Text('完成'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.data_object),
                  title: const Text('脚本'),
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
                        title: const Text('脚本'),
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
                            child: const Text('完成'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.list),
                  title: const Text('参数'),
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
                        title: const Text('参数'),
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
                            child: const Text('完成'),
                          ),
                        ],
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.exit_to_app),
                  title: const Text('命令完成后行为'),
                  trailing: SizedBox(
                    width: 64,
                    child: Row(
                      children: [
                        Expanded(child: Container()),
                        Text(
                          !setting.data.mExitAfterCommandSucceed ? '驻留' : '退出',
                          style: theme.textTheme.bodyMedium,
                        ),
                      ],
                    ),
                  ),
                  onTap: () {
                    showDialog<String>(
                      context: context,
                      builder: (context) => AlertDialog(
                        title: const Text('命令完成后行为'),
                        content: Column(
                          mainAxisSize: MainAxisSize.min,
                          children: [
                            ...[false, true].map(
                              (e) => ListTile(
                                title: Text(!e ? '驻留' : '退出'),
                                leading: Radio<Boolean>(
                                  value: e,
                                  groupValue: setting.data.mExitAfterCommandSucceed,
                                  onChanged: (value) {
                                    setting.data.mExitAfterCommandSucceed = value!;
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
                            child: const Text('完成'),
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
                    child: const Text('重置'),
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
