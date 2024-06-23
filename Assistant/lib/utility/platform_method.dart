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

  static Future<String?> pickStorageItem(
    String type,
    String initialDirectory,
  ) async {
    assertTest(Platform.isAndroid || Platform.isIOS);
    var result = (await _channel!.invokeMethod('pick_storage_item', <dynamic, dynamic>{ 'type': type, 'initial_directory': initialDirectory }) as String?);
    return result;
  }

  static Future<String?> copyStorageFile(
    String source,
    String placement,
  ) async {
    assertTest(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('copy_storage_file', <dynamic, dynamic>{ 'source': source, 'placement': placement }) as String);
    return result;
  }

  static Future<Boolean> checkExternalStoragePermission(
    String mode,
  ) async {
    assertTest(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('check_external_storage_permission', <dynamic, dynamic>{ 'mode': mode }) as Boolean);
    return result;
  }

  static Future<String> queryExternalStoragePath(
  ) async {
    assertTest(Platform.isAndroid);
    var result = (await _channel!.invokeMethod('query_external_storage_path', <dynamic, dynamic>{}) as String);
    return result;
  }

  // #endregion

}
