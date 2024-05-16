import 'dart:core' as core;
import 'package:material_symbols_icons/material_symbols_icons.dart';

// ----------------

typedef Void = void;
typedef Object = core.Object;
typedef Boolean = core.bool;
typedef Integer = core.int;
typedef Floater = core.double;
typedef String = core.String;
typedef List<E> = core.List<E>;
typedef Map<K, V> = core.Map<K, V>;

typedef IconSymbols = Symbols;

// ----------------

const String kApplicationName = 'TwinStar ToolKit - Assistant';

const String kApplicationVersion = '35';

// ----------------

Void assertTest(
  Boolean condition,
) {
  if (!condition) {
    throw core.AssertionError();
  }
  return;
}
