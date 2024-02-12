import 'dart:core' as core;
import 'dart:io';
import 'dart:async';
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

const String kApplicationName = 'TwinStar ToolKit - Shell GUI';

const Integer kApplicationVersion = 33;

// ----------------

Void assertAlways(
  Boolean condition,
) {
  if (!condition) {
    throw core.AssertionError();
  }
  return;
}

// ----------------

Future<String> queryApplicationSharedDirectory(
) async {
  var result = null as String?;
  if (Platform.isWindows) {
    result = (await getApplicationSupportDirectory()).path;
    result = normalizePath(result);
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

String normalizePath(
  String source,
) {
  return source.replaceAll('\\', '/');
}

// ----------------

String convertStringListToTextWithLine(
  List<String> value
) {
  return value.join('\n') + (value.isNotEmpty && value.last.isEmpty ? '\n' : '');
}

List<String> convertStringListFromTextWithLine(
  String text
) {
  var value = text.split('\n').map((value) => (!value.endsWith('\r') ? value : value.substring(0, value.length - 1))).toList();
  if (value.isNotEmpty && value.last.isEmpty) {
    value.removeLast();
  }
  return value;
}

// ----------------

enum FileObjectType {
  any,
  file,
  directory,
  ;

  static FileObjectType formString(
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

  static FileRequestRule formString(
    String name,
  ) {
    return values.firstWhere((element) => (element.name == name));
  }

}

// ----------------

extension BooleanStringExtension on Boolean {

  static Boolean parse(
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
