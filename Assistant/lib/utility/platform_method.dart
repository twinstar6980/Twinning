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

  static Future<({String? target})> pickStorageItem(
    String type,
    String location,
    String name,
  ) async {
    assertTest(SystemChecker.isAndroid || SystemChecker.isIphone);
    var detail = await _channel!.invokeMethod('pick_storage_item', <Object?, Object?>{
      'type': type,
      'location': location,
      'name': name,
    }) as Map<Object?, Object?>;
    return (
      target: detail['target'] as String?,
    );
  }

  static Future<({String destination})> copyStorageFile(
    String source,
    String placement,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await _channel!.invokeMethod('copy_storage_file', <Object?, Object?>{
      'source': source,
      'placement': placement,
    }) as Map<Object?, Object?>;
    return (
      destination: detail['destination'] as String,
    );
  }

  static Future<({Boolean state})> checkExternalStoragePermission(
    String mode,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await _channel!.invokeMethod('check_external_storage_permission', <Object?, Object?>{
      'mode': mode,
    }) as Map<Object?, Object?>;
    return (
      state: detail['state'] as Boolean,
    );
  }

  static Future<({String path})> queryExternalStoragePath(
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await _channel!.invokeMethod('query_external_storage_path', <Object?, Object?>{
    }) as Map<Object?, Object?>;
    return (
      path: detail['path'] as String,
    );
  }

  // #endregion

}
