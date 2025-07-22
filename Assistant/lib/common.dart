import 'dart:core' as core;
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';

// ----------------

typedef Void = void;
typedef Boolean = core.bool;
typedef Integer = core.int;
typedef Floater = core.double;
typedef String = core.String;

typedef IconSymbols = Symbols;

// ----------------

final class ApplicationInformation {

  static const String name = 'Twinning Assistant';

  static const String version = '81';

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

// ----------------

core.Future<Void> refreshState(
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
