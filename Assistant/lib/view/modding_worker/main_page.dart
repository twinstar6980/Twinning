import '/common.dart';
import '/setting.dart';
import '/utility/control_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/command_line_reader.dart';
import '/bridge/client.dart';
import '/bridge/launcher.dart';
import '/view/home/common.dart';
import '/view/modding_worker/message_type.dart';
import '/view/modding_worker/message_card.dart';
import '/view/modding_worker/submission_type.dart';
import '/view/modding_worker/submission_bar.dart';
import '/view/modding_worker/launcher_bar.dart';
import 'dart:async';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ignore_for_file: invalid_use_of_protected_member

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.option,
  });

  @override
  createState() => _MainPageState();

  // ----------------

  final List<String> option;

}

class _MainPageState extends State<MainPage> {

  late List<String>          _additionalArgument;
  late ScrollController      _messageListScrollController;
  late List<Widget>          _messageList;
  late SubmissionBar         _submissionBar;
  late Widget                _launcherBar;
  late _MainPageBridgeClient _sessionClient;

  Future<Boolean> _launchSession(
  ) async {
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var result = null as List<String>?;
    var exception = null as String?;
    var temporaryKernel = null as String?;
    try {
      this._messageList.clear();
      var temporaryKernel = await StorageHelper.temporary();
      await StorageHelper.copyFile(setting.data.mModdingWorker.mKernel, temporaryKernel);
      result = await Launcher.launch(this._sessionClient, temporaryKernel, setting.data.mModdingWorker.mScript, setting.data.mModdingWorker.mArgument + this._additionalArgument);
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
    if (this._messageList.isNotEmpty) {
      this._messageList.add(
        const SizedBox(height: 8),
      );
    }
    if (exception == null) {
      this._messageList.add(
        MessageCard(
          type: MessageType.success,
          title: 'SUCCEEDED',
          description: result!,
        ),
      );
    }
    else {
      this._messageList.add(
        MessageCard(
          type: MessageType.error,
          title: 'FAILED',
          description: [exception],
        ),
      );
    }
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    while (this._messageListScrollController.position.pixels != this._messageListScrollController.position.maxScrollExtent) {
      await this._messageListScrollController.animateTo(this._messageListScrollController.position.maxScrollExtent, duration: const Duration(milliseconds: 100), curve: Curves.easeOut);
    }
    return exception == null;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._additionalArgument = [];
    this._messageListScrollController = ScrollController();
    this._messageList = [];
    this._submissionBar = const SubmissionBar(
      type: null,
      option: [],
      completer: null,
    );
    this._launcherBar = LauncherBar(
      additionalArgument: this._additionalArgument,
      onLaunch: this._launchSession,
    );
    this._sessionClient = _MainPageBridgeClient(this);
    var optionImmediateLaunch = null as Boolean?;
    var optionAdditionalArgument = null as List<String>?;
    try {
      var option = CommandLineReader(this.widget.option);
      if (option.check('-immediate_launch')) {
        optionImmediateLaunch = option.nextBoolean();
      }
      else {
        optionImmediateLaunch = setting.data.mModdingWorker.mImmediateLaunch;
      }
      if (option.check('-additional_argument')) {
        optionAdditionalArgument = option.nextStringList();
      }
      assertTest(option.done());
    }
    catch (e) {
      ControlHelper.runAfterNextFrame(() => throw e); // ignore: use_rethrow_when_possible
    }
    if (optionAdditionalArgument != null) {
      this._additionalArgument.addAll(optionAdditionalArgument);
    }
    if (optionImmediateLaunch != null) {
      if (optionImmediateLaunch) {
        ControlHelper.runAfterNextFrame(() async {
          await this._launchSession();
        });
      }
    }
    return;
  }

  @override
  dispose() {
    this._messageListScrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var theme = Theme.of(context);
    return CustomModulePage(
      content: Column(
        children: [
          Expanded(
            child: SelectionArea(
              child: Scrollbar(
                interactive: true,
                controller: this._messageListScrollController,
                child: ListView(
                  padding: const EdgeInsets.fromLTRB(8, 8, 8, 8),
                  controller: this._messageListScrollController,
                  children: [...this._messageList],
                ),
              ),
            ),
          ),
          LinearProgressIndicator(
            borderRadius: const BorderRadius.all(Radius.circular(2)),
            value: !this._sessionClient._running ? 0 : this._submissionBar.type == null ? null : 1,
            color: !this._sessionClient._running ? null : this._submissionBar.type == null ? null : theme.colorScheme.tertiary,
          ),
        ],
      ),
      bottom: this._sessionClient._running
        ? this._submissionBar
        : this._launcherBar,
    );
  }

}

class _MainPageBridgeClient implements Client {

  // #region structor

  late _MainPageState _controller;

  late Boolean _running;

  // ----------------

  _MainPageBridgeClient(
    _MainPageState controller,
  ) {
    this._controller = controller;
    this._running = false;
  }

  // #endregion

  // #region life

  @override
  start() async {
    assertTest(!this._running);
    this._running = true;
    this._controller.setState(() {});
    return;
  }

  @override
  finish() async {
    assertTest(this._running);
    this._running = false;
    this._controller.setState(() {});
    return;
  }

  // ----------------

