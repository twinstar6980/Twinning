import '/common.dart';
import 'package:app_links/app_links.dart' as lib;

// ----------------

class CustomLinkHelper {

  // #region utility

  static Future<Void> listen(
    Void Function(Uri) handler,
  ) async {
    lib.AppLinks().uriLinkStream.listen((link) {
      handler(link);
      return;
    });
    return;
  }

  // ----------------

  static Future<Uri?> getFirst(
  ) async {
    return await lib.AppLinks().getInitialLink();
  }

  static Future<Uri?> getLast(
  ) async {
    return await lib.AppLinks().getLatestLink();
  }

  // #endregion

}
