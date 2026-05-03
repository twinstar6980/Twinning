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

  Future<({Boolean state})> checkApplicationPermission(
    String  name,
  ) async {
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('check_application_permission', <Object?, Object?>{
      'name': name,
    });
    detail!;
    return (
      state: detail['state']!.as<Boolean>(),
    );
  }

  Future<()> updateApplicationPermission(
    String  name,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('update_application_permission', <Object?, Object?>{
      'name': name,
    });
    detail!;
    return (
    );
  }

  Future<({Boolean state})> checkApplicationExtension(
    String  name,
  ) async {
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('check_application_extension', <Object?, Object?>{
      'name': name,
    });
    detail!;
    return (
      state: detail['state']!.as<Boolean>(),
    );
  }

  Future<()> updateApplicationExtension(
    String  name,
    Boolean state,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('update_application_extension', <Object?, Object?>{
      'name': name,
      'state': state,
    });
    detail!;
    return (
    );
  }

  // ----------------

  Future<({String target})> queryStorageItem(
    String type,
  ) async {
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('query_storage_item', <Object?, Object?>{
      'type': type,
    });
    detail!;
    return (
      target: detail['target']!.as<String>(),
    );
  }

  Future<()> revealStorageItem(
    String target,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('reveal_storage_item', <Object?, Object?>{
      'target': target,
    });
    detail!;
    return (
    );
  }

  Future<({List<String> target})> pickStorageItem(
    String  type,
    Boolean multiply,
    String  location,
    String  name,
  ) async {
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('pick_storage_item', <Object?, Object?>{
      'type': type,
      'multiply': multiply,
      'location': location,
      'name': name,
    });
    detail!;
    return (
      target: detail['target']!.as<List<Object?>>().cast<String>(),
    );
  }

  // ----------------

  Future<()> pushSystemNotification(
    String title,
    String description,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('push_system_notification', <Object?, Object?>{
      'title': title,
      'description': description,
    });
    detail!;
    return (
    );
  }

  // ----------------

  Future<({String destination})> onWindowsQueryStorageLongPath(
    String source,
  ) async {
    assertTest(SystemChecker.isWindows);
    var detail = await this._channel!.invokeMapMethod<Object?, Object?>('on_windows_query_storage_long_path', <Object?, Object?>{
      'source': source,
    });
    detail!;
    return (
      destination: detail['destination']!.as<String>(),
    );
  }

  // #endregion

}
