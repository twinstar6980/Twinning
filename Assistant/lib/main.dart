import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/command_line_reader.dart';
import '/utility/control_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/storage_helper.dart';
import 'dart:io';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:app_links/app_links.dart';

// ----------------

Future<Void> main(
  List<String> argument,
) async {
  var setting = SettingProvider();
  var handleException = (
    Object      exception,
    StackTrace? stack,
  ) {
    ControlHelper.postTask(() async {
      if (setting.state.mApplicationNavigatorKey.currentContext != null) {
        ControlHelper.showCustomModalDialog(
          context: setting.state.mApplicationNavigatorKey.currentContext!,
          title: 'Unhandled Exception',
          contentBuilder: (context, setState) => [
            SelectionArea(
              child: Text(
                '${exception}\n${stack}',
                overflow: TextOverflow.clip,
              ),
            ),
          ],
          actionBuilder: null,
        );
      }
    });
    return;
  };
  var handleCommand = (
    List<String> command,
  ) async {
    if (Platform.isAndroid) {
      var convertedCommand = List<String>.empty(growable: true);
      for (var commandItem in command) {
        if (commandItem.startsWith('content://')) {
          commandItem = await StorageHelper.parseAndroidContentUri(setting.state.mApplicationNavigatorKey.currentContext!, Uri.parse(commandItem), true) ?? commandItem;
        }
        convertedCommand.add(commandItem);
      }
      command = convertedCommand;
    }
    var optionInsertTab = null as (String, ModuleType, List<String>)?;
    var option = CommandLineReader(command);
    if (option.check('-insert_tab')) {
      optionInsertTab = (
        option.nextString(),
        option.nextString().selfLet((it) => ModuleType.values.byName(it)),
        option.nextStringList(),
      );
    }
    assertTest(option.done());
    if (optionInsertTab != null) {
      await setting.state.mHomeInsertTabItem!(ModuleLauncherConfiguration(
        title: optionInsertTab.$1,
        type: optionInsertTab.$2,
        option: optionInsertTab.$3,
      ));
    }
    return;
  };
  var handleLink = (
    Uri link,
  ) async {
    if (link.scheme != 'twinstar.twinning.assistant' || link.hasAuthority || link.path != '/launch') {
      throw Exception('invalid link');
    }
    var command = link.queryParametersAll['command'] ?? [];
    await handleCommand(command);
    return;
  };
  WidgetsFlutterBinding.ensureInitialized();
  WidgetsBinding.instance.platformDispatcher.onError = (error, stack) {
    handleException(error, stack);
    return true;
  };
  FlutterError.onError = (details) {
    FlutterError.presentError(details);
    handleException(details.exception, details.stack);
    return;
  };
  try {
    try {
      await setting.load();
    }
    catch (e) {
      await setting.reset();
    }
    await setting.save();
    await NotificationHelper.initialize();
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      await windowManager.ensureInitialized();
      await windowManager.setTitleBarStyle(TitleBarStyle.hidden);
      if (setting.data.mWindowSizeState) {
        await windowManager.setSize(Size(setting.data.mWindowSizeWidth.toDouble(), setting.data.mWindowSizeHeight.toDouble()));
      }
      if (setting.data.mWindowPositionState) {
        await windowManager.setPosition(Offset(setting.data.mWindowPositionX.toDouble(), setting.data.mWindowPositionY.toDouble()));
      }
      else {
        await windowManager.center();
      }
      await windowManager.waitUntilReadyToShow();
      await windowManager.show();
    }
    if (!(await AppLinks().getInitialLinkString() ?? '').startsWith('twinstar.twinning.assistant:')) {
      if (argument.length >= 1 && argument[0] == 'launch') {
        ControlHelper.postTask(() async {
          handleCommand(argument.slice(1));
        });
      }
      else {
        ControlHelper.postTask(() async {
          setting.state.mHomeShowLauncherPanel!();
        });
      }
    }
    AppLinks().stringLinkStream.listen((link) async {
      ControlHelper.postTask(() async {
        if (link.startsWith('twinstar.twinning.assistant:')) {
          handleLink(Uri.parse(link));
        }
      });
    });
  }
  catch (e, s) {
    handleException(e, s);
  }
  runApp(Application(setting: setting));
  return;
}
