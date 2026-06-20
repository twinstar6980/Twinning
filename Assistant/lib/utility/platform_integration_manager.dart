import '/common.dart';
import '/widget/container.dart';
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
          getArgument('target')!.as<String>(),
        );
        break;
      }
      case 'receive_application_drag_enter': {
        // ignore: unused_local_variable
        var detail = await this._handleReceiveApplicationDragEnter(
        );
        break;
      }
      case 'receive_application_drag_over': {
        // ignore: unused_local_variable
        var detail = await this._handleReceiveApplicationDragOver(
          getArgument('location_x')!.as<Integer>(),
          getArgument('location_y')!.as<Integer>(),
        );
        break;
      }
      case 'receive_application_drag_leave': {
        // ignore: unused_local_variable
        var detail = await this._handleReceiveApplicationDragLeave(
        );
        break;
      }
      case 'receive_application_drag_drop': {
        // ignore: unused_local_variable
        var detail = await this._handleReceiveApplicationDragDrop(
          getArgument('target')!.as<List<Object?>>().cast<String>(),
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

  // ----------------

  Future<()> _handleReceiveApplicationDragEnter(
  ) async {
    await DropRegionManager.instance.onEnter();
    return (
    );
  }

  Future<()> _handleReceiveApplicationDragOver(
    Integer locationX,
    Integer locationY,
  ) async {
    await DropRegionManager.instance.onOver(.new(locationX.toDouble(), locationY.toDouble()));
    return (
    );
  }

  Future<()> _handleReceiveApplicationDragLeave(
  ) async {
    await DropRegionManager.instance.onLeave();
    return (
    );
  }

  Future<()> _handleReceiveApplicationDragDrop(
    List<String> target,
  ) async {
    await DropRegionManager.instance.onDrop(target);
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

  Future<({Integer? accent})> invokeQuerySystemTheme(
  ) async {
    var detail = await this._invoke('query_system_theme', {
    });
    return (
      accent: detail['accent']?.as<Integer>(),
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

  Future<({Integer x, Integer y, Integer width, Integer height})> invokeQueryScreenPlacement(
  ) async {
    var detail = await this._invoke('query_screen_placement', {
    });
    return (
      x: detail['x']!.as<Integer>(),
      y: detail['y']!.as<Integer>(),
      width: detail['width']!.as<Integer>(),
      height: detail['height']!.as<Integer>(),
    );
  }

  Future<({Integer x, Integer y, Integer width, Integer height})> invokeQueryWindowPlacement(
  ) async {
    var detail = await this._invoke('query_window_placement', {
    });
    return (
      x: detail['x']!.as<Integer>(),
      y: detail['y']!.as<Integer>(),
      width: detail['width']!.as<Integer>(),
      height: detail['height']!.as<Integer>(),
    );
  }

  Future<()> invokeUpdateWindowPlacement(
    Integer x,
    Integer y,
    Integer width,
    Integer height,
  ) async {
    // ignore: unused_local_variable
    var detail = await this._invoke('update_window_placement', {
      'x': x,
      'y': y,
      'width': width,
      'height': height,
    });
    return (
    );
  }

  // ----------------

  Future<({Integer width, Integer height, Uint8List data})> invokeOnWindowsExtractAssociatedIcon(
    String target,
  ) async {
    assertTest(SystemChecker.isWindows);
    var detail = await this._invoke('on_windows_extract_associated_icon', {
      'target': target,
    });
    return (
      width: detail['width']!.as<Integer>(),
      height: detail['height']!.as<Integer>(),
      data: detail['data']!.as<Uint8List>(),
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
