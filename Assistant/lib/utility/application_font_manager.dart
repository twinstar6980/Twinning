import '/common.dart';
import '/utility/storage_helper.dart';
import 'package:flutter/services.dart';

// ----------------

class ApplicationFontManager {

  // #region singleton

  static final ApplicationFontManager instance = ._();

  // #endregion

  // #region constructor

  List<String> _loadedFile;

  // ----------------

  ApplicationFontManager._(
  ) :
    this._loadedFile = [];

  // #endregion

  // #region utility

  Future<String?> loadFile(
    String path,
  ) async {
    try {
      var index = this._loadedFile.indexOf(path);
      if (index == -1) {
        index = this._loadedFile.length;
        var loader = FontLoader('_custom_${index + 1}');
        loader.addFont(Future.sync(() async => (await StorageHelper.readFile(path)).buffer.asByteData()));
        await loader.load();
        this._loadedFile.add(path);
      }
      return '_custom_${index + 1}';
    }
    catch (e) {
      return null;
    }
  }

  // #endregion

}
