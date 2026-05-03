import Flutter
import UIKit

@main
@objc class AppDelegate: FlutterAppDelegate, FlutterImplicitEngineDelegate {

  private lazy var customMethodChannel: CustomMethodChannel = CustomMethodChannel(host: self)

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
    self.customMethodChannel.register_didInitializeImplicitFlutterEngine(engineBridge)
    return
  }

}
