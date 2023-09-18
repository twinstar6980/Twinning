import '/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

class PlatformMethod {

  static MethodChannel? _channel = () {
    if (Platform.isAndroid) {
      return const MethodChannel('com.twinstar.toolkit.shell_gui.flutter_method_channel');
    }
    return null;
  }();

  // ----------------

  static
  Future<Boolean>
  checkStoragePermission(
  ) async {
    var result = (await _channel!.invokeMethod('checkStoragePermission', <String, dynamic>{}) as Boolean);
    return result;
  }

  static
  Future<Boolean>
  requestStoragePermission(
  ) async {
    var result = (await _channel!.invokeMethod('requestStoragePermission', <String, dynamic>{}) as Boolean);
    return result;
  }

  static
  Future<List<String>>
  getCommand(
    String? fallbackDirectory,
  ) async {
    var result = (await _channel!.invokeMethod('getCommand', <String, dynamic>{ 'fallbackDirectory': fallbackDirectory }) as List<Object?>).cast<String>();
    return result;
  }

  static
  Future<String?>
  pickPath(
    Boolean typeIsDirectory,
    String? fallbackDirectory,
  ) async {
    var result = (await _channel!.invokeMethod('pickPath', <String, dynamic>{ 'typeIsDirectory': typeIsDirectory, 'fallbackDirectory': fallbackDirectory }) as String?);
    return result;
  }

}
