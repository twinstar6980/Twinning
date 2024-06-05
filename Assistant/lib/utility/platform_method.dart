import '/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

class PlatformMethod {

  // #region channel

  static final MethodChannel? _channel = () {
    if (Platform.isAndroid || Platform.isIOS) {
      return const MethodChannel('com.twinstar.twinning.assistant.CustomMethodChannel');
    }
    return null;
  }();

  // #endregion

  // #region interface

  static Future<String?> pickStoragePath(
    String type,
    String initialDirectory,
    String fallbackDirectory,
  ) async {
    assertTest(Platform.isAndroid || Platform.isIOS);
    var result = (await _channel!.invokeMethod('pick_storage_path', <String, dynamic>{ 'type': type, 'initial_directory': initialDirectory, 'fallback_directory': fallbackDirectory }) as String?);
    return result;
  }

  static Future<String?> parseContentUri(
    String target,
    String fallbackDirectory,
  ) async {
    assertTest(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('parse_content_uri', <String, dynamic>{ 'target': target, 'fallback_directory': fallbackDirectory }) as String?);
    return result;
  }

  static Future<Boolean> checkStoragePermission(
    String mode,
  ) async {
    assertTest(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('check_storage_permission', <String, dynamic>{ 'mode': mode }) as Boolean);
    return result;
  }

  // #endregion

}
