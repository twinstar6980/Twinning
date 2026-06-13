import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/json_helper.dart';
import '/view/home/module_page.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class ForwardHelper {

  // #region utility

  static Future<List<String>> execute(
    BuildContext context,
    List<String> argument,
  ) async {
    var setting = Provider.of<SettingProvider>(context, listen: false);
    var key = GlobalKey();
    await setting.state.homeInsertPage!(
      key,
      .new(
        title: ModuleHelper.query(.coreTaskWorker).name,
        type: .coreTaskWorker,
        option: [
          '-immediate_launch', 'false',
        ],
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
    var setting = Provider.of<SettingProvider>(context, listen: false);
    await setting.state.handleLaunch!(
      ModuleHelper.query(.coreTaskWorker).name,
      .coreTaskWorker,
      [
        '-immediate_launch', 'true',
        '-additional_argument', ...argument,
      ],
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
