import '/common.dart';
import '/utility/storage_path.dart';
import 'dart:io' as lib;

// ----------------

class ProcessHelper {

  // #region workspace

  static StoragePath getWorkspace(
  ) {
    return .of(lib.Directory.current.path);
  }

  // #endregion

  // #region environment

  static Map<String, String> getEnvironment(
  ) {
    return {...lib.Platform.environment};
  }

  // ----------------

  static String? findEnvironment(
    String name,
  ) {
    return lib.Platform.environment[name];
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
    var process = await lib.Process.run(
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
