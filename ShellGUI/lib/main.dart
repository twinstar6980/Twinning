import '/common.dart';
import '/common/platform_method.dart';
import '/common/custom_font_helper.dart';
import '/common/notification_helper.dart';
import '/common/path_picker.dart';
import '/setting.dart';
import '/command.dart';
import '/application.dart';
import 'dart:io';
import 'package:flutter/widgets.dart';
import 'package:window_manager/window_manager.dart';

// ----------------

Future<Void>
main(
  List<String> argument,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  var setting = Setting.init();
  try {
    setting = await Setting.load();
  } catch (e) {
    await Setting.save(setting);
  }
  var commandSource = <String>[];
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    commandSource = argument;
  }
  if (Platform.isAndroid) {
    commandSource = await PlatformMethod.getCommand(setting.mFallbackDirectoryForInvisibleFile);
  }
  var command = null as Command?;
  if (commandSource.isNotEmpty) {
    command = Command('', '', []);
    if (commandSource[0] == '-additional') {
      command.kernel = setting.mCommandKernel;
      command.script = setting.mCommandScript;
      command.argument = setting.mCommandArgument + commandSource.sublist(1);
    } else {
      command.kernel = commandSource.elementAt(0);
      command.script = commandSource.elementAt(1);
      command.argument = commandSource.sublist(2);
    }
  }
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    await WindowManager.instance.ensureInitialized();
    if (!setting.mWindowPositionAlignToCenter) {
      await windowManager.setPosition(Offset(setting.mWindowPositionX.toDouble(), setting.mWindowPositionY.toDouble()));
    }
    if (!setting.mWindowSizeAdhereToDefault) {
      await windowManager.setSize(Size(setting.mWindowSizeWidth.toDouble(), setting.mWindowSizeHeight.toDouble()));
    }
    if (setting.mWindowPositionAlignToCenter) {
      await windowManager.center();
    }
    await windowManager.waitUntilReadyToShow();
    await windowManager.show();
  }
  await NotificationHelper.initialize();
  for (var index = 0; index < setting.mPrimaryFont.length; ++index) {
    var family = await CustomFontHelper.loadFile(setting.mPrimaryFont[index]);
    if (family != null && !gPrimaryFontFamliy.contains(family)) {
      gPrimaryFontFamliy.add(family);
    }
  }
  for (var index = 0; index < setting.mConsoleFont.length; ++index) {
    var family = await CustomFontHelper.loadFile(setting.mConsoleFont[index]);
    if (family != null && !gConsoleFontFamliy.contains(family)) {
      gConsoleFontFamliy.add(family);
    }
  }
  PathPicker.fallbackDirectory = setting.mFallbackDirectoryForInvisibleFile;
  runApp(Application(setting: setting, command: command));
  return;
}
