import '/common.dart';
import '/module.dart';
import '/setting.dart';
import '/utility/json_helper.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class ForwardHelper {

  // #region utility

  static Future<Void> forward(
    BuildContext context,
    List<String> argument,
  ) async {
    var setting = Provider.of<SettingProvider>(context, listen: false);
    setting.state.mInsertTabItem!(ModuleLauncherConfiguration(
      title: ModuleHelper.query(ModuleType.modding_worker).name,
      type: ModuleType.modding_worker,
      option: ['-additional_argument', ...argument],
    ));
    return;
  }

  static Future<Void> forwardMany(
    BuildContext       context,
    List<List<String>> argument,
    Boolean            parallel,
  ) async {
    if (argument.length != 0) {
      if (!parallel) {
        forward(context, argument.expand((value) => [...value]).toList());
      }
      else {
        for (var argumentItem in argument) {
          forward(context, argumentItem);
        }
      }
    }
    return;
  }

  // ----------------

  static List<String> makeArgumentForCommand(
    String? input,
    String? method,
    Object? argument,
  ) {
    var command = <String>[];
    command.add(input ?? '?');
    if (method != null) {
      command.add('-method');
      command.add(method);
    }
    if (argument != null) {
      command.add('-argument');
      command.add(JsonHelper.serializeText(argument, indented: false));
    }
    return command;
  }

  // #endregion

}
