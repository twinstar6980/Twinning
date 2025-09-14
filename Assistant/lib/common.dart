import 'dart:core';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';

// ----------------

typedef Void = void;

typedef Boolean = bool;

typedef Integer = int;

typedef Floater = double;

// ----------------

final class ApplicationInformation {

  static const String name = 'Twinning Assistant';

  static const String version = '112';

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
  String toString(
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
  String toString(
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
  String toString(
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
  String toString(
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

// ----------------

Future<Void> refreshState(
  Void Function(Void Function()) setState,
) async {
  setState(() {});
  await WidgetsBinding.instance.endOfFrame;
  return;
}

extension CommonWidgetExtension<TType extends Widget> on TType {

  Widget withExpanded({
    Integer? flex = null,
  }) {
    flex ??= 1;
    return Expanded(
      flex: flex,
      child: this,
    );
  }

  SelectionArea withSelectionArea({
    ValueChanged<SelectedContent?>? onSelectionChanged = null,
  }) {
    return SelectionArea(
      onSelectionChanged: onSelectionChanged,
      child: this,
    );
  }

  Scrollbar withScrollbar({
    Boolean?          interactive = null,
    ScrollController? controller = null,
  }) {
    interactive ??= true;
    return Scrollbar(
      interactive: interactive,
      controller: controller,
      child: this,
    );
  }

  SingleChildScrollView withSingleChildScrollView({
    Axis?             scrollDirection = null,
    ScrollController? controller      = null,
  }) {
    scrollDirection ??= Axis.vertical;
    return SingleChildScrollView(
      scrollDirection: scrollDirection,
      controller: controller,
      child: this,
    );
  }

}

typedef IconSymbols = Symbols;
