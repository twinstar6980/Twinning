import Cocoa
import FlutterMacOS

class MainFlutterWindow: NSWindow {

  public override func awakeFromNib(
  ) -> Void {
    let flutterViewController = FlutterViewController()
    let windowFrame = self.frame
    self.contentViewController = flutterViewController
    self.setFrame(windowFrame, display: true)
    RegisterGeneratedPlugins(registry: flutterViewController)
    PlatformIntegrationManager.instance().inject_MainFlutterWindow_awakeFromNib(self)
    super.awakeFromNib()
    return
  }

}
