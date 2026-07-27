import '/common.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import 'package:flutter/services.dart' as lib;

// ----------------

class ApplicationFontManager {

  // #region singleton

  static final ApplicationFontManager instance = ._();

  // #endregion

  // #region constructor

  List<StoragePath> _loadedFile;

  // ----------------

  ApplicationFontManager._(
  ) :
    this._loadedFile = [];

  // #endregion

  // #region utility

  Future<String?> loadFile(
    StoragePath path,
  ) async {
    try {
      var index = this._loadedFile.indexOf(path);
      if (index == -1) {
        index = this._loadedFile.length;
        var loader = lib.FontLoader('_custom_${index + 1}');
        loader.addFont(Future.sync(() async => (await StorageHelper.readFileData(path)).buffer.asByteData()));
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
