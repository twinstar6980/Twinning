import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/command_line_reader.dart';
import '/utility/control_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/storage_helper.dart';
import 'package:assistant/view/home/common.dart';
import 'dart:io';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:window_manager/window_manager.dart';
import 'package:app_links/app_links.dart';

// ----------------

Void main(
  List<String> argument,
) {
  _Main.run(argument);
  return;
}

class _Main {

  static SettingProvider _setting = SettingProvider();

  // ----------------

  static Void _handleException(
    Object      exception,
    StackTrace? stack,
  ) {
    ControlHelper.postTask(() async {
      if (_setting.state.mApplicationNavigatorKey.currentContext != null) {
        ControlHelper.showCustomModalDialog<Void>(_setting.state.mApplicationNavigatorKey.currentContext!, CustomModalDialog(
          title: 'Unhandled Exception',
          contentBuilder: (context, setState) => [
            Row(
              children: [
                Expanded(
                  child: SelectionArea(
                    child: Text(
                      '${exception}\n${stack}',
                      overflow: TextOverflow.clip,
                    ),
                  ),
                ),
              ],
            ),
          ],
          actionBuilder: null,
        ));
      }
    });
    return;
  }

  // ----------------

  static Future<Void> _handleCommand(
    List<String> command,
  ) async {
    if (Platform.isAndroid) {
      var convertedCommand = List<String>.empty(growable: true);
      for (var commandItem in command) {
        if (commandItem.startsWith('content://')) {
          commandItem = await StorageHelper.parseAndroidContentUri(_setting.state.mApplicationNavigatorKey.currentContext!, Uri.parse(commandItem), true) ?? commandItem;
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
      await _setting.state.mHomeInsertTabItem!(ModuleLauncherConfiguration(
        title: optionInsertTab.$1,
        type: optionInsertTab.$2,
        option: optionInsertTab.$3,
      ));
    }
    return;
  }

  static Future<Void> _handleLink(
    Uri link,
  ) async {
    if (link.scheme != 'twinstar.twinning.assistant' || link.hasAuthority || link.path != '/launch') {
      throw Exception('invalid link');
    }
    var command = link.queryParametersAll['command'] ?? [];
    await _handleCommand(command);
    return;
  }

  // ----------------

  static Future<Void> run(
    List<String> argument,
  ) async {
    try {
      WidgetsFlutterBinding.ensureInitialized();
      WidgetsBinding.instance.platformDispatcher.onError = (error, stack) {
        _handleException(error, stack);
        return true;
      };
      FlutterError.onError = (details) {
        FlutterError.presentError(details);
        _handleException(details.exception, details.stack);
        return;
      };
      try {
        await _setting.load();
      }
      catch (e) {
        await _setting.reset();
      }
      await _setting.save();
      _setting.state.mHandleCommand = _handleCommand;
      await NotificationHelper.initialize();
      await SystemChrome.setEnabledSystemUIMode(SystemUiMode.edgeToEdge);
      if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
        await windowManager.ensureInitialized();
        await windowManager.setTitleBarStyle(TitleBarStyle.hidden);
        if (_setting.data.mWindowSizeState) {
          await windowManager.setSize(Size(_setting.data.mWindowSizeWidth.toDouble(), _setting.data.mWindowSizeHeight.toDouble()));
        }
        if (_setting.data.mWindowPositionState) {
          await windowManager.setPosition(Offset(_setting.data.mWindowPositionX.toDouble(), _setting.data.mWindowPositionY.toDouble()));
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
            _handleCommand(argument.slice(1));
          });
        }
        else {
          ControlHelper.postTask(() async {
            _setting.state.mHomeShowLauncherPanel!();
          });
        }
      }
      AppLinks().stringLinkStream.listen((link) async {
        ControlHelper.postTask(() async {
          if (link.startsWith('twinstar.twinning.assistant:')) {
            _handleLink(Uri.parse(link));
          }
        });
      });
    }
    catch (e, s) {
      _handleException(e, s);
    }
    runApp(Application(setting: _setting));
    return;
  }

}
