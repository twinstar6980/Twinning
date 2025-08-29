import '/common.dart';
import 'package:app_links/app_links.dart';

// ----------------

class CustomLinkHelper {

  // #region utility

  static Future<Void> listen(
    Void Function(Uri) handler,
  ) async {
    AppLinks().uriLinkStream.listen((link) {
      handler(link);
    });
    return;
  }

  // ----------------

  static Future<Uri?> getFirst(
  ) async {
    return await AppLinks().getInitialLink();
  }

  static Future<Uri?> getLast(
  ) async {
    return await AppLinks().getLatestLink();
  }

  // #endregion

}
