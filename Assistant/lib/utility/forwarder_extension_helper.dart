import '/common.dart';
import '/utility/storage_helper.dart';
import 'package:url_launcher/url_launcher.dart';

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
      result = null;
    }
    if (SystemChecker.isAndroid) {
      result = true;
    }
    if (SystemChecker.isIphone) {
      result = null;
    }
    return result;
  }

  static Future<Boolean?> toggle(
  ) async {
    var result = null as Boolean?;
    if (SystemChecker.isWindows) {
      var stateFile = '${await StorageHelper.queryApplicationSharedDirectory()}/forwarder';
      if (!await StorageHelper.exist(stateFile)) {
        await StorageHelper.createFile(stateFile);
      }
      else {
        await StorageHelper.remove(stateFile);
      }
      result = true;
    }
    if (SystemChecker.isLinux) {
      result = false;
    }
    if (SystemChecker.isMacintosh) {
      // Ventura 13 and later
      await launchUrl(Uri.parse('x-apple.systempreferences:com.apple.ExtensionsPreferences?extensionPointIdentifier=com.apple.fileprovider-nonui'), mode: LaunchMode.externalApplication);
      result = null;
    }
    if (SystemChecker.isAndroid) {
      result = false;
    }
    if (SystemChecker.isIphone) {
      result = false;
    }
    return result;
  }

  // #endregion

}
