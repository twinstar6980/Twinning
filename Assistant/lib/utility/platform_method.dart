import '/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

class PlatformMethod {

  // #region channel

  static final MethodChannel? _channel = () {
    if (Platform.isAndroid || Platform.isIOS) {
      return const MethodChannel('com.twinstar.toolkit.assistant.CustomMethodChannel');
    }
    return null;
  }();

  // #endregion

  // #region interface

  static Future<Boolean> checkStoragePermission(
    String mode,
  ) async {
    assertTest(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('check_storage_permission', <String, dynamic>{ 'mode': mode }) as Boolean);
    return result;
  }

  static Future<String?> pickStoragePath(
    String type,
    String initialDirectory,
    String fallbackDirectory,
  ) async {
    assertTest(Platform.isAndroid || Platform.isIOS);
    var result = (await _channel!.invokeMethod('pick_storage_path', <String, dynamic>{ 'type': type, 'initial_directory': initialDirectory, 'fallback_directory': fallbackDirectory }) as String?);
    return result;
  }

  static Future<List<String>> getLinkCommand(
    String fallbackDirectory,
  ) async {
    assertTest(Platform.isAndroid || Platform.isIOS);
    var result = (await _channel!.invokeMethod('get_link_command', <String, dynamic>{ 'fallback_directory': fallbackDirectory }) as List<Object?>).cast<String>();
    return result;
  }

  // #endregion

}
