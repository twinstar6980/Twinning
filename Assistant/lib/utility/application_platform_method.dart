import '/common.dart';
import 'package:flutter/services.dart';

// ----------------

class ApplicationPlatformMethod {

  // #region singleton

  static final ApplicationPlatformMethod instance = ._();

  // #endregion

  // #region constructor

  MethodChannel? _channel;

  // ----------------

  ApplicationPlatformMethod._(
  ) :
    _channel = null {
    if (SystemChecker.isAndroid || SystemChecker.isIphone) {
      _channel = .new('${ApplicationInformation.identifier}.CustomMethodChannel');
    }
    return;
  }

  // #endregion

  // #region interface

  Future<({String? target})> pickStorageItem(
    String type,
    String location,
    String name,
  ) async {
    assertTest(SystemChecker.isAndroid || SystemChecker.isIphone);
    var detail = await this._channel!.invokeMethod('pick_storage_item', <Object?, Object?>{
      'type': type,
      'location': location,
      'name': name,
    }) as Map<Object?, Object?>;
    return (
      target: detail['target'] as String?,
    );
  }

  Future<({String destination})> copyStorageFile(
    String source,
    String placement,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMethod('copy_storage_file', <Object?, Object?>{
      'source': source,
      'placement': placement,
    }) as Map<Object?, Object?>;
    return (
      destination: detail['destination'] as String,
    );
  }

  Future<({Boolean state})> checkExternalStoragePermission(
    String mode,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMethod('check_external_storage_permission', <Object?, Object?>{
      'mode': mode,
    }) as Map<Object?, Object?>;
    return (
      state: detail['state'] as Boolean,
    );
  }

  Future<({String path})> queryExternalStoragePath(
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMethod('query_external_storage_path', <Object?, Object?>{
    }) as Map<Object?, Object?>;
    return (
      path: detail['path'] as String,
    );
  }

  // #endregion

}
