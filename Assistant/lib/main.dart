import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/exception_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/control_helper.dart';
import '/utility/window_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/custom_link_helper.dart';
import '/view/home/common.dart';
import 'package:collection/collection.dart';
import 'package:provider/provider.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

// ----------------

Void main(
  List<String> argument,
) {
  _MainApplication.run(argument);
  return;
}

class _MainApplication {

  // #region utility

  static Future<Void> _handleException(
    Object      exception,
    StackTrace? stack,
  ) async {
    try {
      await ControlHelper.postTask(() async {
        if (_setting.state.applicationNavigatorKey.currentContext != null) {
          await ControlHelper.showDialogAsModal<Void>(_setting.state.applicationNavigatorKey.currentContext!, CustomModalDialog(
            title: 'Unhandled Exception',
            contentBuilder: (context, setStateForPanel) => [
              Row(
                children: [
                  Text(
                    ExceptionHelper.generateMessage(exception, stack),
                    overflow: TextOverflow.clip,
                  ).withSelectionArea(
                  ).withExpanded(),
                ],
              ),
            ],
            actionBuilder: null,
          ));
        }
      });
    }
    catch (e) {
      // ignored
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
        RadioGroup<ModuleType>(
          groupValue: targetType,
          onChanged: (value) async {
            targetType = value;
            await refreshState(setStateForPanel);
          },
          child: Column(
            children: ModuleType.values.map((item) => ListTile(
              contentPadding: EdgeInsets.zero,
              enabled: forwardOption[item.index] != null,
              leading: Radio(
                enabled: forwardOption[item.index] != null,
                value: item,
              ),
              title: Text(
                ModuleHelper.query(item).name,
                overflow: TextOverflow.ellipsis,
              ),
            )).toList(),
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

  // #endregion

  // #region life

  static final SettingProvider _setting = SettingProvider();

  // ----------------

  static Future<Void> run(
    List<String> argument,
  ) async {
    try {
      WidgetsFlutterBinding.ensureInitialized();
      ExceptionHelper.initialize((exception, stack) async {
        _handleException(exception, stack);
        return;
      });
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
        await WindowHelper.ensureInitialized();
        if (_setting.data.windowSizeState) {
          await WindowHelper.setSize(_setting.data.windowSizeWidth.toDouble(), _setting.data.windowSizeHeight.toDouble());
        }
        if (_setting.data.windowPositionState) {
          await WindowHelper.setPosition(_setting.data.windowPositionX.toDouble(), _setting.data.windowPositionY.toDouble());
        }
        else {
          await WindowHelper.setAtCenter();
        }
        await WindowHelper.waitUntilReadyToShow();
        await WindowHelper.show();
      }
      CustomLinkHelper.listen((link) async {
        ControlHelper.postTask(() async {
          await _handleLink(link);
        });
        return;
      });
      if (await CustomLinkHelper.getFirst() == null) {
        ControlHelper.postTask(() async {
          if (argument.length >= 1 && argument[0] == 'application') {
            await _handleCommand(argument.slice(1));
          }
          else {
            await _setting.state.homeShowLauncherPanel!();
          }
        });
      }
    }
    catch (e, s) {
      _handleException(e, s);
    }
    runApp(Application(setting: _setting));
    return;
  }

  // #endregion

}
