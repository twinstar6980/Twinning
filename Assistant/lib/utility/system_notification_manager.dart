import '/common.dart';
import '/utility/application_platform_method.dart';

// ----------------

class SystemNotificationManager {

  // #region singleton

  static final SystemNotificationManager instance = ._();

  // #endregion

  // #region constructor

  // ----------------

  SystemNotificationManager._(
  );

  // #endregion

  // #region utility

  Future<Void> push(
    String title,
    String description,
  ) async {
    await ApplicationPlatformMethod.instance.pushSystemNotification(title, description);
    return;
  }

  // #endregion

}
