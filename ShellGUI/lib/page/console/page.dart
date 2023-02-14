// ignore_for_file: unused_import

import '/common.dart';
import 'dart:async';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:path_provider/path_provider.dart';
import '/setting.dart';
import '/command.dart';
import '/page/console/output_bar.dart';
import '/page/console/action_bar.dart';
import '/page/console/launch_bar.dart';
import '/page/console/input_bar.dart';
import '/page/console/messsage_type.dart';
import '/host/host.dart';
import '/host/launcher.dart';

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
  Future<void>
  start(
  ) async {
    assert(!this._running);
    this._outputBarListItem.clear();
    this._inputBarContent = const IdleInputBarContent();
    this.setState(() {});
    this._running = true;
    return;
  }

  @override
  Future<void>
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
    switch (argument[0]) {
      case 'name': {
        result.add('gui');
        break;
      }
      case 'version': {
        result.add('${kApplicationVersion}');
        break;
      }
      case 'system': {
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
      case 'output_notify': {
        var type = MessageTypeExtension.fromString(argument[1]);
        var title = argument[2];
        var description = argument.sublist(3);
        this._outputBarListItem.add(
          const SizedBox(height: 8),
        );
        this._outputBarListItem.add(
          NotifyOutputBar(
            type: type,
            title: title,
            description: description,
          ),
        );
        this.setState(() {});
        Future.delayed(const Duration(milliseconds: 100), () {
          this._outputBarListScrollController.jumpTo(this._outputBarListScrollController.position.maxScrollExtent);
        });
        break;
      }
      case 'input_pause': {
        var completer = Completer<dynamic>();
        this._inputBarContent = PauseInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        await completer.future;
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      case 'input_confirm': {
        var completer = Completer<Boolean?>();
        this._inputBarContent = ConfirmInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(
          input == null ? '' : (!input ? 'n' : 'y'),
        );
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      case 'input_number': {
        var completer = Completer<Floating?>();
        this._inputBarContent = NumberInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input == 0.0 ? '0.0' : (input < 0.0 ? '-${-input}' : '+${input}')));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      case 'input_integer': {
        var completer = Completer<Integer?>();
        this._inputBarContent = IntegerInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input == 0 ? '0' : (input < 0 ? '-${-input}' : '+${input}')));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      case 'input_size': {
        var completer = Completer<String?>();
        this._inputBarContent = SizeInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input.toString()));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      case 'input_string': {
        var completer = Completer<String?>();
        this._inputBarContent = StringInputBarContent(
          completer: completer,
        );
        this.setState(() {});
        var input = await completer.future;
        result.add(input == null ? '' : (input.toString()));
        this._inputBarContent = const IdleInputBarContent();
        this.setState(() {});
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      case 'input_option': {
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
        await Future.delayed(const Duration(milliseconds: 100));
        break;
      }
      default: {
        throw Exception('invalid method');
      }
    }
    return result;
  }
  
  // ----------------

  Future<String?>
  _launch(
    List<String> additionalArgument,
  ) async {
    late String? result;
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var actualCorePath = setting.data.mCore;
    try {
      if (Platform.isAndroid) {
        var directory = await getApplicationSupportDirectory();
        var originalCorePath = actualCorePath;
        actualCorePath = '${directory.path}/core';
        var originalCoreFile = File(originalCorePath);
        if (!originalCoreFile.existsSync()) {
          throw Exception('core file not found');
        }
        var actualCoreFile = File(actualCorePath);
        // NOTE : will crash if actualCoreFile already exist before copy
        if (actualCoreFile.existsSync()) {
          actualCoreFile.deleteSync(recursive: true);
        }
        originalCoreFile.copySync(actualCorePath);
      }
      result = await Launcher.launch(this, actualCorePath, setting.data.mScript, [...setting.data.mArgument, ...additionalArgument]);
    } catch (e) {
      result = '${e}';
    }
    if (result != null) {
      this._outputBarListItem.add(
        const SizedBox(height: 8),
      );
      this._outputBarListItem.add(
        NotifyOutputBar(
          type: MessageType.error,
          title: result,
          description: const [],
        ),
      );
      await Future.delayed(const Duration(milliseconds: 100));
      this._outputBarListScrollController.jumpTo(this._outputBarListScrollController.position.maxScrollExtent);
      this.setState(() {});
    }
    return result;
  }

  // ----------------

  @override
  Widget build(BuildContext context) {
    var setting = Provider.of<SettingProvider>(context);
    var command = Provider.of<CommandProvider>(context);
    if (setting.loaded && command.additionalArgument != null) {
      () async {
        var additionalArgument = command.additionalArgument!;
        command.set(null);
        var result = await this._launch(additionalArgument);
        if (setting.data.mExitAfterCommandSucceed && result == null) {
          exitApp();
        }
      }();
    }
    return Container(
      padding: const EdgeInsets.fromLTRB(8, 0, 8, 0),
      child: Column(
        children: [
          Expanded(
            child: ListView(
              controller: this._outputBarListScrollController,
              children: [...this._outputBarListItem],
            ),
          ),
          const SizedBox(height: 8),
          LinearProgressIndicator(
            minHeight: 1,
            value: this._running ? null : 0.0,
          ),
          const SizedBox(height: 8),
          ActionBar(
            content: this._running ? this._inputBarContent! : LaunchBarContent(onLaunch: this._launch),
          ),
          const SizedBox(height: 8),
        ],
      ),
    );
  }
  
}
