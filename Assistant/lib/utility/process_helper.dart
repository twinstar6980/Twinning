import '/common.dart';
import '/utility/storage_helper.dart';
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

  // #region program

  static Future<String?> searchProgram(
    String  name,
    Boolean allowExtension,
  ) async {
    var result = null as String?;
    var itemDelimiter = SystemChecker.isWindows ? ';' : ':';
    var pathEnvironment = queryEnvironment('PATH');
    assertTest(pathEnvironment != null);
    var pathList = pathEnvironment!.split(itemDelimiter);
    if (SystemChecker.isWindows) {
      pathList = pathList.map(StorageHelper.regularize).toList();
    }
    var pathExtensionList = [''];
    if (SystemChecker.isWindows && allowExtension) {
      var pathExtensionEnvironment = queryEnvironment('PATHEXT');
      assertTest(pathExtensionEnvironment != null);
      pathExtensionList.addAll(pathExtensionEnvironment!.split(itemDelimiter));
    }
    for (var path in pathList) {
      var pathBase = '${path}/${name}';
      var pathExtension = null as String?;
      for (var pathExtensionItem in pathExtensionList) {
        if (await StorageHelper.existFile('${pathBase}${pathExtensionItem}')) {
          pathExtension = pathExtensionItem;
          break;
        }
      }
      if (pathExtension != null) {
        result = '${pathBase}${pathExtension}';
        break;
      }
    }
    return result;
  }

  static Future<String> searchProgramEnsure(
    String  name,
    Boolean allowExtension,
  ) async {
    var result = await searchProgram(name, allowExtension);
    if (result == null) {
      throw Exception('could not find \'${name}\' program from \'PATH\' environment');
    }
    return result;
  }

  // #endregion

}
