import '/common.dart';
import '/setting.dart';
import '/utility/wrapper.dart';
import '/utility/exception_helper.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/utility/notification_helper.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/bridge/library.dart' as bridge;
import '/bridge/client.dart' as bridge;
import '/bridge/launcher.dart' as bridge;
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/modding_worker/message_type.dart';
import '/view/modding_worker/message_card.dart';
import '/view/modding_worker/submission_type.dart';
import '/view/modding_worker/submission_bar.dart';
import '/view/modding_worker/value_expression.dart';
import 'dart:async';
import 'package:flutter/widgets.dart';
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

class _MainPageState extends State<MainPage> implements ModulePageState {

  late List<String>          _additionalArgument;
  late List<Widget>          _messageList;
  late ScrollController      _messageListScrollController;
  late SubmissionBar         _submissionBar;
  late _MainPageBridgeClient _sessionClient;
  late Boolean               _sessionRunning;

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
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var history = setting.state.moddingWorkerSubmissionHistory[type.index];
    var completer = Completer<Void>();
    var valueWrapper = Wrapper<ValueExpression?>(null);
    this._submissionBar = SubmissionBar(
      type: type,
      option: option,
      history: history,
      value: valueWrapper,
      completer: completer,
    );
    await refreshState(this.setState);
    await completer.future;
    var value = valueWrapper.value;
    this._submissionBar = SubmissionBar(
      type: null,
      option: null,
      history: null,
      value: null,
      completer: null,
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
    var exception = null as (Object, StackTrace)?;
    this._sessionRunning = true;
    this._messageList.clear();
    await refreshState(this.setState);
    try {
      var setting = Provider.of<SettingProvider>(this.context, listen: false);
      var kernel = await StorageHelper.temporary();
      var library = bridge.Library();
      try {
        await StorageHelper.copy(setting.data.moddingWorker.kernel, kernel);
        library.open(kernel);
        result = await bridge.Launcher.launch(this._sessionClient, library, setting.data.moddingWorker.script, setting.data.moddingWorker.argument + this._additionalArgument);
      }
      catch (e, s) {
        exception = (e, s);
      }
      if (library.state()) {
        library.close();
      }
      if (await StorageHelper.exist(kernel)) {
        await StorageHelper.remove(kernel);
      }
    }
    catch (e, s) {
      exception = (e, s);
    }
    if (exception == null) {
      this._sendMessage(.success, 'SUCCEEDED', result!);
    }
    else {
      this._sendMessage(.error, 'FAILED', [ExceptionHelper.generateMessage(exception.$1, exception.$2)]);
    }
    this._sessionRunning = false;
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
      await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
        title: 'Session In Progress',
        contentBuilder: (context, setStaate) => [],
        actionBuilder: null,
      ));
      return false;
    }
    return true;
  }

  @override
  modulePageEnterView() async {
    if (this._submissionBar.type != null) {
      this._submissionBar = SubmissionBar(
        type: this._submissionBar.type,
        option: this._submissionBar.option,
        history: this._submissionBar.history,
        value: this._submissionBar.value,
        completer: this._submissionBar.completer,
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
  modulePageApplyOption(optionView) async {
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    var optionImmediateLaunch = null as Boolean?;
    var optionAdditionalArgument = null as List<String>?;
    var option = CommandLineReader(optionView);
    if (option.check('-immediate_launch')) {
      optionImmediateLaunch = option.nextBoolean();
    }
    else {
      optionImmediateLaunch = setting.data.moddingWorker.immediateLaunch;
    }
    if (option.check('-additional_argument')) {
      optionAdditionalArgument = option.nextStringList();
    }
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
    if (optionAdditionalArgument != null) {
      this._additionalArgument.addAll(optionAdditionalArgument);
    }
    if (optionImmediateLaunch) {
      this._launchSession();
    }
    await refreshState(this.setState);
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

  // ----------------

  @override
  initState() {
    super.initState();
    this._additionalArgument = [];
    this._messageList = [];
    this._messageListScrollController = .new();
    this._submissionBar = SubmissionBar(
      type: null,
      option: null,
      history: null,
      value: null,
      completer: null,
    );
    this._sessionClient = .new(this);
    this._sessionRunning = false;
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
      onDropFile: null,
      content: FlexContainer.vertical([
        ListContainer.of(
          padding: .fromLTRB(12, 4, 12, 4),
          controller: this._messageListScrollController,
          itemCount:this._messageList.length,
          itemBuilder: (context, index) => this._messageList[index],
        ).withStyledScrollBar(
          controller: this._messageListScrollController,
        ).withFlexExpanded(),
        StyledProgress.linear(
          paused: !this._sessionRunning ? false : this._submissionBar.type != null,
          value: !this._sessionRunning ? 1.0 : null,
        ),
      ]),
      bottom: this._sessionRunning
        ? this._submissionBar
        : StyledBottomBar.standard(
          primary: StyledFloatingButton.standard(
            tooltip: 'Launch',
            icon: Icon(IconSet.play_circle),
            onPressed: (context) async {
              this._launchSession();
            },
          ),
          secondary: [
            StyledBadge.standard(
              label: StyledText.custom(
                '${this._additionalArgument.length}',
                style: getSpecialFontTextStyle(context),
              ),
              child: StyledIconButton.filledTonal(
                tooltip: 'Additional Argument',
                icon: BoxContainer.of(
                  constraints: .tightFor(width: 40),
                  child: Icon(IconSet.attach_file, fill: 1),
                ),
                onPressed: (context) async {
                  await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
                    title: 'Additional Argument',
                    contentBuilder: (context, setStateForPanel) => [
                      StyledInput.outlined(
                        style: getSpecialFontTextStyle(context),
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
      default: throw Exception('invalid method');
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
    var version = ApplicationInformation.version;
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
    var typeValue = switch (type) {
      'load_file'      => 'load_file',
      'load_directory' => 'load_directory',
      'save_file'      => 'save_file',
      _                => throw Exception(),
    };
    target = await StorageHelper.pick(typeValue, this._controller.context, '@ModdingWorker.Generic', null) ?? '';
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

TextStyle getSpecialFontTextStyle(
  BuildContext context, {
  Boolean      listen = true,
}) {
  var setting = Provider.of<SettingProvider>(context, listen: listen);
  return TextStyle(inherit: true).copyWith(
    fontFamily: '',
    fontFamilyFallback: [...setting.state.moddingWorkerMessageFontFamily, ...setting.state.themeFontFamliy],
  );
}
