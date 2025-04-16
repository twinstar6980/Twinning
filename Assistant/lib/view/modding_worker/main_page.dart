import '/common.dart';
import '/setting.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/control_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/bridge/library.dart' as bridge;
import '/bridge/client.dart' as bridge;
import '/bridge/launcher.dart' as bridge;
import '/view/home/common.dart';
import '/view/modding_worker/message_type.dart';
import '/view/modding_worker/message_card.dart';
import '/view/modding_worker/submission_type.dart';
import '/view/modding_worker/submission_bar.dart';
import '/view/modding_worker/value_expression.dart';
import 'dart:async';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.option,
  });

  // ----------------

  final List<String> option;

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> implements CustomModulePageState {

  late List<String>                _additionalArgument;
  late List<Widget>                _messageList;
  late ScrollController            _messageListScrollController;
  late List<List<ValueExpression>> _submissionHistory;
  late SubmissionBar               _submissionBar;
  late _MainPageBridgeClient       _sessionClient;
  late Boolean                     _sessionRunning;

  Future<Void> _sendMessage(
    MessageType  type,
    String       title,
    List<String> description,
  ) async {
    this._messageList.add(
      Container(
        margin: const EdgeInsets.fromLTRB(0, 4, 0, 4),
        child: MessageCard(
          type: type,
          title: title,
          description: description,
        ),
      ),
    );
    var shouldScrollToEnd = this._messageListScrollController.position.pixels == this._messageListScrollController.position.maxScrollExtent;
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    if (shouldScrollToEnd) {
      this._messageListScrollController.jumpTo(this._messageListScrollController.position.maxScrollExtent);
    }
    return;
  }

  Future<ValueExpression?> _receiveSubmission(
    SubmissionType type,
    List<String>   option,
  ) async {
    var history = this._submissionHistory[type.index];
    var completer = Completer<Void>();
    var valueWrapper = Wrapper<ValueExpression?>(null);
    this._submissionBar = SubmissionBar(
      type: type,
      option: option,
      history: history,
      value: valueWrapper,
      completer: completer,
    );
    this.setState(() {});
    await completer.future;
    var value = valueWrapper.value;
    this._submissionBar = const SubmissionBar(
      type: null,
      option: null,
      history: null,
      value: null,
      completer: null,
    );
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    if (value != null) {
      var valueString = ValueExpressionHelper.makeString(value);
      history.removeWhere((item) => ValueExpressionHelper.makeString(item) == valueString);
      history.add(value);
    }
    return value;
  }

  Future<List<String>?> _launchSession(
  ) async {
    assertTest(!this._sessionRunning);
    var result = null as List<String>?;
    var exception = null as Object?;
    this._sessionRunning = true;
    this._messageList.clear();
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    try {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      var kernel = await StorageHelper.temporary();
      var library = bridge.Library();
      try {
        await StorageHelper.copyFile(setting.data.mModdingWorker.mKernel, kernel);
        library.open(kernel);
        result = await bridge.Launcher.launch(this._sessionClient, library, setting.data.mModdingWorker.mScript, setting.data.mModdingWorker.mArgument + this._additionalArgument);
      }
      catch (e) {
        exception = e;
      }
      if (library.state()) {
        library.close();
      }
      if (await StorageHelper.existFile(kernel)) {
        await StorageHelper.removeFile(kernel);
      }
    }
    catch (e) {
      exception = e;
    }
    if (exception == null) {
      this._sendMessage(MessageType.success, 'SUCCEEDED', result!);
    }
    else {
      this._sendMessage(MessageType.error, 'FAILED', [exception.toString()]);
    }
    this._sessionRunning = false;
    this.setState(() {});
    await WidgetsBinding.instance.endOfFrame;
    return exception == null ? result! : null;
  }

  // ----------------

  @override
  modulePageApplyOption(optionView) async {
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var optionImmediateLaunch = null as Boolean?;
    var optionAdditionalArgument = null as List<String>?;
    var option = CommandLineReader(optionView);
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
    if (optionAdditionalArgument != null) {
      this._additionalArgument.addAll(optionAdditionalArgument);
    }
    if (optionImmediateLaunch) {
      this._launchSession();
    }
    this.setState(() {});
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = CommandLineWriter();
    if (option.check('-additional_argument')) {
      option.nextStringList(this._additionalArgument);
    }
    return option.done();
  }

  @override
  modulePageRequestClose() async {
    if (this._sessionRunning) {
      await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
        title: 'Session In Progress',
        contentBuilder: (context, setStaate) => [],
        actionBuilder: null,
      ));
      return false;
    }
    return true;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._additionalArgument = [];
    this._messageList = [];
    this._messageListScrollController = ScrollController();
    this._submissionHistory = SubmissionType.values.map((value) => <ValueExpression>[]).toList();
    this._submissionBar = const SubmissionBar(
      type: null,
      option: null,
      history: null,
      value: null,
      completer: null,
    );
    this._sessionClient = _MainPageBridgeClient(this);
    this._sessionRunning = false;
    ControlHelper.postTask(() async {
      await this.modulePageApplyOption(this.widget.option);
    });
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
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
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return CustomModulePageRegion(
      onDropFile: null,
      content: Column(
        children: [
          Expanded(
            child: SelectionArea(
              child: Scrollbar(
                interactive: true,
                controller: this._messageListScrollController,
                child: ListView(
                  padding: const EdgeInsets.fromLTRB(12, 4, 12, 4),
                  controller: this._messageListScrollController,
                  children: [...this._messageList],
                ),
              ),
            ),
          ),
          LinearProgressIndicator(
            value: !this._sessionRunning ? 1 : this._submissionBar.type == null ? null : 1,
            color: !this._sessionRunning ? null : this._submissionBar.type == null ? null : theme.colorScheme.tertiary,
          ),
        ],
      ),
      bottom: this._sessionRunning
        ? this._submissionBar
        : CustomBottomBarContent(
          primary: FloatingActionButton(
            tooltip: 'Launch',
            elevation: 0,
            focusElevation: 0,
            hoverElevation: 0,
            highlightElevation: 0,
            disabledElevation: 0,
            child: const Icon(IconSymbols.play_circle),
            onPressed: () async {
              this._launchSession();
            },
          ),
          secondary: [
            Badge.count(
              textStyle: theme.textTheme.labelSmall?.copyWith(
                fontFamily: '',
                fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
              ),
              count: this._additionalArgument.length,
              child: IconButton.filledTonal(
                tooltip: 'Additional Argument',
                padding: EdgeInsets.zero,
                icon: const SizedBox(
                  width: 56,
                  child: Icon(IconSymbols.attach_file),
                ),
                onPressed: () async {
                  await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
                    title: 'Additional Argument',
                    contentBuilder: (context, setState) => [
                      CustomTextField(
                        keyboardType: TextInputType.multiline,
                        inputFormatters: const [],
                        decoration: const InputDecoration(
                          contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                          filled: false,
                          border: OutlineInputBorder(),
                        ),
                        style: theme.textTheme.bodyLarge?.copyWith(
                          fontFamily: '',
                          fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                        ),
                        value: ConvertHelper.makeStringListToStringWithLine(this._additionalArgument),
                        onChanged: (value) async {
                          this._additionalArgument.clear();
                          this._additionalArgument.addAll(ConvertHelper.parseStringListFromStringWithLine(value));
                          this.setState(() {});
                          setState(() {});
                        },
                      ),
                    ],
                    actionBuilder: null,
                  ));
                },
              ),
            ),
          ],
        ),
    );
  }

}

