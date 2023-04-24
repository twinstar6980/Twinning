// ignore_for_file: unused_import, unnecessary_cast

import '/common.dart';
import 'dart:io';
import 'package:flutter/widgets.dart';
import 'package:window_manager/window_manager.dart';
import '/application.dart';
import '/setting.dart';
import '/command.dart';
import '/platform_method.dart';
import '/notification_helper.dart';

// ----------------

Future<Void>
main(
  List<String> argument,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    await WindowManager.instance.ensureInitialized();
    await windowManager.center();
    await windowManager.waitUntilReadyToShow();
    await windowManager.show();
  }
  await NotificationHelper.initialize();
  var setting = Setting.init();
  try {
    setting = await Setting.load();
  } catch (e) {
    await Setting.save(setting);
  }
  var commandSource = <String>[];
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    commandSource = argument;
  }
  if (Platform.isAndroid) {
    commandSource = (await gPlatformMethod!.invokeMethod('getCommand', <String, dynamic>{}) as List<Object?>).map((e) => e as String).toList();
  }
  var command = null as Command?;
  if (commandSource.isNotEmpty) {
    command = Command('', '', []);
    if (commandSource[0] == '-additional') {
      command.core = setting.mCore.substring(0);
      command.script = setting.mScript.substring(0);
      command.argument = setting.mArgument.sublist(0);
      command.argument.addAll(commandSource.sublist(1));
    } else {
      command.core = commandSource.elementAt(0);
      command.script = commandSource.elementAt(1);
      command.argument = commandSource.sublist(2);
    }
  }
  runApp(Application(setting: setting, command: command));
  return;
}
