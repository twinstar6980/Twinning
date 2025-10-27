import '/common.dart';
import 'package:flutter/services.dart';

// ----------------

class PlatformMethod {

  // #region channel

  static final MethodChannel? _channel = () {
    if (SystemChecker.isAndroid || SystemChecker.isIphone) {
      return MethodChannel('${ApplicationInformation.identifier}.CustomMethodChannel');
    }
    return null;
  }();

  // #endregion

  // #region interface

  static Future<String?> pickStorageItem(
    String type,
    String location,
    String name,
  ) async {
    assertTest(SystemChecker.isAndroid || SystemChecker.isIphone);
    var result = (await _channel!.invokeMethod('pick_storage_item', <dynamic, dynamic>{ 'type': type, 'location': location, 'name': name }) as String?);
    return result;
  }

  static Future<String?> copyStorageFile(
    String source,
    String placement,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var result = (await _channel!.invokeMethod('copy_storage_file', <dynamic, dynamic>{ 'source': source, 'placement': placement }) as String);
    return result;
  }

  static Future<Boolean> checkExternalStoragePermission(
    String mode,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var result = (await _channel!.invokeMethod('check_external_storage_permission', <dynamic, dynamic>{ 'mode': mode }) as Boolean);
    return result;
  }

  static Future<String> queryExternalStoragePath(
  ) async {
    assertTest(SystemChecker.isAndroid);
    var result = (await _channel!.invokeMethod('query_external_storage_path', <dynamic, dynamic>{}) as String);
    return result;
  }

  // #endregion

}
