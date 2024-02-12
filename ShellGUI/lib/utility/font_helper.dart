import '/common.dart';
import 'dart:io';
import 'package:flutter/services.dart';

// ----------------

class FontHelper {

  static List<String> _cachedFilePath = [];

  // ----------------

  static Future<String?> loadFile(
    String path,
  ) async {
    var index = _cachedFilePath.indexOf(path);
    if (index == -1) {
      index = _cachedFilePath.length;
      try {
        var loader = FontLoader('_custom_${index + 1}');
        loader.addFont(Future.sync(() async => ((await File(path).readAsBytes()).buffer.asByteData())));
        await loader.load();
        _cachedFilePath.add(path);
      }
      catch (e) {
        return null;
      }
    }
    return '_custom_${index + 1}';
  }

}
