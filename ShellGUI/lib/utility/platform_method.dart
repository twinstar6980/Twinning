import '/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

class PlatformMethod {

  static final MethodChannel? _channel = () {
    if (Platform.isAndroid || Platform.isIOS) {
      return const MethodChannel('com.twinstar.toolkit.shell_gui.CustomMethodChannel');
    }
    return null;
  }();

  // ----------------

  static Future<Boolean> checkStoragePermission(
  ) async {
    assertAlways(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('checkStoragePermission', <String, dynamic>{}) as Boolean);
    return result;
  }

  static Future<Boolean> requestStoragePermission(
  ) async {
    assertAlways(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('requestStoragePermission', <String, dynamic>{}) as Boolean);
    return result;
  }

  static Future<String?> pickPath(
    Boolean isDirectory,
    String  fallbackDirectory,
  ) async {
    assertAlways(Platform.isAndroid || Platform.isIOS);
    var result = (await _channel!.invokeMethod('pickPath', <String, dynamic>{ 'isDirectory': isDirectory, 'fallbackDirectory': fallbackDirectory }) as String?);
    return result;
  }

  static Future<List<String>> getCommand(
    String fallbackDirectory,
  ) async {
    assertAlways(Platform.isAndroid || Platform.isIOS);
    var result = (await _channel!.invokeMethod('getCommand', <String, dynamic>{ 'fallbackDirectory': fallbackDirectory }) as List<Object?>).cast<String>();
    return result;
  }

}
