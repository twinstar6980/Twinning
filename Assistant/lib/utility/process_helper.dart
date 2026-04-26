import '/common.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
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

  // #region program

  static Future<StoragePath?> searchProgram(
    String  name,
    Boolean allowExtension,
  ) async {
    var result = null as StoragePath?;
    var itemDelimiter = SystemChecker.isWindows ? ';' : ':';
    var pathEnvironment = ProcessHelper.findEnvironment('PATH');
    assertTest(pathEnvironment != null);
    var pathList = pathEnvironment!.split(itemDelimiter).map(StoragePath.of);
    var pathExtensionList = [''];
    if (SystemChecker.isWindows && allowExtension) {
      var pathExtensionEnvironment = ProcessHelper.findEnvironment('PATHEXT');
      assertTest(pathExtensionEnvironment != null);
      pathExtensionList.addAll(pathExtensionEnvironment!.split(itemDelimiter).map((it) => it.toLowerCase()));
    }
    for (var path in pathList) {
      for (var pathExtension in pathExtensionList) {
        var currentPath = path.join('${name}${pathExtension}');
        if (await StorageHelper.existFile(currentPath)) {
          result = currentPath;
          break;
        }
      }
    }
    return result;
  }

  static Future<StoragePath> searchProgramEnsure(
    String  name,
    Boolean allowExtension,
  ) async {
    var result = await ProcessHelper.searchProgram(name, allowExtension);
    if (result == null) {
      throw Exception('could not find \'${name}\' program from \'PATH\' environment');
    }
    return result;
  }

  // #endregion

}
