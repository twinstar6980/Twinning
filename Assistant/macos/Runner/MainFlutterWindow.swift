import Cocoa
import FlutterMacOS

class MainFlutterWindow: NSWindow {

  private lazy var customMethodChannel: CustomMethodChannel = CustomMethodChannel(host: self)

  public override func awakeFromNib(
  ) {
    let flutterViewController = FlutterViewController()
    let windowFrame = self.frame
    self.contentViewController = flutterViewController
    self.setFrame(windowFrame, display: true)
    RegisterGeneratedPlugins(registry: flutterViewController)
    self.customMethodChannel.register_awakeFromNib()
    super.awakeFromNib()
    return
  }

}
