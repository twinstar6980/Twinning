import '/common.dart';
import '/utility/notification_helper.dart';
import '/utility/path_picker.dart';
import '/setting.dart';
import '/bridge/host.dart';
import '/bridge/launcher.dart';
import '/page/console/action_bar.dart';
import '/page/console/messsage_type.dart';
import '/page/console/output_bar.dart';
import '/page/console/input_bar.dart';
import '/page/console/launch_bar.dart';
import 'dart:async';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:path_provider/path_provider.dart';

// ----------------

class ConsolePage extends StatefulWidget {

  const ConsolePage({
    super.key,
  });

  @override
  createState() => _ConsolePageState();

  // ----------------

}

class _ConsolePageState extends State<ConsolePage> implements Host {

  List<String>     _additionalArgument = [];
  ScrollController _outputBarListScrollController = ScrollController();
  List<Widget>     _outputBarListItem = [];
  Widget?          _inputBarContent = null;

  // ----------------

  Boolean _running = false;

  @override
  start() async {
    assertAlways(!this._running);
    this._inputBarContent = const IdleInputBarContent();
    this.setState(() {});
    this._running = true;
    return;
  }

  @override
  finish() async {
    assertAlways(this._running);
    this._inputBarContent = null;
    this.setState(() {});
    this._running = false;
    return;
  }

  @override
  execute(argument) async {
    assertAlways(this._running);
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var result = <String>[];
    assertAlways(argument.length >= 1);
    var method = argument[0];
    switch (method) {
      case 'version': {
        assertAlways(argument.length == 1);
        var number = kApplicationVersion;
        result.add('${number}');
        break;
      }
      case 'host': {
        assertAlways(argument.length == 1);
        var name = 'gui';
        result.add(name);
        break;
      }
      case 'system': {
        assertAlways(argument.length == 1);
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
      case 'output_message': {
        assertAlways(argument.length >= 3);
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
        assertAlways(argument.length == 1);
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
      case 'input_boolean': {
        assertAlways(argument.length == 1);
        var completer = Completer<Boolean?>();
        this._inputBarContent = BooleanInputBarContent(
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
      case 'input_integer': {
        assertAlways(argument.length == 1);
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
      case 'input_floater': {
        assertAlways(argument.length == 1);
        var completer = Completer<Floater?>();
        this._inputBarContent = FloaterInputBarContent(
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
      case 'input_size': {
        assertAlways(argument.length == 1);
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
        assertAlways(argument.length == 1);
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
        assertAlways(argument.length == 1);
        var completer = Completer<String?>();
        this._inputBarContent = PathInputBarContent(
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
      case 'input_enumeration': {
        assertAlways(argument.length >= 1);
        var option = argument.sublist(1);
        var completer = Completer<Integer?>();
        this._inputBarContent = EnumerationInputBarContent(
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
        assertAlways(argument.length == 2);
        var type = FileObjectType.formString(argument[1]);
        var selection = null as String?;
        switch (type) {
          case FileObjectType.any: {
            throw Exception('path type should not be any');
            break; // ignore: dead_code
          }
          case FileObjectType.file: {
            selection = await PathPicker.pickFile(setting.data.mFallbackDirectory);
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
        assertAlways(argument.length == 3);
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

  Future<Boolean> _launch(
  ) async {
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var result = null as String?;
    var exception = null as String?;
    var temporaryKernel = null as String?;
    try {
      this._outputBarListItem.clear();
      if (Platform.isAndroid) {
        var temporaryDirectory = normalizePath((await getApplicationCacheDirectory()).path);
        var temporaryIndex = 0;
        do {
          temporaryIndex += 1;
          temporaryKernel = '${temporaryDirectory}/kernel.${temporaryIndex}';
        }
        while (await File(temporaryKernel).exists());
        await File(setting.data.mConsoleKernel).copy(temporaryKernel);
      }
      result = await Launcher.launch(this, temporaryKernel ?? setting.data.mConsoleKernel, setting.data.mConsoleScript, setting.data.mConsoleArgument + this._additionalArgument);
    }
    catch (e) {
      exception = e.toString();
    }
    if (temporaryKernel != null) {
      var temporaryKernelFile = File(temporaryKernel);
      if (await temporaryKernelFile.exists()) {
        await temporaryKernelFile.delete();
      }
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
          description: [result!],
        ),
      );
    }
    else {
      this._outputBarListItem.add(
        MessageOutputBar(
          type: MessageType.error,
          title: 'FAILED',
          description: [exception],
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

  // ----------------

  @override
  dispose() {
    this._outputBarListScrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    if (setting.state.mAdditionalArgument != null) {
      () async {
        this._additionalArgument.addAll(setting.state.mAdditionalArgument!);
        setting.state.mAdditionalArgument = null;
        setting.update();
        await this._launch();
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
            content: this._running
              ? this._inputBarContent!
              : LaunchBarContent(
                additionalArgument: this._additionalArgument,
                onLaunch: this._launch,
              ),
          ),
          const SizedBox(height: 8),
        ],
      ),
    );
  }

}
