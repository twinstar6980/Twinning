import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/platform_method.dart';
import '/utility/notification_helper.dart';
import '/utility/command_line_reader.dart';
import '/view/home/application.dart';
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
            title: const Text(
              'Unhandled Exception',
              overflow: TextOverflow.ellipsis,
            ),
            content: SelectionArea(
              child: Text(
                '${exception}\n${stack}',
                overflow: TextOverflow.clip,
              ),
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
    var optionWindowPosition = null as (Integer, Integer)?;
    var optionWindowSize = null as (Integer, Integer)?;
    var optionInitialTab = null as (String, ModuleType, List<String>)?;
    try {
      var optionView = <String>[];
      if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
        optionView = argument;
      }
      if (Platform.isAndroid || Platform.isIOS) {
        optionView = await PlatformMethod.getLinkCommand(setting.data.mFallbackDirectory);
      }
      var option = CommandLineReader(optionView);
      if (option.check('-window_position')) {
        optionWindowPosition = (
          option.nextInteger(),
          option.nextInteger(),
        );
      }
      else {
        if (setting.data.mWindowPositionState) {
          optionWindowPosition ??= (setting.data.mWindowPositionX, setting.data.mWindowPositionY);
        }
      }
      if (option.check('-window_size')) {
        optionWindowSize = (
          option.nextInteger(),
          option.nextInteger(),
        );
      }
      else {
        if (setting.data.mWindowSizeState) {
          optionWindowSize ??= (setting.data.mWindowSizeWidth, setting.data.mWindowSizeHeight);
        }
      }
      if (option.check('-initial_tab')) {
        optionInitialTab = (
          option.nextString(),
          ModuleType.values.byName(option.nextString()),
          option.nextStringList(),
        );
      }
      assertTest(option.done());
    }
    catch (e, s) {
      handleUnhandledException(e, s);
    }
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      await windowManager.ensureInitialized();
      if (optionWindowSize != null) {
        await windowManager.setSize(Size(optionWindowSize.$1.toDouble(), optionWindowSize.$2.toDouble()));
      }
      if (optionWindowPosition != null) {
        await windowManager.setPosition(Offset(optionWindowPosition.$1.toDouble(), optionWindowPosition.$2.toDouble()));
      }
      else {
        await windowManager.center();
      }
      await windowManager.setTitleBarStyle(TitleBarStyle.hidden);
      await windowManager.waitUntilReadyToShow();
      await windowManager.show();
    }
    setting.state.mInitialTab = optionInitialTab;
    await NotificationHelper.initialize();
    await setting.update();
  }
  catch (e, s) {
    handleUnhandledException(e, s);
  }
  runApp(Application(setting: setting, navigator: navigatorKey));
  return;
}
