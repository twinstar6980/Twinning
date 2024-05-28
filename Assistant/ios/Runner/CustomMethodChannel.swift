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
    let rootView = self.host.window?.rootViewController as! FlutterViewController
    FlutterMethodChannel(
      name: "com.twinstar.twinning.assistant.CustomMethodChannel",
      binaryMessenger: rootView.binaryMessenger
    ).setMethodCallHandler({ [weak self] (call, result) in
      Task {
        await self?.handle(call: call, result: result)
      }
      return
    })
    if let link = launchOptions?[.url] as? URL {
      if let linkComponent = NSURLComponents(url: link, resolvingAgainstBaseURL: true) {
        if linkComponent.scheme == "twinstar.twinning.assistant" && linkComponent.host == nil && linkComponent.port == nil && linkComponent.path == "/run" {
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
        throw NSError(domain: "invalid argument.", code: 0)
      }
      switch call.method {
      case "pick_storage_path":
        guard let detailType = argument["type"] as? String else {
          throw NSError(domain: "invalid argument.", code: 0)
        }
        let detailTarget = try await self.handlePickStoragePath(
          type: detailType
        )
        result(detailTarget)
      case "get_link_command":
        let detailCommand = try await self.handleGetLinkCommand(
        )
        result(detailCommand)
      default:
        result(FlutterMethodNotImplemented)
      }
    }
    catch {
      result(FlutterError(code: "", message: error.localizedDescription, details: nil))
    }
    return
  }

  private func handlePickStoragePath(
    type: String
  ) async throws -> String? {
    guard type == "open_file" || type == "open_directory" || type == "save_file" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    let rootView = self.host.window?.rootViewController as! FlutterViewController
    var pickerView: UIDocumentPickerViewController!
    if type == "open_file" {
      pickerView = UIDocumentPickerViewController(forOpeningContentTypes: [.item])
    }
    if type == "open_directory" {
      pickerView = UIDocumentPickerViewController(forOpeningContentTypes: [.folder])
    }
    if type == "save_file" {
      pickerView = UIDocumentPickerViewController(forExporting: [], asCopy: false)
    }
    pickerView.delegate = self
    pickerView.allowsMultipleSelection = false
    pickerView.shouldShowFileExtensions = true
    rootView.present(pickerView, animated: true)
    let targetUrl = await withCheckedContinuation { (continuation) in self.continuation = continuation } as? URL
    self.continuation = nil
    let target = targetUrl == nil ? nil : try self.parsePathOfFileURL(url: targetUrl!)
    return target
  }

  private func handleGetLinkCommand(
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
      throw NSError(domain: "invalid url.", code: 0)
    }
    guard urlComponent.scheme == "file" && urlComponent.host == "" && urlComponent.port == nil && urlComponent.path != nil else {
      throw NSError(domain: "unknown url.", code: 0)
    }
    var path = urlComponent.path!
    if path.count > 1 && path.last == "/" {
      path.removeLast()
    }
    return path
  }

}
