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
    this._channel = null {
    if (SystemChecker.isWindows || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
      this._channel = .new('${ApplicationInformation.identifier}.CustomMethodChannel');
    }
    return;
  }

  // #endregion

  // #region interface

  Future<({String placement})> copyStorageFile(
    String target,
    String location,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('copy_storage_file', <Object?, Object?>{
      'target': target,
      'location': location,
    });
    detail!;
    return (
      placement: detail['placement'] as String,
    );
  }

  Future<()> revealStorageFile(
    String target,
  ) async {
    assertTest(SystemChecker.isAndroid);
    // ignore: unused_local_variable
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('reveal_storage_file', <Object?, Object?>{
      'target': target,
    });
    detail!;
    return (
    );
  }

  Future<({String? target})> pickStorageItem(
    String type,
    String location,
    String name,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('pick_storage_item', <Object?, Object?>{
      'type': type,
      'location': location,
      'name': name,
    });
    detail!;
    return (
      target: detail['target'] as String?,
    );
  }

  Future<({Boolean state})> checkExternalStoragePermission(
    String mode,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('check_external_storage_permission', <Object?, Object?>{
      'mode': mode,
    });
    detail!;
    return (
      state: detail['state'] as Boolean,
    );
  }

  Future<({String path})> queryExternalStoragePath(
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('query_external_storage_path', <Object?, Object?>{
    });
    detail!;
    return (
      path: detail['path'] as String,
    );
  }

  // #endregion

}
