import Flutter
import UIKit

@main
@objc class AppDelegate: FlutterAppDelegate, FlutterImplicitEngineDelegate {

  public override func application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?,
  ) -> Bool {
    return super.application(application, didFinishLaunchingWithOptions: launchOptions)
  }

  public func didInitializeImplicitFlutterEngine(
    _ engineBridge: FlutterImplicitEngineBridge,
  ) -> Void {
    GeneratedPluginRegistrant.register(with: engineBridge.pluginRegistry)
    PlatformIntegrationManager.instance().inject_AppDelegate_didInitializeImplicitFlutterEngine(self, engineBridge)
    return
  }

}
