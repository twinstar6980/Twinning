import '/common.dart';
import '/utility/storage_path.dart';
import 'dart:io';

// ----------------

class ProcessHelper {

  // #region workspace

  static StoragePath getWorkspace(
  ) {
    return .of(Directory.current.path);
  }

  // #endregion

  // #region environment

  static Map<String, String> getEnvironment(
  ) {
    return {...Platform.environment};
  }

  // ----------------

  static String? findEnvironment(
    String name,
  ) {
    return Platform.environment[name];
  }

  // #endregion

  // #region process

  static Future<({Integer code, String output, String error})> runProcess(
    StoragePath          program,
    List<String>         argument,
    StoragePath?         workspace,
    Map<String, String>? environment,
  ) async {
    if (workspace == null) {
      workspace = ProcessHelper.getWorkspace();
    }
    if (environment == null) {
      environment = ProcessHelper.getEnvironment();
    }
    var process = await Process.run(
      program.emitNative(),
      argument,
      workingDirectory: workspace.emitNative(),
      environment: environment,
    );
    return (
      code: process.exitCode,
      output: process.stdout.toString(),
      error: process.stderr.toString(),
    );
  }

  // #endregion

}
