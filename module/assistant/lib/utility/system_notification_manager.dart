import '/common.dart';
import '/utility/platform_integration_manager.dart';

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
    await PlatformIntegrationManager.instance.invokePushSystemNotification(title, description);
    return;
  }

  // #endregion

}