class _MainPageBridgeClient implements bridge.Client {

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
    return;
  }

  @override
  finish() async {
    assertTest(this._running);
    this._running = false;
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
      case 'pick_storage_item': {
        assertTest(argument.length == 2);
        var detailType = argument[1];
        var detail = await this.callbackPickStorageItem(detailType);
        var detailTarget = detail.$1;
        result.add(detailTarget);
        break;
      }
      case 'push_system_notification': {
        assertTest(argument.length == 3);
        var detailTitle = argument[1];
        var detailDescription = argument[2];
        var detail = await this.callbackPushSystemNotification(detailTitle, detailDescription); // ignore: unused_local_variable
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
    var version = kApplicationVersion;
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
    this._controller._sendMessage(typeValue, title, description);
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
    var valueData = await this._controller._receiveSubmission(typeValue, option);
    if (valueData != null) {
      value = ValueExpressionHelper.makeString(valueData);
    }
    return (value,);
  }

  Future<(String,)> callbackPickStorageItem(
    String type,
  ) async {
    var target = '';
    switch (type) {
      case 'load_file': {
        target = await StorageHelper.pickLoadFile(this._controller.context, 'ModdingWorker.Generic') ?? '';
        break;
      }
      case 'load_directory': {
        target = await StorageHelper.pickLoadDirectory(this._controller.context, 'ModdingWorker.Generic') ?? '';
        break;
      }
      case 'save_file': {
        target = await StorageHelper.pickSaveFile(this._controller.context, 'ModdingWorker.Generic') ?? '';
        break;
      }
      default: throw Exception();
    }
    return (target,);
  }

  Future<()> callbackPushSystemNotification(
    String title,
    String description,
  ) async {
    await NotificationHelper.push(title, description);
    return ();
  }

  // #endregion

}
