import Flutter
import UIKit

@main
@objc class AppDelegate: FlutterAppDelegate, FlutterImplicitEngineDelegate {

  private lazy var platformIntegrationManager: PlatformIntegrationManager = PlatformIntegrationManager(host: self)

  public override func application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?,
  ) -> Bool {
    return super.application(application, didFinishLaunchingWithOptions: launchOptions)
  }

  public func didInitializeImplicitFlutterEngine(
    _ engineBridge: FlutterImplicitEngineBridge,
  ) {
    GeneratedPluginRegistrant.register(with: engineBridge.pluginRegistry)
    self.platformIntegrationManager.register_didInitializeImplicitFlutterEngine(engineBridge)
    return
  }

}
