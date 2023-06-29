// ignore_for_file: unused_import

import '/common.dart';
import 'dart:io';
import '/common/platform_method.dart';

// ----------------

class PermissionHelper {

  static
  Future<Boolean>
  checkStoragePermission(
  ) async {
    var result = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = true;
    }
    if (Platform.isAndroid) {
      result = (await PlatformMethod.checkStoragePermission());
    }
    if (Platform.isIOS) {
      result = true;
    }
    return result;
  }

  static
  Future<Boolean>
  requestStoragePermission(
  ) async {
    var result = true;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      result = true;
    }
    if (Platform.isAndroid) {
      result = (await PlatformMethod.requestStoragePermission());
    }
    if (Platform.isIOS) {
      result = true;
    }
    return result;
  }

}
