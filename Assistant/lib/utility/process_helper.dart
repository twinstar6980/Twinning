import '/common.dart';
import 'dart:io';

// ----------------

class ProcessHelper {

  // #region environment

  static String? queryEnvironment(
    String name,
  ) {
    return Platform.environment[name];
  }

  // ----------------

  static Map<String, String> listEnvironment(
  ) {
    return {...Platform.environment};
  }

  // #endregion

  // #region process

  static Future<(Integer, String, String)> runProcess(
    String               program,
    List<String>         argument,
    Map<String, String>? environment,
  ) async {
    if (environment == null) {
      environment = listEnvironment();
    }
    var process = await Process.run(
      program,
      argument,
      environment: environment,
    );
    return (process.exitCode, process.stdout.toString(), process.stderr.toString());
  }

  // #endregion

}
