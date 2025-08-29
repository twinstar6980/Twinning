import '/common.dart'; // ignore: unused_import
import 'package:flutter/widgets.dart';
import 'package:material_symbols_icons/get.dart';

// ----------------

class IconHelper {

  // #region utility

  static IconData queryOutlined(
    String name,
  ) {
    return SymbolsGet.get(name, SymbolStyle.outlined);
  }

  // #endregion

}
