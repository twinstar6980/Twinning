import Cocoa
import FlutterMacOS

class MainFlutterWindow: NSWindow {

  private lazy var platformIntegrationManager: PlatformIntegrationManager = PlatformIntegrationManager(host: self)

  public override func awakeFromNib(
  ) {
    let flutterViewController = FlutterViewController()
    let windowFrame = self.frame
    self.contentViewController = flutterViewController
    self.setFrame(windowFrame, display: true)
    RegisterGeneratedPlugins(registry: flutterViewController)
    self.platformIntegrationManager.register_awakeFromNib()
    super.awakeFromNib()
    return
  }

}
