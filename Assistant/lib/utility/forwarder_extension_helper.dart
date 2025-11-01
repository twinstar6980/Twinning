import '/common.dart';
import '/utility/storage_helper.dart';
import '/utility/process_helper.dart';

// ----------------

class ForwarderExtensionHelper {

  // #region utility

  static Future<Boolean?> check(
  ) async {
    var result = null as Boolean?;
    if (SystemChecker.isWindows) {
      var stateFile = '${await StorageHelper.queryApplicationSharedDirectory()}/forwarder';
      result = await StorageHelper.exist(stateFile);
    }
    if (SystemChecker.isLinux) {
      result = false;
    }
    if (SystemChecker.isMacintosh) {
      var pluginkitResult = await ProcessHelper.runProcess(
        '/usr/bin/pluginkit',
        [
          '-m',
          '-i', '${ApplicationInformation.identifier}.Forwarder',
        ],
        null,
      );
      result = pluginkitResult.$1 == 0 && pluginkitResult.$2.startsWith('+');
    }
    if (SystemChecker.isAndroid) {
      result = true;
    }
    if (SystemChecker.isIphone) {
      result = null;
    }
    return result;
  }

  static Future<Void> toggle(
    Boolean? state,
  ) async {
    if (state == null) {
      return;
    }
    if (SystemChecker.isWindows) {
      var stateFile = '${await StorageHelper.queryApplicationSharedDirectory()}/forwarder';
      var exist = await StorageHelper.exist(stateFile);
      if (!state && exist) {
        await StorageHelper.remove(stateFile);
      }
      if (state && !exist) {
        await StorageHelper.createFile(stateFile);
      }
    }
    if (SystemChecker.isLinux) {
      throw UnsupportedException();
    }
    if (SystemChecker.isMacintosh) {
      var pluginkitResult = await ProcessHelper.runProcess(
        '/usr/bin/pluginkit',
        [
          '-e', '${!state ? 'ignore' : 'use'}',
          '-i', '${ApplicationInformation.identifier}.Forwarder',
        ],
        null,
      );
      assertTest(pluginkitResult.$1 == 0);
    }
    if (SystemChecker.isAndroid) {
      throw UnsupportedException();
    }
    if (SystemChecker.isIphone) {
      throw UnsupportedException();
    }
    return;
  }

  // #endregion

}