  @override
  callback(argument) async {
    assertTest(this._running);
    var result = <String>[];
    assertTest(argument.length >= 1);
    switch (argument[0]) {
      case 'name': {
        assertTest(argument.length == 1);
        var detail = await this.callbackName();
        var detailName = detail.$1;
        result.add(detailName);
        break;
      }
      case 'version': {
        assertTest(argument.length == 1);
        var detail = await this.callbackVersion();
        var detailVersion = detail.$1;
        result.add(detailVersion);
        break;
      }
      case 'send_message': {
        assertTest(argument.length >= 3);
        var detailType = argument[1];
        var detailTitle = argument[2];
        var detailDescription = argument.sublist(3);
        var detail = await this.callbackSendMessage(detailType, detailTitle, detailDescription); // ignore: unused_local_variable
        break;
      }
      case 'receive_submission': {
        assertTest(argument.length >= 2);
        var detailType = argument[1];
        var detailOption = argument.sublist(2);
        var detail = await this.callbackReceiveSubmission(detailType, detailOption);
        var detailValue = detail.$1;
        result.add(detailValue);
        break;
      }
      case 'pick_path': {
        assertTest(argument.length == 2);
        var detailType = argument[1];
        var detail = await this.callbackPickPath(detailType);
        var detailTarget = detail.$1;
        result.add(detailTarget);
        break;
      }
      case 'push_notification': {
        assertTest(argument.length == 3);
        var detailTitle = argument[1];
        var detailDescription = argument[2];
        var detail = await this.callbackPushNotification(detailTitle, detailDescription); // ignore: unused_local_variable
        break;
      }
      default: {
        throw Exception('invalid method');
      }
    }
    return result;
  }

  Future<(String,)> callbackName(
  ) async {
    var name = 'assistant';
    return (name,);
  }

  Future<(String,)> callbackVersion(
  ) async {
    var version = kApplicationVersion.toString();
    return (version,);
  }

  Future<()> callbackSendMessage(
    String       type,
    String       title,
    List<String> description,
  ) async {
    var typeValue = switch (type) {
      'verbosity'   => MessageType.verbosity,
      'information' => MessageType.information,
      'warning'     => MessageType.warning,
      'error'       => MessageType.error,
      'success'     => MessageType.success,
      'input'       => MessageType.input,
      _             => throw Exception(),
    };
    if (this._controller._messageList.isNotEmpty) {
      this._controller._messageList.add(
        const SizedBox(height: 8),
      );
    }
    this._controller._messageList.add(
      MessageCard(
        type: typeValue,
        title: title,
        description: description,
      ),
    );
    var shouldScrollToEnd = this._controller._messageListScrollController.position.pixels == this._controller._messageListScrollController.position.maxScrollExtent;
    this._controller.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    if (shouldScrollToEnd) {
      this._controller._messageListScrollController.jumpTo(this._controller._messageListScrollController.position.maxScrollExtent);
    }
    return ();
  }

  Future<(String,)> callbackReceiveSubmission(
    String       type,
    List<String> option,
  ) async {
    var value = '';
    var typeValue = switch (type) {
      'pause'       => SubmissionType.pause,
      'boolean'     => SubmissionType.boolean,
      'integer'     => SubmissionType.integer,
      'floater'     => SubmissionType.floater,
      'size'        => SubmissionType.size,
      'string'      => SubmissionType.string,
      'path'        => SubmissionType.path,
      'enumeration' => SubmissionType.enumeration,
      _             => throw Exception(),
    };
    var completer = Completer<Object?>();
    this._controller._submissionBar = SubmissionBar(
      type: typeValue,
      option: option,
      completer: completer,
    );
    this._controller.setState(() {});
    var valueSource = await completer.future;
    switch (typeValue) {
      case SubmissionType.pause: {
        valueSource as Null;
        value = '';
        break;
      }
      case SubmissionType.boolean: {
        valueSource as Boolean?;
        value = valueSource == null ? '' : (!valueSource ? 'n' : 'y');
        break;
      }
      case SubmissionType.integer: {
        valueSource as Integer?;
        value = valueSource == null ? '' : (valueSource == 0 ? '0' : (valueSource < 0 ? '-${-valueSource}' : '+${valueSource}'));
        break;
      }
      case SubmissionType.floater: {
        valueSource as Floater?;
        value = valueSource == null ? '' : (valueSource == 0.0 ? '0.0' : (valueSource < 0.0 ? '-${-valueSource}' : '+${valueSource}'));
        break;
      }
      case SubmissionType.size: {
        valueSource as String?;
        value = valueSource == null ? '' : (valueSource);
        break;
      }
      case SubmissionType.string: {
        valueSource as String?;
        value = valueSource == null ? '' : (valueSource);
        break;
      }
      case SubmissionType.path: {
        valueSource as String?;
        value = valueSource == null ? '' : (valueSource);
        break;
      }
      case SubmissionType.enumeration: {
        valueSource as Integer?;
        value = valueSource == null ? '' : ('${valueSource}');
        break;
      }
    }
    this._controller._submissionBar = const SubmissionBar(
      type: null,
      option: [],
      completer: null,
    );
    this._controller.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    return (value,);
  }

  Future<(String,)> callbackPickPath(
    String type,
  ) async {
    var target = '';
    var setting = Provider.of<SettingProvider>(this._controller.context, listen: false);
    switch (type) {
      case 'open_file': {
        target = await StorageHelper.pickOpenFile('ModdingWorker.Generic', setting.data.mFallbackDirectory) ?? '';
        break;
      }
      case 'open_directory': {
        target = await StorageHelper.pickOpenDirectory('ModdingWorker.Generic') ?? '';
        break;
      }
      case 'save_file': {
        target = await StorageHelper.pickSaveFile('ModdingWorker.Generic') ?? '';
        break;
      }
    }
    return (target,);
  }

  Future<()> callbackPushNotification(
    String title,
    String description,
  ) async {
    await NotificationHelper.push(title, description);
    return ();
  }

  // #endregion

}
