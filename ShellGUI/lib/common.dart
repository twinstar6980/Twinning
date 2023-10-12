import 'dart:core' as core;
import 'dart:io';
import 'dart:async';
import 'package:flutter/services.dart';
import 'package:path_provider/path_provider.dart';

// ----------------

typedef Void = void;
typedef Object = core.Object;
typedef Boolean = core.bool;
typedef Integer = core.int;
typedef Floater = core.double;
typedef String = core.String;
typedef List<E> = core.List<E>;
typedef Map<K, V> = core.Map<K, V>;

// ----------------

const kApplicationName = 'TwinStar ToolKit - Shell GUI';

const kApplicationVersion = 26;

// ----------------

List<String> gPrimaryFontFamliy = [];

List<String> gConsoleFontFamliy = [];

// ----------------

Void
exitApplication(
) {
  if (Platform.isAndroid) {
    SystemNavigator.pop();
  } else {
    exit(0);
  }
}

// ----------------

Future<String>
queryApplicationSharedDirectory(
) async {
  var result = null as String?;
  if (Platform.isWindows) {
    result = (await getApplicationSupportDirectory()).path.replaceAll('\\', '/');
  }
  if (Platform.isLinux) {
    result = (await getApplicationSupportDirectory()).path;
  }
  if (Platform.isMacOS) {
    result = (await getApplicationSupportDirectory()).path;
  }
  if (Platform.isAndroid) {
    result = (await getExternalStorageDirectory())!.path;
  }
  if (Platform.isIOS) {
    result = (await getApplicationDocumentsDirectory()).path;
  }
  return result!;
}

// ----------------

String
normalizeLibraryPath(
  String path,
) {
  var result = path;
  if (Platform.isWindows) {
    result += '.';
  }
  return result;
}

// ----------------

enum FileObjectType {
  any,
  file,
  directory,
  ;

  static
  FileObjectType
  formString(
    String name,
  ) {
    return values.firstWhere((element) => (element.name == name));
  }

}

enum FileRequestRule {
  any,
  input,
  output,
  ;

  static
  FileRequestRule
  formString(
    String name,
  ) {
    return values.firstWhere((element) => (element.name == name));
  }

}

// ----------------

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
