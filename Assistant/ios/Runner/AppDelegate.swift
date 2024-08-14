import Flutter
import UIKit
import flutter_local_notifications
import app_links

@main
@objc class AppDelegate: FlutterAppDelegate {
  private lazy var customMethodChannel: CustomMethodChannel = CustomMethodChannel(host: self)
  public override func application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?
  ) -> Bool {
    FlutterLocalNotificationsPlugin.setPluginRegistrantCallback { (registry) in
      GeneratedPluginRegistrant.register(with: registry)
    }
    if #available(iOS 10.0, *) {
      UNUserNotificationCenter.current().delegate = self as UNUserNotificationCenterDelegate
    }
    GeneratedPluginRegistrant.register(with: self)
    self.customMethodChannel.register_application(application, didFinishLaunchingWithOptions: launchOptions)
    if let url = AppLinks.shared.getLink(launchOptions: launchOptions) {
      AppLinks.shared.handleLink(url: url)
      return true
    }
    return super.application(application, didFinishLaunchingWithOptions: launchOptions)
  }
}
