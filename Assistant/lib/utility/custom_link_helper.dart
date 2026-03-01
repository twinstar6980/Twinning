import '/common.dart';
import 'package:app_links/app_links.dart' as lib;

// ----------------

class CustomLinkHelper {

  // #region utility

  static Boolean _initialized = false;

  static lib.AppLinks? _plugin = null;

  // ----------------

  static Future<Void> initialize(
  ) async {
    assertTest(!_initialized);
    _plugin = lib.AppLinks();
    _initialized = true;
    return;
  }

  static Future<Void> listen(
    Void Function(Uri link) handler,
  ) async {
    assertTest(_initialized);
    _plugin!.uriLinkStream.listen((link) {
      handler(link);
      return;
    });
    return;
  }

  // ----------------

  static Future<Uri?> getFirst(
  ) async {
    assertTest(_initialized);
    return await _plugin!.getInitialLink();
  }

  static Future<Uri?> getLast(
  ) async {
    assertTest(_initialized);
    return await _plugin!.getLatestLink();
  }

  // #endregion

}
