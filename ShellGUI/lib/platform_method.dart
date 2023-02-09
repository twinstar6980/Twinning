// ignore_for_file: unused_import

import '/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

final MethodChannel? gPlatformMethod = () {
  if (Platform.isAndroid) {
    return const MethodChannel('com.twinstar.toolkit.shell_gui.flutter_method_channel');
  }
  return null;
}();
