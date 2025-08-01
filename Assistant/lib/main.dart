import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/command_line_reader.dart';
import '/utility/control_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
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
    if (_setting.state.applicationNavigatorKey.currentContext != null) {
      ControlHelper.showDialogAsModal<Void>(_setting.state.applicationNavigatorKey.currentContext!, CustomModalDialog(
        title: 'Unhandled Exception',
        contentBuilder: (context, setStateForPanel) => [
          Row(
            children: [
              Text(
                '${exception}\n${stack}',
                overflow: TextOverflow.clip,
              ).withSelectionArea(
              ).withExpanded(),
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
    await _setting.state.homeInsertTabItem!(ModuleLauncherConfiguration(
      title: title,
      type: type,
      option: option,
    ));
    return;
  }

  static Future<Void> _handleForward(
    List<String> resource,
  ) async {
    var setting = Provider.of<SettingProvider>(_setting.state.applicationNavigatorKey.currentContext!, listen: false);
    var forwardOption = await ModuleType.values.map((value) async => await ModuleHelper.query(value).generateForwardOption(resource)).wait;
    var targetType = forwardOption[setting.data.forwarderDefaultTarget.index] != null ? setting.data.forwarderDefaultTarget : null;
    var canContinue = (setting.data.forwarderImmediateJump && targetType != null) || (await ControlHelper.showDialogAsModal<Boolean>(_setting.state.applicationNavigatorKey.currentContext!, CustomModalDialog(
      title: 'Forward',
      contentBuilder: (context, setStateForPanel) => [
        ...ModuleType.values.map(
          (item) => ListTile(
            contentPadding: EdgeInsets.zero,
            enabled: forwardOption[item.index] != null,
            leading: Radio(
              value: item,
              groupValue: targetType,
              onChanged: forwardOption[item.index] == null
                ? null
                : (value) async {
                  targetType = item;
                  await refreshState(setStateForPanel);
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
          child: Text('Cancel'),
          onPressed: () => Navigator.pop(context, false),
        ),
        TextButton(
          child: Text('Continue'),
          onPressed: () => Navigator.pop(context, true),
        ),
      ],
    )) ?? false);
    if (canContinue && targetType != null) {
      await _handleLaunch(ModuleHelper.query(targetType!).name, targetType!, forwardOption[targetType!.index]!);
    }
    return;
  }

  static Future<Void> _handleCommand(
    List<String> command,
  ) async {
    if (SystemChecker.isAndroid) {
      var convertedCommand = <String>[];
      for (var commandItem in command) {
        if (commandItem.startsWith('content://')) {
          commandItem = await StorageHelper.parseAndroidContentUri(_setting.state.applicationNavigatorKey.currentContext!, Uri.parse(commandItem), true) ?? commandItem;
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
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
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
    if (link.scheme != 'twinstar.twinning.assistant' || link.authority != '' || link.path != '/application') {
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
      _setting.state.handleLaunch = _handleLaunch;
      _setting.state.handleForward = _handleForward;
      _setting.state.handleCommand = _handleCommand;
      _setting.state.handleLink = _handleLink;
      await NotificationHelper.initialize();
      await SystemChrome.setEnabledSystemUIMode(SystemUiMode.edgeToEdge);
      if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
        await windowManager.ensureInitialized();
        if (_setting.data.windowSizeState) {
          await windowManager.setSize(Size(_setting.data.windowSizeWidth.toDouble(), _setting.data.windowSizeHeight.toDouble()));
        }
        if (_setting.data.windowPositionState) {
          await windowManager.setPosition(Offset(_setting.data.windowPositionX.toDouble(), _setting.data.windowPositionY.toDouble()));
        }
        else {
          await windowManager.center();
        }
        await windowManager.waitUntilReadyToShow();
        await windowManager.show();
      }
      if (!(await AppLinks().getInitialLinkString() ?? '').startsWith('twinstar.twinning.assistant:')) {
        ControlHelper.postTask(() async {
          if (argument.length >= 1 && argument[0] == 'application') {
            await _handleCommand(argument.slice(1));
          }
          else {
            await _setting.state.homeShowLauncherPanel!();
          }
        });
      }
      AppLinks().stringLinkStream.listen((link) async {
        ControlHelper.postTask(() async {
          if (link.startsWith('twinstar.twinning.assistant:')) {
            await _handleLink(Uri.parse(link));
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
