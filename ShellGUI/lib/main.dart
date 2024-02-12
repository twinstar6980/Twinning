import '/common.dart';
import '/utility/platform_method.dart';
import '/utility/notification_helper.dart';
import '/setting.dart';
import '/application.dart';
import 'dart:io';
import 'dart:ui';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';

// ----------------

Future<Void> main(
  List<String> argument,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  var navigatorKey = GlobalKey<NavigatorState>();
  var handleUnhandledException = (
    Object      exception,
    StackTrace? stack,
  ) {
    WidgetsBinding.instance.addPostFrameCallback((_) {
      if (navigatorKey.currentContext != null) {
        showDialog(
          context: navigatorKey.currentContext!,
          builder: (context) => AlertDialog(
            scrollable: true,
            title: const Text('Unhandled Exception'),
            content: SelectionArea(
              child: Text('${exception}\n${stack}'),
            ),
            actions: [
              TextButton(
                onPressed: () => Navigator.pop(context, 'OK'),
                child: const Text('OK'),
              ),
            ],
          ),
        );
      }
    });
    return;
  };
  FlutterError.onError = (details) {
    FlutterError.presentError(details);
    handleUnhandledException(details.exception, details.stack);
    return;
  };
  PlatformDispatcher.instance.onError = (error, stack) {
    handleUnhandledException(error, stack);
    return true;
  };
  var setting = SettingProvider.init();
  try {
    await setting.data.load();
  }
  catch (e, s) {
    handleUnhandledException(e, s);
  }
  try {
    var command = <String>[];
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      command = argument;
    }
    if (Platform.isAndroid || Platform.isIOS) {
      command = await PlatformMethod.getCommand(setting.data.mFallbackDirectory);
    }
    if (command.isNotEmpty) {
      if (command[0] == '-additional_argument') {
        setting.state.mAdditionalArgument = command.sublist(1);
      }
    }
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      await windowManager.ensureInitialized();
      if (!setting.data.mWindowPositionAlignToCenter) {
        await windowManager.setPosition(Offset(setting.data.mWindowPositionX.toDouble(), setting.data.mWindowPositionY.toDouble()));
      }
      if (!setting.data.mWindowSizeAdhereToDefault) {
        await windowManager.setSize(Size(setting.data.mWindowSizeWidth.toDouble(), setting.data.mWindowSizeHeight.toDouble()));
      }
      if (setting.data.mWindowPositionAlignToCenter) {
        await windowManager.center();
      }
      await windowManager.waitUntilReadyToShow();
      await windowManager.show();
    }
    await NotificationHelper.initialize();
    await setting.update();
  }
  catch (e, s) {
    handleUnhandledException(e, s);
  }
  runApp(Application(setting: setting, navigator: navigatorKey));
  return;
}
