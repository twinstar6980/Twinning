import UIKit
import UniformTypeIdentifiers
import Flutter

class CustomMethodChannel: NSObject, UIDocumentPickerDelegate {

  // MARK: - variable

  private let host: AppDelegate

  private var continuation: CheckedContinuation<Any?, Never>?

  private var command: Array<String>

  // MARK: - construct

  public init(
    host: AppDelegate
  ) {
    self.host = host
    self.continuation = nil
    self.command = []
  }

  // MARK: - register

  public func register_application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?
  ) -> Void {
    let rootController = self.host.window?.rootViewController as! FlutterViewController
    FlutterMethodChannel(
      name: "com.twinstar.toolkit.shell_gui.CustomMethodChannel",
      binaryMessenger: rootController.binaryMessenger
    ).setMethodCallHandler({ [weak self] (call, result) in
      Task {
        await self?.handle(call: call, result: result)
      }
      return
    })
    if let link = launchOptions?[.url] as? URL {
      if let linkComponent = NSURLComponents(url: link, resolvingAgainstBaseURL: true) {
        if linkComponent.scheme == "twinstar.toolkit.shell-gui" && linkComponent.host == nil && linkComponent.port == nil && linkComponent.path == "/run" {
          self.command.append(contentsOf: (linkComponent.queryItems ?? []).filter({ (item) in return item.name == "command" }).map({ (item) in return item.value ?? "" }))
        }
      }
    }
    return
  }

  // MARK: - handle

  private func handle(
    call: FlutterMethodCall,
    result: @escaping FlutterResult
  ) async -> Void {
    do {
      guard let argument = call.arguments as? [String: Any?] else {
        throw NSError(domain: "invalid argument", code: 0)
      }
      switch call.method {
      case "pickPath":
        guard let argument_isDirectory = argument["isDirectory"] as? NSNumber else {
          throw NSError(domain: "invalid argument", code: 0)
        }
        result(try await self.handle_pickPath(
          isDirectory: argument_isDirectory.boolValue
        ))
      case "getCommand":
        result(try await self.handle_getCommand(
        ))
      default:
        result(FlutterMethodNotImplemented)
      }
    }
    catch {
      result(FlutterError(code: "", message: error.localizedDescription, details: nil))
    }
    return
  }

  private func handle_pickPath(
    isDirectory: Bool
  ) async throws -> String? {
    let rootController = self.host.window?.rootViewController as! FlutterViewController
    let pickerController = UIDocumentPickerViewController(forOpeningContentTypes: [!isDirectory ? .item : .folder])
    pickerController.delegate = self
    pickerController.allowsMultipleSelection = false
    pickerController.shouldShowFileExtensions = true
    rootController.present(pickerController, animated: true)
    let url = await withCheckedContinuation { (continuation) in self.continuation = continuation } as? URL
    self.continuation = nil
    let path = url == nil ? nil : try self.parsePathOfFileURL(url: url!)
    return path
  }

  private func handle_getCommand(
  ) async throws -> Array<String> {
    return self.command
  }

  // MARK: - implement - UIDocumentPickerDelegate

  public func documentPicker(
    _ controller: UIDocumentPickerViewController,
    didPickDocumentAt url: URL
  ) -> Void {
    controller.dismiss(animated: true)
    self.continuation!.resume(returning: url)
    return
  }

  public func documentPickerWasCancelled(
    _ controller: UIDocumentPickerViewController
  ) -> Void {
    controller.dismiss(animated: true)
    self.continuation!.resume(returning: nil)
    return
  }

  // MARK: - utility

  private func parsePathOfFileURL(
    url: URL
  ) throws -> String {
    guard let urlComponent = NSURLComponents(url: url, resolvingAgainstBaseURL: true) else {
      throw NSError(domain: "invalid url", code: 0)
    }
    guard urlComponent.scheme == "file" && urlComponent.host == "" && urlComponent.port == nil && urlComponent.path != nil else {
      throw NSError(domain: "unknown url", code: 0)
    }
    var path = urlComponent.path!
    if path != "/" && path.last == "/" {
      path.removeLast()
    }
    return path
  }

}
