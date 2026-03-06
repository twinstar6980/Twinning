import '/common.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart' as lib;

// ----------------

class ApplicationNotificationManager {

  // #region singleton

  static final ApplicationNotificationManager instance = ._();

  // #endregion

  // #region constructor

  Boolean _initialized;

  lib.FlutterLocalNotificationsPlugin? _plugin;

  Void Function()? _handler;

  // ----------------

  ApplicationNotificationManager._(
  ) :
    this._initialized = false,
    this._plugin = null,
    this._handler = null;

  // #endregion

  // #region utility

  Future<Void> initialize(
  ) async {
    assertTest(!this._initialized);
    this._plugin = .new();
    await this._plugin!.initialize(
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
      onDidReceiveNotificationResponse: (details) {
        this._handler?.call();
        return;
      },
    );
    this._initialized = true;
    return;
  }

  Future<Void> listen(
    Void Function()? handler,
  ) async {
    assertTest(this._initialized);
    this._handler = handler;
    return;
  }

  // ----------------

  Future<Void> push(
    String title,
    String description,
  ) async {
    assertTest(this._initialized);
    await this._plugin!.show(
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
