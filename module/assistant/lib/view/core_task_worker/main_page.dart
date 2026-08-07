import '/common.dart';
import '/setting.dart';
import '/utility/wrapper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/system_notification_manager.dart';
import '/bridge/library.dart' as bridge;
import '/bridge/client.dart' as bridge;
import '/bridge/launcher.dart' as bridge;
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/core_task_worker/setting.dart';
import '/view/core_task_worker/configuration.dart';
import '/view/core_task_worker/option.dart';
import '/view/core_task_worker/message_type.dart';
import '/view/core_task_worker/message_card.dart';
import '/view/core_task_worker/submission_type.dart';
import '/view/core_task_worker/submission_bar.dart';
import '/view/core_task_worker/value_expression.dart';
import 'dart:async' as lib;

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.setting,
    required this.configuration,
    required this.option,
  });

  // ----------------

  final Setting       setting;
  final Configuration configuration;
  final Option        option;

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> implements ModulePageState {

  late List<String>          _additionalArgument;
  late List<Widget>          _messageList;
  late ScrollController      _messageListScrollController;
  late SubmissionBar?        _submissionBar;
  late _MainPageBridgeClient _sessionClient;
  late Boolean               _sessionRunning;
  late Wrapper<Boolean>?     _sessionTerminating;

  Future<Void> _sendMessage(
    MessageType  type,
    String       title,
    List<String> description,
  ) async {
    this._messageList.add(
      BoxContainer.of(
        margin: .fromLTRB(0, 4, 0, 4),
        child: MessageCard(
          type: type,
          title: title,
          description: description,
        ),
      ),
    );
    var shouldScrollToEnd = this._messageListScrollController.position.pixels == this._messageListScrollController.position.maxScrollExtent;
    await refreshState(this.setState);
    if (shouldScrollToEnd) {
      this._messageListScrollController.jumpTo(this._messageListScrollController.position.maxScrollExtent);
    }
    return;
  }

  Future<ValueExpression?> _receiveSubmission(
    SubmissionType type,
    List<String>   option,
  ) async {
    var setting = SettingProvider.of(this.context, listen: false);
    var history = setting.state.coreTaskWorkerSubmissionHistory[type.index];
    var completer = lib.Completer<Void>();
    var valueWrapper = Wrapper<ValueExpression?>();
    this._submissionBar = SubmissionBar(
      type: type,
      option: option,
      history: history,
      value: valueWrapper,
      onSubmit: () async {
        completer.complete();
      },
      terminate: null,
      onTerminate: null,
    );
    await refreshState(this.setState);
    await completer.future;
    var value = valueWrapper.value;
    this._submissionBar = SubmissionBar(
      type: null,
      option: null,
      history: null,
      value: null,
      onSubmit: null,
      terminate: this._sessionTerminating,
      onTerminate: () async {
        this._sessionTerminating!.value = !this._sessionTerminating!.value;
      },
    );
    await refreshState(this.setState);
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
    var exception = null as ({Object exception, StackTrace stack})?;
    this._sessionRunning = true;
    this._sessionTerminating = .of(false);
    this._messageList.clear();
    this._submissionBar = SubmissionBar(
      type: null,
      option: null,
      history: null,
      value: null,
      onSubmit: null,
      terminate: this._sessionTerminating,
      onTerminate: () async {
        this._sessionTerminating!.value = !this._sessionTerminating!.value;
      },
    );
    await refreshState(this.setState);
    try {
      var (temporaryDirectory, temporaryDirectoryFinalizer) = await StorageHelper.temporary(useCache: true);
      var kernel = temporaryDirectory.join('kernel');
      var library = bridge.Library();
      try {
        await StorageHelper.copy(this.widget.setting.kernel, kernel, true);
        library.open(kernel);
        result = await bridge.Launcher.launch(this._sessionClient, library, this.widget.setting.script.emit(), this.widget.setting.argument + this._additionalArgument);
      }
      catch (e, s) {
        exception = (exception: e, stack: s);
      }
      if (library.state()) {
        library.close();
      }
      await temporaryDirectoryFinalizer.dispose();
    }
    catch (e, s) {
      exception = (exception: e, stack: s);
    }
    if (exception == null) {
      this._sendMessage(.success, 'SUCCEEDED', result!);
    }
    else {
      this._sendMessage(.error, 'FAILED', ConvertHelper.generateExceptionMessage(exception.exception, exception.stack));
    }
    this._sessionRunning = false;
    this._sessionTerminating = null;
    this._submissionBar = null;
    await refreshState(this.setState);
    return exception != null ? null : result!;
  }

  // ----------------

  @override
  modulePageOpenView() async {
    return;
  }

  @override
  modulePageCloseView() async {
    if (this._sessionRunning) {
      this._sessionTerminating!.value = true;
      this._submissionBar!.onSubmit?.call();
      var dialogFinalizer = await MoreModalDialogExtension.showForWait(context, 'Closing ...');
      try {
        while (this._sessionRunning) {
          await Future.delayed(.new(milliseconds: 100));
        }
      }
      finally {
        await dialogFinalizer.dispose();
      }
    }
    return true;
  }

  @override
  modulePageEnterView() async {
    if (this._submissionBar != null) {
      this._submissionBar = SubmissionBar(
        type: this._submissionBar!.type,
        option: this._submissionBar!.option,
        history: this._submissionBar!.history,
        value: this._submissionBar!.value,
        onSubmit: this._submissionBar!.onSubmit,
        terminate: this._submissionBar!.terminate,
        onTerminate: this._submissionBar!.onTerminate,
      );
      await refreshState(this.setState);
    }
    return;
  }

  @override
  modulePageExitView() async {
    return;
  }

  @override
  modulePageApplyOption(option) async {
    option as Option;
    if (option.additionalArgument != null) {
      this._additionalArgument.addAll(option.additionalArgument!);
    }
    if (option.immediateLaunch ?? this.widget.setting.immediateLaunch) {
      this._launchSession();
    }
    await refreshState(this.setState);
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = Option();
    option.immediateLaunch = null;
    option.additionalArgument = this._additionalArgument.toList();
    return option;
  }

  @override
  modulePageExecuteCommand(method, argument) async {
    var result = <String>[];
    switch (method) {
      case 'launch': {
        this._additionalArgument = [...argument];
        await refreshState(this.setState);
        var launchResult = await _launchSession();
        result.addAll(launchResult ?? []);
        break;
      }
      default: throw UnsupportedException();
    }
    return result;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    this._additionalArgument = [];
    this._messageList = [];
    this._messageListScrollController = .new();
    this._submissionBar = null;
    this._sessionClient = .new(this);
    this._sessionRunning = false;
    this._sessionTerminating = null;
    postTask(() async {
      await this.modulePageOpenView();
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
    return ModulePageRegion(
      onStorageDrop: null,
      content: FlexContainer.vertical([
        ListContainer.of(
          padding: .fromLTRB(12, 4, 12, 4),
          controller: this._messageListScrollController,
          itemCount:this._messageList.length,
          itemBuilder: (context, index) => this._messageList[index],
        ).withStyledScrollBar(
          controller: this._messageListScrollController,
        ).withFlexExpanded(),
        FlexContainer.horizontal([
          StyledProgress.linear(
            paused: !this._sessionRunning ? false : this._submissionBar!.type != null,
            value: !this._sessionRunning ? 1.0 : null,
          ).withFlexExpanded(),
        ]),
      ]),
      bottom: this._sessionRunning
        ? this._submissionBar!
        : StyledBottomBar.standard(
          primary: StyledFloatingButton.standard(
            tooltip: 'Launch',
            icon: IconView.of(IconSet.play_circle),
            onPressed: (context) async {
              this._launchSession();
            },
          ),
          secondary: [
            StyledBadge.standard(
              label: StyledText.inherit('${this._additionalArgument.length}'),
              child: StyledIconButton.filledTonal(
                tooltip: 'Additional Argument',
                icon: BoxContainer.of(
                  constraints: .tightFor(width: 40),
                  child: IconView.of(IconSet.attach_file, fill: 1),
                ),
                onPressed: (context) async {
                  await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
                    title: 'Additional Argument',
                    contentBuilder: (context, setStateForPanel) => [
                      StyledInput.outlined(
                        type: .multiline,
                        format: null,
                        hint: null,
                        prefix: null,
                        suffix: null,
                        value: ConvertHelper.makeStringListToStringWithLine(this._additionalArgument),
                        onChanged: (context, value) async {
                          this._additionalArgument.clear();
                          this._additionalArgument.addAll(ConvertHelper.parseStringListFromStringWithLine(value));
                          await refreshState(setStateForPanel);
                          await refreshState(this.setState);
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

class _MainPageBridgeClient extends bridge.Client {

  // #region constructor

  _MainPageState _controller;

  Boolean _running;

  // ----------------

  _MainPageBridgeClient(
    _MainPageState controller,
  ) :
    this._controller = controller,
    this._running = false;

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

  // #endregion

  // #region handle

  @override
  handle(argument) async {
    assertTest(this._running);
    var result = <String>[];
    assertTest(argument.length >= 1);
    switch (argument[0]) {
      case 'query_context': {
        assertTest(argument.length == 2);
        var detail = await this.handleQueryContext(
          argument[1],
        );
        result.add(detail.value);
        break;
      }
      case 'send_message': {
        assertTest(argument.length >= 3);
        // ignore: unused_local_variable
        var detail = await this.handleSendMessage(
          argument[1],
          argument[2],
          argument.sublist(3),
        );
        break;
      }
      case 'receive_submission': {
        assertTest(argument.length >= 2);
        var detail = await this.handleReceiveSubmission(
          argument[1],
          argument.sublist(2),
        );
        result.add(detail.value);
        break;
      }
      case 'query_storage_item': {
        assertTest(argument.length == 2);
        var detail = await this.handleQueryStorageItem(
          argument[1],
        );
        result.add(detail.target.emit());
        break;
      }
      case 'reveal_storage_item': {
        assertTest(argument.length == 2);
        // ignore: unused_local_variable
        var detail = await this.handleRevealStorageItem(
          argument[1].selfLet((it) => .of(it)),
        );
        break;
      }
      case 'pick_storage_item': {
        assertTest(argument.length == 5);
        var detail = await this.handlePickStorageItem(
          argument[1],
          argument[2].selfLet((it) => .parse(it)),
          argument[3].selfLet((it) => .of(it)),
          argument[4],
        );
        result.addAll(detail.target.map((it) => it.emit()));
        break;
      }
      case 'push_system_notification': {
        assertTest(argument.length == 3);
        // ignore: unused_local_variable
        var detail = await this.handlePushSystemNotification(
          argument[1],
          argument[2],
        );
        break;
      }
      default: throw Exception('invalid method');
    }
    return result;
  }

  // ----------------

  Future<({String value})> handleQueryContext(
    String name,
  ) async {
    assertTest(name == 'name' || name == 'version' || name == 'terminate');
    var value = '';
    if (name == 'name') {
      value = 'assistant';
    }
    if (name == 'version') {
      value = ApplicationInformation.version;
    }
    if (name == 'terminate') {
      value = this._controller._sessionTerminating!.value.toString();
    }
    return (
      value: value,
    );
  }

  // ----------------

  Future<()> handleSendMessage(
    String       type,
    String       title,
    List<String> description,
  ) async {
    var typeValue = ConvertHelper.parseEnumerationFromStringOfSnakeCase(type, MessageType.values);
    this._controller._sendMessage(typeValue, title, description);
    return (
    );
  }

  Future<({String value})> handleReceiveSubmission(
    String       type,
    List<String> option,
  ) async {
    var typeValue = ConvertHelper.parseEnumerationFromStringOfSnakeCase(type, SubmissionType.values);
    var valueValue = await this._controller._receiveSubmission(typeValue, option);
    var value = '';
    if (valueValue != null) {
      value = ValueExpressionHelper.makeString(valueValue);
      if (valueValue.macro == null) {
        value = '??${value}';
      }
    }
    return (
      value: value,
    );
  }

  // ----------------

  Future<({StoragePath target})> handleQueryStorageItem(
    String type,
  ) async {
    var typeValue = ConvertHelper.parseEnumerationFromStringOfSnakeCase(type, StorageQueryType.values);
    var target = await StorageHelper.query(typeValue);
    return (
      target: target,
    );
  }

  Future<()> handleRevealStorageItem(
    StoragePath target,
  ) async {
    StorageHelper.reveal(target);
    return (
    );
  }

  Future<({List<StoragePath> target})> handlePickStorageItem(
    String      type,
    Boolean     multiply,
    StoragePath location,
    String      name,
  ) async {
    var typeValue = ConvertHelper.parseEnumerationFromStringOfSnakeCase(type, StoragePickType.values);
    var target = await StorageHelper.pick(typeValue, multiply, location, name);
    return (
      target: target,
    );
  }

  // ----------------

  Future<()> handlePushSystemNotification(
    String title,
    String description,
  ) async {
    await SystemNotificationManager.instance.push(title, description);
    return (
    );
  }

  // #endregion

}
