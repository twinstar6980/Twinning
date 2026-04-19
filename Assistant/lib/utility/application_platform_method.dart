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
    this._channel = .new('${ApplicationInformation.identifier}.CustomMethodChannel');

  // #endregion

  // #region interface

  Future<({Boolean state})> checkStoragePermission(
    String mode,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('check_storage_permission', <Object?, Object?>{
      'mode': mode,
    });
    detail!;
    return (
      state: detail['state'] as Boolean,
    );
  }

  Future<({String target})> queryStorageItem(
    String type,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('query_storage_item', <Object?, Object?>{
      'type': type,
    });
    detail!;
    return (
      target: detail['target'] as String,
    );
  }

  Future<()> revealStorageItem(
    String target,
  ) async {
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone);
    // ignore: unused_local_variable
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('reveal_storage_item', <Object?, Object?>{
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
    assertTest(SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone);
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

  Future<({String? path})> resolveContentUri(
    String  uri,
    String? fallback,
  ) async {
    assertTest(SystemChecker.isAndroid);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('resolve_content_uri', <Object?, Object?>{
      'uri': uri,
      'fallback': fallback,
    });
    detail!;
    return (
      path: detail['path'] as String?,
    );
  }

  // #endregion

}
