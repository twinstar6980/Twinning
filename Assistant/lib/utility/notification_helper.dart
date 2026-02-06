import '/common.dart';
import '/utility/window_helper.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart' as lib;

// ----------------

class NotificationHelper {

  // #region utility

  static lib.FlutterLocalNotificationsPlugin? _plugin = null;

  // ----------------

  static Future<Void> initialize(
  ) async {
    assertTest(_plugin == null);
    _plugin = .new();
    await _plugin!.initialize(
      settings: .new(
        android: .new(
          '@mipmap/ic_launcher_foreground',
        ),
        iOS: .new(
        ),
        macOS: .new(
        ),
        linux: .new(
          defaultActionName: '',
          defaultIcon: lib.AssetsLinuxIcon('asset/logo.png'),
        ),
        windows: .new(
          appName: ApplicationInformation.name,
          appUserModelId: ApplicationInformation.identifier,
          guid: '3FCD5C89-78F3-489B-88E5-37CBC3C3FC1A',
          iconPath: 'asset/logo.png',
        ),
      ),
      onDidReceiveNotificationResponse: (details) async {
        if (SystemChecker.isWindows || SystemChecker.isLinux || SystemChecker.isMacintosh) {
          await WindowHelper.show();
        }
        return;
      },
    );
    return;
  }

  static Future<Void> push(
    String title,
    String description,
  ) async {
    assertTest(_plugin != null);
    await _plugin!.show(
      id: DateTime.now().millisecondsSinceEpoch % 0x80000000,
      title: title,
      body: description,
      notificationDetails: .new(
        android: .new('${ApplicationInformation.identifier}.notification_channel.main', 'Main'),
        iOS: .new(),
        macOS: .new(),
        linux: .new(),
        windows: .new(),
      ),
    );
    return;
  }

  // #endregion

}
