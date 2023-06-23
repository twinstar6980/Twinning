// ignore_for_file: unused_import

import '/common.dart';
import 'dart:io';
import 'package:flutter/widgets.dart';
import 'package:window_manager/window_manager.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'package:local_notifier/local_notifier.dart';
import '/application.dart';
import '/platform_method.dart';

// ----------------

class NotificationHelper {

  static FlutterLocalNotificationsPlugin? _flutterLocalNotificationsPlugin;

  static
  Future<Void>
  initialize(
  ) async {
    if (Platform.isWindows) {
      await localNotifier.setup(
        appName: kApplicationName,
        shortcutPolicy: ShortcutPolicy.requireCreate,
      );
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      _flutterLocalNotificationsPlugin = FlutterLocalNotificationsPlugin();
      var initializationSettings = InitializationSettings(
        android: const AndroidInitializationSettings('@mipmap/ic_launcher'),
        iOS: const DarwinInitializationSettings(),
        macOS: const DarwinInitializationSettings(),
        linux: LinuxInitializationSettings(defaultActionName: '', defaultIcon: AssetsLinuxIcon('asset/logo.png')),
      );
      await _flutterLocalNotificationsPlugin!.initialize(initializationSettings, onDidReceiveNotificationResponse: null);
    }
    return;
  }

  static
  Future<Void>
  push(
    String title,
    String description,
  ) async {
    if (Platform.isWindows) {
      await LocalNotification(
        title: title,
        body: description,
      ).show();
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      await _flutterLocalNotificationsPlugin!.show(
        DateTime.now().millisecondsSinceEpoch % 0x80000000,
        title,
        description,
        const NotificationDetails(
          android: AndroidNotificationDetails(
            'com.twinstar.toolkit.shell_gui.notification_channel.main',
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

}
