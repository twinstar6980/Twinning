import '/common.dart';
import 'dart:io';
import 'package:local_notifier/local_notifier.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'package:window_manager/window_manager.dart';

// ----------------

class NotificationHelper {

  // #region interface

  static FlutterLocalNotificationsPlugin? _flutterLocalNotificationsPlugin = null;

  // ----------------

  static Future<Void> initialize(
  ) async {
    if (Platform.isWindows) {
      await localNotifier.setup(
        appName: kApplicationName,
        shortcutPolicy: ShortcutPolicy.requireCreate,
      );
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
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
        ),
        onDidReceiveNotificationResponse: null,
      );
    }
    return;
  }

  static Future<Void> push(
    String title,
    String description,
  ) async {
    if (Platform.isWindows) {
      var notification = LocalNotification(
        title: title,
        body: description,
      );
      notification.onClick = () async {
        await windowManager.focus();
      };
      await notification.show();
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
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
        ),
      );
    }
    return;
  }

  // #endregion

}
