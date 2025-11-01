import '/common.dart'; // ignore: unused_import
import 'package:flutter/widgets.dart';
import 'package:material_symbols_icons/get.dart' as lib;

// ----------------

class IconHelper {

  // #region utility

  static IconData queryOutlined(
    String name,
  ) {
    return lib.SymbolsGet.get(name, lib.SymbolStyle.outlined);
  }

  // #endregion

}
