import Cocoa
import FlutterMacOS

class MainFlutterWindow: NSWindow, NSDraggingDestination {

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

  public func draggingEntered(
    _ sender: any NSDraggingInfo,
  ) -> NSDragOperation {
    return PlatformIntegrationManager.instance().inject_MainFlutterWindow_draggingEntered(self, sender)
  }

  public func draggingUpdated(
    _ sender: any NSDraggingInfo,
  ) -> NSDragOperation {
    return PlatformIntegrationManager.instance().inject_MainFlutterWindow_draggingUpdated(self, sender)
  }

  public func draggingExited(
    _ sender: (any NSDraggingInfo)?,
  ) -> Void {
    return PlatformIntegrationManager.instance().inject_MainFlutterWindow_draggingExited(self, sender)
  }

  public func performDragOperation(
    _ sender: any NSDraggingInfo,
  ) -> Bool {
    return PlatformIntegrationManager.instance().inject_MainFlutterWindow_performDragOperation(self, sender)
  }

}
