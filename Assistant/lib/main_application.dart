import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/exception_helper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/window_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/custom_link_helper.dart';
import '/utility/system_ui_helper.dart';
import 'dart:async';
import 'widget/export.dart';
import 'package:collection/collection.dart';
import 'package:provider/provider.dart';
import 'package:flutter/widgets.dart';

// ----------------

class MainApplication {

  // #region utility

  static Future<Void> _handleException(
    Object      exception,
    StackTrace? stack,
  ) async {
    try {
      await postTask(() async {
        if (_setting.state.applicationNavigatorKey.currentContext != null) {
          await StyledModalDialogExtension.show<Void>(_setting.state.applicationNavigatorKey.currentContext!, StyledModalDialog.standard(
            title: 'Unhandled Exception',
            contentBuilder: (context, setStateForPanel) => [
              FlexContainer.horizontal([
                StyledText.custom(
                  ExceptionHelper.generateMessage(exception, stack),
                  overflow: .clip,
                ).withSelectableArea(
                ).withFlexExpanded(),
              ]),
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
    await _setting.state.homeInsertPage!(.new(
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
    var canContinue = (setting.data.forwarderImmediateJump && targetType != null) || (await StyledModalDialogExtension.show<Boolean>(_setting.state.applicationNavigatorKey.currentContext!, StyledModalDialog.standard(
      title: 'Forward',
      contentBuilder: (context, setStateForPanel) => [
        ...ModuleType.values.map((item) => StyledListTile.standardTight(
          enabled: forwardOption[item.index] != null,
          leading: StyledRadio.standard(
            enabled: forwardOption[item.index] != null,
            value: targetType == item,
            onChanged: (context) async {
              targetType = item;
              await refreshState(setStateForPanel);
            },
          ),
          content: StyledText.inherit(ModuleHelper.query(item).name),
        )),
      ],
      actionBuilder: (context) => [
        StyledButton.text(
          content: StyledText.inherit('Cancel'),
          onPressed: (context) => Navigator.pop(context, false),
        ),
        StyledButton.text(
          content: StyledText.inherit('Continue'),
          onPressed: (context) => Navigator.pop(context, true),
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
          commandItem = await StorageHelper.parseAndroidContentUri(_setting.state.applicationNavigatorKey.currentContext!, .parse(commandItem), true) ?? commandItem;
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
        option.nextString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, ModuleType.values)),
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
    if (link.scheme != ApplicationInformation.identifier || link.authority != '' || link.path != '/application') {
      throw Exception('invalid link');
    }
    var command = link.queryParametersAll['command'] ?? [];
    await _handleCommand(command);
    return;
  }

  // #endregion

  // #region life

  static final SettingProvider _setting = .new();

  // ----------------

  static Future<Void> run(
    List<String> argument,
  ) async {
    try {
      WidgetsFlutterBinding.ensureInitialized();
      ExceptionHelper.initialize();
      ExceptionHelper.listen((exception, stack) async {
        _handleException(exception, stack);
        return;
      });
      var needShowOnboarding = false;
      try {
        await _setting.load();
      }
      catch (e) {
        needShowOnboarding = true;
      }
      await _setting.save();
      _setting.state.handleLaunch = _handleLaunch;
      _setting.state.handleForward = _handleForward;
      _setting.state.handleCommand = _handleCommand;
      _setting.state.handleLink = _handleLink;
      await NotificationHelper.initialize();
      await CustomLinkHelper.initialize();
      await SystemUiHelper.applyMode(.edgeToEdge);
      if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
        await WindowHelper.ensureInitialized();
        if (_setting.data.windowSizeState) {
          await WindowHelper.setSize(_setting.data.windowSizeWidth, _setting.data.windowSizeHeight);
        }
        if (_setting.data.windowPositionState) {
          await WindowHelper.setPosition(_setting.data.windowPositionX, _setting.data.windowPositionY);
        }
        else {
          await WindowHelper.setAtCenter();
        }
        await WindowHelper.waitUntilReadyToShow();
        await WindowHelper.show();
      }
      postTask(() async {
        if (needShowOnboarding) {
          await _setting.state.homeShowOnboarding!();
        }
        await NotificationHelper.listen(() async {
          if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
            await WindowHelper.show();
          }
          return;
        });
        await CustomLinkHelper.listen((link) async {
          await _handleLink(link);
          return;
        });
        if (await CustomLinkHelper.getFirst() == null) {
          if (argument.length >= 1 && argument[0] == 'application') {
            await _handleCommand(argument.slice(1));
          }
          else {
            await _setting.state.homeShowLauncher!();
          }
        }
      });
    }
    catch (e, s) {
      _handleException(e, s);
    }
    runApp(Application(setting: _setting));
    return;
  }

  // #endregion

}
