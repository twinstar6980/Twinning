import '/common.dart';
import '/utility/storage_helper.dart';
import 'package:flutter/services.dart';

// ----------------

class FontHelper {

  // #region load

  static final List<String> _loadedFilePath = [];

  // ----------------

  static Future<String?> loadFile(
    String path,
  ) async {
    try {
      var index = _loadedFilePath.indexOf(path);
      if (index == -1) {
        index = _loadedFilePath.length;
        var loader = FontLoader('_custom_${index + 1}');
        loader.addFont(Future.sync(() async => (await StorageHelper.readFile(path)).buffer.asByteData()));
        await loader.load();
        _loadedFilePath.add(path);
      }
      return '_custom_${index + 1}';
    }
    catch (e) {
      return null;
    }
  }

  // #endregion

}
