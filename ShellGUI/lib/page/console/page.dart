// ignore_for_file: unused_import, unnecessary_cast, dead_code

import '/common.dart';
import 'dart:async';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:path/path.dart' as p_path;
import 'package:path_provider/path_provider.dart';
import '/setting.dart';
import '/command.dart';
import '/common/platform_method.dart';
import '/common/notification_helper.dart';
import '/common/path_picker.dart';
import '/page/console/output_bar.dart';
import '/page/console/action_bar.dart';
import '/page/console/launch_bar.dart';
import '/page/console/input_bar.dart';
import '/page/console/messsage_type.dart';
import '/bridge/host.dart';
import '/bridge/launcher.dart';

// ----------------

class ConsolePage extends StatefulWidget {

  const ConsolePage({
    super.key,
  });

  @override
  State<ConsolePage> createState() => _ConsolePageState();

  // ----------------

}

class _ConsolePageState extends State<ConsolePage> implements Host {

  final ScrollController _outputBarListScrollController = ScrollController();
  final List<Widget>     _outputBarListItem = [];
        Widget?          _inputBarContent;

  // ----------------

  Boolean _running = false;

  @override
  Future<Void>
  start(
  ) async {
    assert(!this._running);
    this._inputBarContent = const IdleInputBarContent();
    this.setState(() {});
    this._running = true;
    return;
  }

  @override
  Future<Void>
  finish(
  ) async {
    assert(this._running);
    this._inputBarContent = null;
    this.setState(() {});
    this._running = false;
    return;
  }
  
