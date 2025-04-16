import '/common.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';

// ----------------

class NotificationHelper {

  // #region interface

  static FlutterLocalNotificationsPlugin? _flutterLocalNotificationsPlugin = null;

  // ----------------

  static Future<Void> initialize(
  ) async {
    _flutterLocalNotificationsPlugin = FlutterLocalNotificationsPlugin();
    await _flutterLocalNotificationsPlugin!.initialize(
      InitializationSettings(
        android: const AndroidInitializationSettings(
          '@mipmap/ic_launcher_foreground',
        ),
        iOS: const DarwinInitializationSettings(
        ),
        macOS: const DarwinInitializationSettings(
        ),
        linux: LinuxInitializationSettings(
          defaultActionName: '',
          defaultIcon: AssetsLinuxIcon('asset/logo.png'),
        ),
        windows: WindowsInitializationSettings(
          appName: kApplicationName,
          appUserModelId: 'TwinStar.Twinning.Assistant.Notification',
          guid: '3FCD5C89-78F3-489B-88E5-37CBC3C3FC1A',
          iconPath: 'asset/logo.png',
        ),
      ),
      onDidReceiveNotificationResponse: null,
    );
    return;
  }

  static Future<Void> push(
    String title,
    String description,
  ) async {
    await _flutterLocalNotificationsPlugin!.show(
      DateTime.now().millisecondsSinceEpoch % 0x80000000,
      title,
      description,
      const NotificationDetails(
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
