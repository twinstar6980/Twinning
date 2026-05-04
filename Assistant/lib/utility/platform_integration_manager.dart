import '/common.dart';
import 'dart:async';
import 'package:flutter/services.dart';

// ----------------

class PlatformIntegrationManager {

  // #region singleton

  static final PlatformIntegrationManager instance = ._();

  // #endregion

  // #region constructor

  MethodChannel _channel;

  StreamController<String> _streamControllerForLink;

  // ----------------

  PlatformIntegrationManager._(
  ) :
    this._channel = .new('${ApplicationInformation.identifier}/PlatformIntegrationManager'),
    this._streamControllerForLink = .new() {
    this._channel.setMethodCallHandler((call) async => await this._handle(call));
  }

  // #endregion

  // #region handle

  Future<Object?> _handle(
    MethodCall call,
  ) async {
    var rawArgument = call.arguments as Map<Object?, Object?>;
    var getArgument = (String name) {
      return rawArgument[name];
    };
    var rawResult = <Object?, Object?>{};
    // ignore: unused_local_variable
    var setResult = (String name, Object? value) {
      rawArgument[name] = value;
      return null as Void;
    };
    switch (call.method) {
      case 'receive_application_link': {
        // ignore: unused_local_variable
        var detail = await this._handleReceiveApplicationLink(
          getArgument('target') as String,
        );
        break;
      }
      default: throw Exception('invalid method');
    }
    return rawResult;
  }

  // ----------------

  Future<()> _handleReceiveApplicationLink(
    String target,
  ) async {
    this._streamControllerForLink.sink.add(target);
    return (
    );
  }

  // #endregion

  // #region invoke

  Future<Map<Object?, Object?>> _invoke(
    String                method,
    Map<Object?, Object?> argument,
  ) async {
    return (await this._channel.invokeMapMethod<Object?, Object?>(method, argument))!;
  }

  // ----------------

  Future<({Boolean state})> invokeCheckApplicationPermission(
    String  name,
  ) async {
    var detail = await this._invoke('check_application_permission', {
      'name': name,
    });
    return (
      state: detail['state']!.as<Boolean>(),
    );
  }

  Future<()> invokeUpdateApplicationPermission(
    String  name,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._invoke('update_application_permission', {
      'name': name,
    });
    return (
    );
  }

  // ----------------

  Future<({Boolean state})> invokeCheckApplicationExtension(
    String  name,
  ) async {
    var detail = await this._invoke('check_application_extension', {
      'name': name,
    });
    return (
      state: detail['state']!.as<Boolean>(),
    );
  }

  Future<()> invokeUpdateApplicationExtension(
    String  name,
    Boolean state,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._invoke('update_application_extension', {
      'name': name,
      'state': state,
    });
    return (
    );
  }

  // ----------------

  Future<({String target})> invokeQueryStorageItem(
    String type,
  ) async {
    var detail = await this._invoke('query_storage_item', {
      'type': type,
    });
    return (
      target: detail['target']!.as<String>(),
    );
  }

  Future<()> invokeRevealStorageItem(
    String target,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._invoke('reveal_storage_item', {
      'target': target,
    });
    return (
    );
  }

  Future<({List<String> target})> invokePickStorageItem(
    String  type,
    Boolean multiply,
    String  location,
    String  name,
  ) async {
    var detail = await this._invoke('pick_storage_item', {
      'type': type,
      'multiply': multiply,
      'location': location,
      'name': name,
    });
    return (
      target: detail['target']!.as<List<Object?>>().cast<String>(),
    );
  }

  // ----------------

  Future<()> invokePushSystemNotification(
    String title,
    String description,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._invoke('push_system_notification', {
      'title': title,
      'description': description,
    });
    return (
    );
  }

  // ----------------

  Future<({String destination})> invokeOnWindowsQueryStorageLongPath(
    String source,
  ) async {
    assertTest(SystemChecker.isWindows);
    var detail = await this._invoke('on_windows_query_storage_long_path', {
      'source': source,
    });
    return (
      destination: detail['destination']!.as<String>(),
    );
  }

  // #endregion

  // #region utility

  Stream<String> getStreamForLink(
  ) {
    return this._streamControllerForLink.stream;
  }

  // #endregion

}