  @override
  Future<List<String>>
  execute(
    List<String> argument,
  ) async {
    assert(this._running);
    var result = List<String>.empty(growable: true);
    assert(argument.length >= 1);
    var method = argument[0];
    switch (method) {
      case 'version': {
        assert(argument.length == 1);
        var number = kApplicationVersion;
        result.add('${number}');
        break;
      }
      case 'host': {
        assert(argument.length == 1);
        var name = 'gui';
        result.add(name);
        break;
      }
      case 'system': {
        assert(argument.length == 1);
        var name = () {
          if (Platform.isWindows) {
            return 'windows';
          }
          if (Platform.isLinux) {
            return 'linux';
          }
          if (Platform.isMacOS) {
            return 'macintosh';
          }
          if (Platform.isAndroid) {
            return 'android';
          }
          if (Platform.isIOS) {
            return 'iphone';
          }
          throw Exception();
        }();
        result.add(name);
        break;
      }
      case 'architecture': {
        assert(argument.length == 1);
        var name = 'unknown';
        result.add(name);
        break;
      }
      case 'output_message': {
        assert(argument.length >= 3);
        var type = MessageType.fromString(argument[1]);
        var title = argument[2];
        var description = argument.sublist(3);
        if (this._outputBarListItem.isNotEmpty) {
          this._outputBarListItem.add(
            const SizedBox(height: 8),
          );
        }
        this._outputBarListItem.add(
          MessageOutputBar(
            type: type,
            title: title,
            description: description,
          ),
        );
        var shouldScrollToEnd = this._outputBarListScrollController.position.pixels == this._outputBarListScrollController.position.maxScrollExtent;
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        if (shouldScrollToEnd) {
          this._outputBarListScrollController.jumpTo(this._outputBarListScrollController.position.maxScrollExtent);
        }
        break;
      }
      case 'input_pause': {
        assert(argument.length == 1);
        var completer = Completer<dynamic>();
        this._inputBarContent = PauseInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        await completer.future;
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_confirmation': {
        assert(argument.length == 1);
        var completer = Completer<Boolean?>();
        this._inputBarContent = ConfirmationInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (!input ? 'n' : 'y'));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_number': {
        assert(argument.length == 1);
        var completer = Completer<Floater?>();
        this._inputBarContent = NumberInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input == 0.0 ? '0.0' : (input < 0.0 ? '-${-input}' : '+${input}')));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_integer': {
        assert(argument.length == 1);
        var completer = Completer<Integer?>();
        this._inputBarContent = IntegerInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input == 0 ? '0' : (input < 0 ? '-${-input}' : '+${input}')));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_size': {
        assert(argument.length == 1);
        var completer = Completer<String?>();
        this._inputBarContent = SizeInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input.toString()));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_string': {
        assert(argument.length == 1);
        var completer = Completer<String?>();
        this._inputBarContent = StringInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input.toString()));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_path': {
        assert(argument.length == 3);
        var type = FileObjectType.formString(argument[1]);
        var rule = FileRequestRule.formString(argument[2]);
        var completer = Completer<String?>();
        this._inputBarContent = PathInputBarContent(
          completer: completer,
          type: type,
          rule: rule,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input.toString()));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'input_option': {
        assert(argument.length >= 1);
        var option = argument.sublist(1);
        var completer = Completer<Integer?>();
        this._inputBarContent = OptionInputBarContent(
          completer: completer,
          option: option,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : ('${input}'));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await WidgetsBinding.instance.endOfFrame;
        break;
      }
      case 'pick_path': {
        assert(argument.length == 2);
        var type = FileObjectType.formString(argument[1]);
        var selection = null as String?;
        switch (type) {
          case FileObjectType.any: {
            throw Exception('path type should not be any');
            break;
          }
          case FileObjectType.file: {
            selection = await PathPicker.pickFile();
            break;
          }
          case FileObjectType.directory: {
            selection = await PathPicker.pickDirectory();
            break;
          }
        }
        result.add(selection == null ? '' : ('${selection}'));
        break;
      }
      case 'push_notification': {
        assert(argument.length == 3);
        var title = argument[1];
        var description = argument[2];
        await NotificationHelper.push(title, description);
        break;
      }
      default: {
        throw Exception('invalid method');
      }
    }
    return result;
  }
  
  // ----------------

  Future<Boolean>
  _launch(
    Command command,
  ) async {
    var exception = null as String?;
    var result = null as String?;
    var actualKernelPath = command.kernel;
    try {
      this._outputBarListItem.clear();
      if (Platform.isAndroid) {
        var directory = await getApplicationSupportDirectory();
        var originalKernelPath = actualKernelPath;
        actualKernelPath = p_path.join(directory.path, 'kernel');
        var originalKernelFile = File(originalKernelPath);
        if (!originalKernelFile.existsSync()) {
          throw Exception('kernel file not found');
        }
        var actualKernelFile = File(actualKernelPath);
        // NOTE : will crash if actualKernelFile already exist before copy
        if (actualKernelFile.existsSync()) {
          actualKernelFile.deleteSync(recursive: true);
        }
        originalKernelFile.copySync(actualKernelPath);
      }
      result = await Launcher.launch(this, actualKernelPath, command.script, command.argument);
    } catch (e) {
      exception = e.toString();
    }
    if (this._outputBarListItem.isNotEmpty) {
      this._outputBarListItem.add(
        const SizedBox(height: 8),
      );
    }
    if (exception == null) {
      this._outputBarListItem.add(
        MessageOutputBar(
          type: MessageType.success,
          title: 'SUCCEEDED',
          description:  [result!],
        ),
      );
    } else {
      this._outputBarListItem.add(
        MessageOutputBar(
          type: MessageType.error,
          title: 'FAILED',
          description:  [exception],
        ),
      );
    }
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    while (this._outputBarListScrollController.position.pixels != this._outputBarListScrollController.position.maxScrollExtent) {
      await this._outputBarListScrollController.animateTo(this._outputBarListScrollController.position.maxScrollExtent, duration: const Duration(milliseconds: 100), curve: Curves.easeOut);
    }
    return exception == null;
  }

  Future<Boolean>
  _launchDefault(
    List<String> additionalArgument,
  ) async {
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var command = Command(
      setting.data.mCommandKernel,
      setting.data.mCommandScript,
      setting.data.mCommandArgument + additionalArgument,
    );
    return this._launch(command);
  }

  // ----------------

  @override
  Widget build(BuildContext context) {
    var setting = Provider.of<SettingProvider>(context);
    var command = Provider.of<CommandProvider>(context);
    if (command.data != null) {
      () async {
        var commandData = command.data!;
        command.set(null);
        var state = await this._launch(commandData);
        if (setting.data.mBehaviorAfterCommandSucceed && state) {
          exitApplication();
        }
      }();
    }
    return Container(
      padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
      child: Column(
        children: [
          Expanded(
            child: Scrollbar(
              interactive: true,
              controller: this._outputBarListScrollController,
              child: ListView(
                padding: const EdgeInsets.fromLTRB(0, 8, 0, 8),
                controller: this._outputBarListScrollController,
                children: [...this._outputBarListItem],
              ),
            ),
          ),
          LinearProgressIndicator(
            minHeight: 1,
            value: this._running ? null : 0.0,
          ),
          const SizedBox(height: 8),
          ActionBar(
            content: this._running ? this._inputBarContent! : LaunchBarContent(onLaunch: this._launchDefault),
          ),
          const SizedBox(height: 8),
        ],
      ),
    );
  }
  
}
