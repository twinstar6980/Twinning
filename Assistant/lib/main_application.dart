import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/application.dart';
import '/utility/exception_helper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/window_helper.dart';
import '/utility/application_notification_manager.dart';
import '/utility/application_link_manager.dart';
import '/utility/system_ui_helper.dart';
import 'dart:async';
import 'widget/export.dart';
import 'package:collection/collection.dart';
import 'package:provider/provider.dart';
import 'package:flutter/widgets.dart';

// ----------------

class MainApplication {

  // #region singleton

  static final MainApplication instance = ._();

  // #endregion

  // #region constructor

  final SettingProvider _setting;

  // ----------------

  MainApplication._(
  ) :
    this._setting = .new();

  // #endregion

  // #region life

  Future<Void> run(
    List<String> argument,
  ) async {
    try {
      WidgetsFlutterBinding.ensureInitialized();
      ExceptionHelper.initialize();
      ExceptionHelper.listen((exception, stack) async {
        this._handleException(exception, stack);
        return;
      });
      var needShowOnboarding = false;
      try {
        await this._setting.load();
      }
      catch (e) {
        needShowOnboarding = true;
      }
      await this._setting.save();
      this._setting.state.handleLaunch = this._handleLaunch;
      this._setting.state.handleForward = this._handleForward;
      this._setting.state.handleCommand = this._handleCommand;
      this._setting.state.handleLink = this._handleLink;
      await ApplicationNotificationManager.instance.initialize();
      await ApplicationLinkManager.instance.initialize();
      await SystemUiHelper.applyMode(.edgeToEdge);
      if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
        await WindowHelper.ensureInitialized();
        if (this._setting.data.windowSizeState) {
          await WindowHelper.setSize(this._setting.data.windowSizeWidth, this._setting.data.windowSizeHeight);
        }
        if (this._setting.data.windowPositionState) {
          await WindowHelper.setPosition(this._setting.data.windowPositionX, this._setting.data.windowPositionY);
        }
        else {
          await WindowHelper.setAtCenter();
        }
        await WindowHelper.waitUntilReadyToShow();
        await WindowHelper.show();
      }
      postTask(() async {
        if (needShowOnboarding) {
          await this._setting.state.homeShowOnboarding!();
        }
        await ApplicationNotificationManager.instance.listen(() async {
          if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
            await WindowHelper.show();
          }
          return;
        });
        await ApplicationLinkManager.instance.listen((link) async {
          await this._handleLink(link);
          return;
        });
        if (await ApplicationLinkManager.instance.getFirst() == null) {
          if (argument.length >= 1 && argument[0] == 'application') {
            await this._handleCommand(argument.slice(1));
          }
          else {
            await this._setting.state.homeShowLauncher!();
          }
        }
      });
    }
    catch (e, s) {
      this._handleException(e, s);
    }
    runApp(Application(setting: this._setting));
    return;
  }

  // #endregion

  // #region utility

  Future<Void> _handleException(
    Object      exception,
    StackTrace? stack,
  ) async {
    try {
      await postTask(() async {
        if (this._setting.state.applicationNavigatorKey.currentContext != null) {
          await StyledModalDialogExtension.show<Void>(this._setting.state.applicationNavigatorKey.currentContext!, StyledModalDialog.standard(
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

  Future<Void> _handleLaunch(
    String       title,
    ModuleType   type,
    List<String> option,
  ) async {
    await this._setting.state.homeInsertPage!(.new(
      title: title,
      type: type,
      option: option,
    ));
    return;
  }

  Future<Void> _handleForward(
    List<String> resource,
  ) async {
    var setting = Provider.of<SettingProvider>(this._setting.state.applicationNavigatorKey.currentContext!, listen: false);
    var forwardOption = await ModuleType.values.map((value) async => await ModuleHelper.query(value).generateForwardOption(resource)).wait;
    var targetType = forwardOption[setting.data.forwarderDefaultTarget.index] != null ? setting.data.forwarderDefaultTarget : null;
    var canContinue = setting.data.forwarderImmediateJump && targetType != null;
    if (!canContinue) {
      canContinue = await StyledModalDialogExtension.show<Boolean>(this._setting.state.applicationNavigatorKey.currentContext!, StyledModalDialog.standard(
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
      )) ?? false;
    }
    if (canContinue && targetType != null) {
      await this._handleLaunch(ModuleHelper.query(targetType!).name, targetType!, forwardOption[targetType!.index]!);
    }
    return;
  }

  Future<Void> _handleCommand(
    List<String> command,
  ) async {
    if (SystemChecker.isAndroid) {
      var convertedCommand = <String>[];
      for (var commandItem in command) {
        if (commandItem.startsWith('content://')) {
          commandItem = await StorageHelper.parseAndroidContentUri(this._setting.state.applicationNavigatorKey.currentContext!, .parse(commandItem), true) ?? commandItem;
        }
        convertedCommand.add(commandItem);
      }
      command = convertedCommand;
    }
    var optionLaunch = null as ({String title, ModuleType type, List<String> option})?;
    var optionForward = null as ({List<String> resource})?;
    var option = CommandLineReader(command);
    if (option.check('-launch')) {
      optionLaunch = (
        title: option.nextString(),
        type: option.nextString().selfLet((it) => ConvertHelper.parseEnumerationFromStringOfSnakeCase(it, ModuleType.values)),
        option: option.nextStringList(),
      );
    }
    if (option.check('-forward')) {
      optionForward = (
        resource: option.nextStringList(),
      );
    }
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
    if (optionLaunch != null) {
      await this._handleLaunch(optionLaunch.title, optionLaunch.type, optionLaunch.option);
    }
    if (optionForward != null) {
      await this._handleForward(optionForward.resource);
    }
    return;
  }

  Future<Void> _handleLink(
    Uri link,
  ) async {
    if (link.scheme != ApplicationInformation.identifier || link.authority != '' || link.path != '/application') {
      throw Exception('invalid link');
    }
    var command = link.queryParametersAll['command'] ?? [];
    await this._handleCommand(command);
    return;
  }

  // #endregion

}
