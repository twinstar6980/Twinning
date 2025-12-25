import 'dart:core' as lib;
import 'dart:core' hide Object, bool, int, double, String;
import 'dart:io';

// ----------------

typedef Void = void;

typedef Object = lib.Object;

typedef Boolean = lib.bool;

typedef Integer = lib.int;

typedef Floater = lib.double;

typedef String = lib.String;

// ----------------

final class ApplicationInformation {

  static const String identifier = 'com.twinstar.twinning.assistant';

  static const String version = '134';

  static const String name = 'Twinning Assistant';

  static const String developer = 'TwinStar';

  static const String year = '2023-2025';

}

final class SystemChecker {

  static final Boolean isWindows = Platform.isWindows;

  static final Boolean isLinux = Platform.isLinux;

  static final Boolean isMacintosh = Platform.isMacOS;

  static final Boolean isAndroid = Platform.isAndroid;

  static final Boolean isIphone = Platform.isIOS;

}

// ----------------

class UnreachableException implements Exception {

  // #region constructor

  // ----------------

  UnreachableException(
  );

  // #endregion

  // #region string

  @override
  toString(
  ) {
    return 'UnreachableException';
  }

  // #endregion

}

class UnimplementedException implements Exception {

  // #region constructor

  // ----------------

  UnimplementedException(
  );

  // #endregion

  // #region string

  @override
  toString(
  ) {
    return 'UnimplementedException';
  }

  // #endregion

}

class UnsupportedException implements Exception {

  // #region constructor

  // ----------------

  UnsupportedException(
  );

  // #endregion

  // #region string

  @override
  toString(
  ) {
    return 'UnsupportedException';
  }

  // #endregion

}

class AssertionException implements Exception {

  // #region constructor

  final String? expression;

  // ----------------

  AssertionException(
    String? expression,
  ) :
    this.expression = expression;

  // #endregion

  // #region string

  @override
  toString(
  ) {
    return 'AssertionException${this.expression == null ? '' : ': ${this.expression}'}';
  }

  // #endregion

}

Void assertTest(
  Boolean condition, {
  String? expression = null,
}) {
  if (!condition) {
    throw AssertionException(expression);
  }
  return;
}

// ----------------

extension CommonObjectExtension<TType extends Object> on TType {

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

  // ----------------

  TTarget as<TTarget>(
  ) {
    return this as TTarget;
  }

}
