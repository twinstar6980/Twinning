// ignore_for_file: unused_import

import 'dart:core' as core;
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

typedef Void = void;
typedef Object = core.Object;
typedef Boolean = core.bool;
typedef Integer = core.int;
typedef Floating = core.double;
typedef String = core.String;
typedef List<E> = core.List<E>;
typedef Set<E> = core.Set<E>;
typedef Map<K, V> = core.Map<K, V>;

// ----------------

const kApplicationName = 'TwinStar ToolKit - Shell GUI';

const kApplicationVersion = 3;

// ----------------

Void
exitApp(
) {
  if (Platform.isAndroid) {
    SystemNavigator.pop();
  } else {
    exit(0);
  }
}

// ----------------

String
normalizeLibraryPath(
  String path,
) {
  if (Platform.isWindows) {
    return '${path}.';
  } else {
    return '${path}';
  }
}

extension BooleanStringExtension on Boolean {

  static
  Boolean
  parse(
    String name,
  ) {
    late Boolean value;
    switch (name) {
      case 'false': {
        value = false;
        break;
      }
      case 'true': {
        value = true;
        break;
      }
      default: {
        throw core.Exception('invalid boolean string');
      }
    }
    return value;
  }

}
