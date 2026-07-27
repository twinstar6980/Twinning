import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/json_helper.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/core_task_worker/option.dart';

// ----------------

class ForwardHelper {

  // #region utility

  static Future<List<String>> execute(
    BuildContext context,
    List<String> argument,
  ) async {
    var setting = SettingProvider.of(context, listen: false);
    var key = GlobalKey();
    await setting.state.homeInsertPage!(
      key,
      .new(
        title: ModuleHelper.query(.coreTaskWorker).name,
        type: .coreTaskWorker,
        option: Option(
          immediateLaunch: false,
          additionalArgument: null,
        ),
      ),
      true,
    );
    var result = await key.currentState!.as<ModulePageState>().modulePageExecuteCommand('launch', argument);
    await setting.state.homeRemovePage!(key);
    return result;
  }

  // ----------------

  static Future<Void> forward(
    BuildContext context,
    List<String> argument,
  ) async {
    var setting = SettingProvider.of(context, listen: false);
    await setting.state.handleLaunch!(
      ModuleHelper.query(.coreTaskWorker).name,
      .coreTaskWorker,
      Option(
        immediateLaunch: true,
        additionalArgument: argument,
      ),
    );
    return;
  }

  static Future<Void> forwardMany(
    BuildContext       context,
    List<List<String>> argument,
    Boolean            parallel,
  ) async {
    if (!parallel) {
      await ForwardHelper.forward(context, argument.expand((value) => [...value]).toList());
    }
    else {
      for (var argumentItem in argument) {
        await ForwardHelper.forward(context, argumentItem);
      }
    }
    return;
  }

  // ----------------

  static String makeMethodMaybeBatch(
    String  method,
    Boolean batch,
  ) {
    return '${method}${!batch ? '' : '!batch'}';
  }

  static List<String> makeArgumentForCommand(
    String? input,
    String? method,
    Object? argument,
  ) {
    var command = <String>[];
    command.add(input ?? '?none');
    if (method != null) {
      command.add('-method');
      command.add(method);
    }
    if (argument != null) {
      command.add('-argument');
      command.add(JsonHelper.encodeText(argument, indented: false));
    }
    return command;
  }

  // #endregion

}
