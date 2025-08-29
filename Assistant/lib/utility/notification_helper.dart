import '/common.dart';
import '/utility/window_helper.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';

// ----------------

class NotificationHelper {

  // #region utility

  static FlutterLocalNotificationsPlugin? _plugin = null;

  // ----------------

  static Future<Void> initialize(
  ) async {
    _plugin = FlutterLocalNotificationsPlugin();
    await _plugin!.initialize(
      InitializationSettings(
        android: AndroidInitializationSettings(
          '@mipmap/ic_launcher_foreground',
        ),
        iOS: DarwinInitializationSettings(
        ),
        macOS: DarwinInitializationSettings(
        ),
        linux: LinuxInitializationSettings(
          defaultActionName: '',
          defaultIcon: AssetsLinuxIcon('asset/logo.png'),
        ),
        windows: WindowsInitializationSettings(
          appName: ApplicationInformation.name,
          appUserModelId: 'TwinStar.Twinning.Assistant.Notification',
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
    await _plugin!.show(
      DateTime.now().millisecondsSinceEpoch % 0x80000000,
      title,
      description,
      NotificationDetails(
        android: AndroidNotificationDetails(
          'com.twinstar.twinning.assistant.notification_channel.main',
          'Main',
        ),
        iOS: DarwinNotificationDetails(
        ),
        macOS: DarwinNotificationDetails(
        ),
        linux: LinuxNotificationDetails(
        ),
        windows: WindowsNotificationDetails(
        ),
      ),
    );
    return;
  }

  // #endregion

}
