import Cocoa
import FlutterMacOS

@main
class AppDelegate: FlutterAppDelegate {

  public override func applicationShouldTerminateAfterLastWindowClosed(
    _ sender: NSApplication,
  ) -> Bool {
    return true
  }

  public override func applicationSupportsSecureRestorableState(
    _ app: NSApplication,
  ) -> Bool {
    return true
  }

  public override func applicationWillFinishLaunching(
    _ notification: Notification,
  ) -> Void {
    PlatformIntegrationManager.instance().inject_AppDelegate_applicationWillFinishLaunching(self, notification)
    return
  }

}
