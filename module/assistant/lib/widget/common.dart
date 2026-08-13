import '/common.dart';
import 'package:provider/provider.dart' as lib;
import 'package:flutter/widgets.dart' as lib;
import 'package:material_ui/material_ui.dart' as material;
import 'package:material_symbols_icons/material_symbols_icons.dart' as lib;
import 'package:material_symbols_icons/get.dart' as lib;

// ----------------

// #region type

// TODO

typedef BuildContext = lib.BuildContext;

typedef State<T extends lib.StatefulWidget> = lib.State<T>;

typedef Key = lib.Key;
typedef ObjectKey = lib.ObjectKey;
typedef GlobalKey<T extends lib.State<lib.StatefulWidget>> = lib.GlobalKey<T>;

typedef Widget = lib.Widget;
typedef StatelessWidget = lib.StatelessWidget;
typedef StatefulWidget = lib.StatefulWidget;

typedef TickerProviderStateMixin<T extends lib.StatefulWidget> = lib.TickerProviderStateMixin<T>;

typedef Builder = lib.Builder;
typedef StatefulBuilder = lib.StatefulBuilder;
typedef LayoutBuilder = lib.LayoutBuilder;
typedef FutureBuilder<T> = lib.FutureBuilder<T>;
typedef StreamBuilder<T> = lib.StreamBuilder<T>;
typedef AnimatedBuilder = lib.AnimatedBuilder;

typedef ChangeNotifier = lib.ChangeNotifier;
typedef ChangeNotifierProvider<T extends ChangeNotifier?> = lib.ChangeNotifierProvider<T>;
typedef Consumer<T> = lib.Consumer<T>;
typedef Provider<T> = lib.Provider<T>;

typedef ScrollController = lib.ScrollController;
typedef TextEditingController = lib.TextEditingController;
typedef AnimationController = lib.AnimationController;

typedef Navigator = lib.Navigator;
typedef NavigatorState = lib.NavigatorState;

typedef MediaQuery = lib.MediaQuery;
typedef MediaQueryData = lib.MediaQueryData;

typedef TextStyle = lib.TextStyle;
typedef TextBaseline = lib.TextBaseline;
typedef TextAlign = lib.TextAlign;

typedef Offset = lib.Offset;
typedef Matrix4 = lib.Matrix4;
typedef BoxConstraints = lib.BoxConstraints;
typedef EdgeInsetsGeometry = lib.EdgeInsetsGeometry;
typedef BorderRadiusGeometry = lib.BorderRadiusGeometry;

typedef Animation<T> = lib.Animation<T>;
typedef Animatable<T> = lib.Animatable<T>;

// #endregion

// #region basic

Future<Void> refreshState(
  Void Function(Void Function() action)? setState,
) async {
  setState?.call(() {});
  await lib.WidgetsBinding.instance.endOfFrame;
  return;
}

Future<Void> postTask(
  Future<Void> Function() action,
) async {
  await lib.WidgetsBinding.instance.endOfFrame;
  await action();
  return;
}

// #endregion

// #region color

typedef Color = lib.Color;

typedef ColorFilter = lib.ColorFilter;

typedef ColorSet = material.Colors;

// #endregion

// #region icon

typedef IconData = lib.IconData;

typedef IconSet = lib.Symbols;

extension IconSetExtension on IconSet {

  static IconData queryOutlined(
    String name,
  ) {
    return lib.SymbolsGet.get(name, .outlined);
  }

}

// #endregion
