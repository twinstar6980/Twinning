// ignore_for_file: unused_import

import '/common.dart';
import 'dart:io';
import 'package:flutter/widgets.dart';
import 'package:window_manager/window_manager.dart';
import '/application.dart';
import '/platform_method.dart';

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
  var command = argument.isEmpty ? null : argument.sublist(1);
  if (Platform.isAndroid) {
    var methodArgument = <String, dynamic>{};
    var methodResult = await gPlatformMethod!.invokeMethod('getCommand', methodArgument) as List<Object?>?;
    command = methodResult?.map((e) => e as String).toList();
  }
  runApp(Application(command: command));
  return;
}
