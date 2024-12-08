import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/command_line_reader.dart';
import '/utility/control_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import 'dart:io';
import 'package:collection/collection.dart';
import 'package:provider/provider.dart';
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
    if (_setting.state.mApplicationNavigatorKey.currentContext != null) {
      ControlHelper.showDialogAsModal<Void>(_setting.state.mApplicationNavigatorKey.currentContext!, CustomModalDialog(
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
    return;
  }

  // ----------------

  static Future<Void> _handleLaunch(
    String       title,
    ModuleType   type,
    List<String> option,
  ) async {
    await _setting.state.mHomeInsertTabItem!(ModuleLauncherConfiguration(
      title: title,
      type: type,
      option: option,
    ));
    return;
  }

  static Future<Void> _handleForward(
    List<String> item,
  ) async {
    var setting = Provider.of<SettingProvider>(_setting.state.mApplicationNavigatorKey.currentContext!, listen: false);
    var forwardState = await ModuleType.values.map((value) async => await ModuleHelper.query(value).checkForwardState?.call(item) ?? false).wait;
    var targetType = forwardState[setting.data.mForwarderDefaultTarget.index] ? setting.data.mForwarderDefaultTarget : null;
    var canContinue = setting.data.mForwarderImmediateForward ? true : await ControlHelper.showDialogAsModal<Boolean>(_setting.state.mApplicationNavigatorKey.currentContext!, CustomModalDialog(
      title: 'Forward',
      contentBuilder: (context, setState) => [
        ...ModuleType.values.map(
          (item) => ListTile(
            contentPadding: EdgeInsets.zero,
            enabled: forwardState[item.index],
            leading: Radio(
              value: item,
              groupValue: targetType,
              onChanged: !forwardState[item.index]
                ? null
                : (value) async {
                  targetType = item;
                  setState(() {});
                },
            ),
            title: Text(
              ModuleHelper.query(item).name,
              overflow: TextOverflow.ellipsis,
            ),
          ),
        ),
      ],
      actionBuilder: (context) => [
        TextButton(
          child: const Text('Cancel'),
          onPressed: () => Navigator.pop(context, false),
        ),
        TextButton(
          child: const Text('Continue'),
          onPressed: () => Navigator.pop(context, true),
        ),
      ],
    )) ?? false;
    if (canContinue && targetType != null) {
      await _setting.state.mHomeInsertTabItem!(ModuleLauncherConfiguration(
        title: ModuleHelper.query(targetType!).name,
        type: targetType!,
        option: ModuleHelper.query(targetType!).generateForwardOption!(item),
      ));
    }
    return;
  }

  static Future<Void> _handleCommand(
    List<String> command,
  ) async {
    if (Platform.isAndroid) {
      var convertedCommand = <String>[];
      for (var commandItem in command) {
        if (commandItem.startsWith('content://')) {
          commandItem = await StorageHelper.parseAndroidContentUri(_setting.state.mApplicationNavigatorKey.currentContext!, Uri.parse(commandItem), true) ?? commandItem;
        }
        convertedCommand.add(commandItem);
      }
      command = convertedCommand;
    }
    var optionLaunch = null as (String, ModuleType, List<String>)?;
    var optionForward = null as (List<String>,)?;
    var option = CommandLineReader(command);
    if (option.check('-launch')) {
      optionLaunch = (
        option.nextString(),
        option.nextString().selfLet((it) => ModuleType.values.byName(it)),
        option.nextStringList(),
      );
    }
    if (option.check('-forward')) {
      optionForward = (
        option.nextStringList(),
      );
    }
    assertTest(option.done());
    if (optionLaunch != null) {
      await _handleLaunch(optionLaunch.$1, optionLaunch.$2, optionLaunch.$3);
    }
    if (optionForward != null) {
      await _handleForward(optionForward.$1);
    }
    return;
  }

  static Future<Void> _handleLink(
    Uri link,
  ) async {
    if (link.scheme != 'twinstar.twinning.assistant' || link.hasAuthority || link.path != '/application') {
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
      _setting.state.mHandleLaunch = _handleLaunch;
      _setting.state.mHandleForward = _handleForward;
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
        if (argument.length >= 1 && argument[0] == 'application') {
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
      ControlHelper.postTask(() async {
        _handleException(e, s);
      });
    }
    runApp(Application(setting: _setting));
    return;
  }

}
