import UIKit
import UniformTypeIdentifiers
import Flutter

class CustomMethodChannel: NSObject, UIDocumentPickerDelegate {

  // MARK: - variable

  private let host: AppDelegate

  private var continuation: CheckedContinuation<Any?, Never>?

  // MARK: - construct

  public init(
    host: AppDelegate,
  ) {
    self.host = host
    self.continuation = nil
    return
  }

  // MARK: - register

  public func register_didInitializeImplicitFlutterEngine(
    _ engineBridge: FlutterImplicitEngineBridge,
  ) -> Void {
    FlutterMethodChannel(
      name: "\(try! self.getApplicationIdentifier()).CustomMethodChannel",
      binaryMessenger: engineBridge.applicationRegistrar.messenger(),
    ).setMethodCallHandler({ [weak self] (call, result) in
      Task {
        await self?.handle(call: call, result: result)
      }
      return
    })
    return
  }

  // MARK: - handle

  private func handle(
    call: FlutterMethodCall,
    result: @escaping FlutterResult,
  ) async -> Void {
    do {
      guard let argument = call.arguments as? [String: Any?] else {
        throw NSError(domain: "invalid argument.", code: 0)
      }
      switch call.method {
      case "pick_storage_item":
        let detail = try await self.handlePickStorageItem(
          type: argument["type"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          location: argument["location"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          name: argument["name"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
        result([
          "target": detail,
        ])
      default:
        result(FlutterMethodNotImplemented)
      }
    }
    catch {
      result(FlutterError(code: "", message: error.localizedDescription, details: nil))
    }
    return
  }

  private func handlePickStorageItem(
    type: String,
    location: String,
    name: String,
  ) async throws -> String? {
    guard type == "load_file" || type == "load_directory" || type == "save_file" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    if type == "save_file" {
      throw NSError(domain: "unsupported type.", code: 0)
    }
    var picker: UIDocumentPickerViewController!
    if type == "load_file" || type == "load_directory" {
      picker = UIDocumentPickerViewController(forOpeningContentTypes: [type == "load_file" ? .item : .folder])
      picker.allowsMultipleSelection = false
    }
    picker.shouldShowFileExtensions = true
    picker.directoryURL = URL(fileURLWithPath: location)
    picker.delegate = self
    (try self.getCurrentSceneWindow().rootViewController as! FlutterViewController).present(picker, animated: true)
    let targetUrl = await withCheckedContinuation { (continuation) in self.continuation = continuation } as? URL
    self.continuation = nil
    let target = targetUrl == nil ? nil : try self.getFileActualPath(url: targetUrl!)
    return target
  }

  // MARK: - implement UIDocumentPickerDelegate

  public func documentPicker(
    _ controller: UIDocumentPickerViewController,
    didPickDocumentAt url: URL,
  ) -> Void {
    controller.dismiss(animated: true)
    self.continuation!.resume(returning: url)
    return
  }

  public func documentPickerWasCancelled(
    _ controller: UIDocumentPickerViewController,
  ) -> Void {
    controller.dismiss(animated: true)
    self.continuation!.resume(returning: nil)
    return
  }

  // MARK: - utility

  private func getApplicationIdentifier(
  ) throws -> String {
    guard let identifier = Bundle.main.bundleIdentifier else {
      throw NSError(domain: "failed to get bundle identifier.", code: 0)
    }
    return identifier
  }

  // ----------------

  private func getFileActualPath(
    url: URL,
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

  private func getCurrentSceneWindow(
  ) throws -> UIWindow {
    guard let currentScene = UIApplication.shared.connectedScenes.first(where: { $0.activationState == .foregroundActive }) as? UIWindowScene else {
      throw NSError(domain: "invalid scene.", code: 0)
    }
    guard let currentWindow = currentScene.windows.first(where: { $0.isKeyWindow }) else {
      throw NSError(domain: "invalid window.", code: 0)
    }
    return currentWindow
  }

}
