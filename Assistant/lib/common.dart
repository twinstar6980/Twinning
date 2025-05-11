import 'dart:core' as core;
import 'package:material_symbols_icons/material_symbols_icons.dart';

// ----------------

typedef Void = void;
typedef Boolean = core.bool;
typedef Integer = core.int;
typedef Floater = core.double;
typedef String = core.String;

typedef IconSymbols = Symbols;

// ----------------

const String kApplicationName = 'Twinning Assistant';

const String kApplicationVersion = '69';

// ----------------

Void assertTest(
  Boolean condition,
) {
  if (!condition) {
    throw core.AssertionError();
  }
  return;
}

extension CommonObjectExtension<TType extends core.Object> on TType {

  TTarget as<TTarget>(
  ) {
    return this as TTarget;
  }

  // ----------------

  TResult selfLet<TResult>(
    TResult Function(TType) action,
  ) {
    return action(this);
  }

  TType selfAlso(
    Void Function(TType) action,
  ) {
    action(this);
    return this;
  }

}
