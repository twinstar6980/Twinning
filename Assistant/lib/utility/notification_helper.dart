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
    _plugin = lib.FlutterLocalNotificationsPlugin();
    await _plugin!.initialize(
      lib.InitializationSettings(
        android: lib.AndroidInitializationSettings(
          '@mipmap/ic_launcher_foreground',
        ),
        iOS: lib.DarwinInitializationSettings(
        ),
        macOS: lib.DarwinInitializationSettings(
        ),
        linux: lib.LinuxInitializationSettings(
          defaultActionName: '',
          defaultIcon: lib.AssetsLinuxIcon('asset/logo.png'),
        ),
        windows: lib.WindowsInitializationSettings(
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
    await _plugin!.show(
      DateTime.now().millisecondsSinceEpoch % 0x80000000,
      title,
      description,
      lib.NotificationDetails(
        android: lib.AndroidNotificationDetails(
          '${ApplicationInformation.identifier}.notification_channel.main',
          'Main',
        ),
        iOS: lib.DarwinNotificationDetails(
        ),
        macOS: lib.DarwinNotificationDetails(
        ),
        linux: lib.LinuxNotificationDetails(
        ),
        windows: lib.WindowsNotificationDetails(
        ),
      ),
    );
    return;
  }

  // #endregion

}
