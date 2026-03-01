import '/common.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart' as lib;
import 'package:material_symbols_icons/material_symbols_icons.dart' as lib;
import 'package:material_symbols_icons/get.dart' as lib;

// ----------------

// #region basic

Future<Void> refreshState(
  Void Function(Void Function() action) setState,
) async {
  setState(() {});
  await WidgetsBinding.instance.endOfFrame;
  return;
}

Future<Void> postTask(
  Future<Void> Function() action,
) async {
  await WidgetsBinding.instance.endOfFrame;
  await action();
  return;
}

// #endregion

// #region color

typedef Color = lib.Color;

typedef ColorSet = lib.Colors;

// #endregion

// #region icon

typedef IconSet = lib.Symbols;

class IconSetExtension {

  static IconData queryOutlined(
    String name,
  ) {
    return lib.SymbolsGet.get(name, .outlined);
  }

}

// #endregion
