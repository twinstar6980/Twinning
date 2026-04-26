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
      name: "\(try! self.queryApplicationIdentifier()).CustomMethodChannel",
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
      guard let argumentMap = call.arguments as? [String: Any?] else {
        throw NSError(domain: "invalid argument.", code: 0)
      }
      var resultMap: [String: Any] = [:]
      switch call.method {
      case "query_storage_item":
        let detail = try await self.handleQueryStorageItem(
          type: argumentMap["type"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
        resultMap["target"] = detail
      case "reveal_storage_item":
        let _ = try await self.handleRevealStorageItem(
          target: argumentMap["target"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
      case "pick_storage_item":
        let detail = try await self.handlePickStorageItem(
          type: argumentMap["type"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          multiply: argumentMap["multiply"] as? Bool ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          location: argumentMap["location"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          name: argumentMap["name"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
        resultMap["target"] = detail
      default:
        throw NSError(domain: "invalid method.", code: 0)
      }
      result(resultMap)
    }
    catch {
      result(FlutterError(code: "", message: error.localizedDescription, details: nil))
    }
    return
  }

  private func handleQueryStorageItem(
    type: String,
  ) async throws -> String {
    guard type == "user_home" || type == "application_shared" || type == "application_temporary" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    var target: String? = nil
    if type == "user_home" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))"
    }
    if type == "application_shared" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))"
    }
    if type == "application_temporary" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))/temporary"
    }
    return target!
  }

  private func handleRevealStorageItem(
    target: String,
  ) async throws -> Void {
    var link = URL(fileURLWithPath: target)
    let linkComponent = NSURLComponents(url: link, resolvingAgainstBaseURL: true)!
    linkComponent.scheme = "shareddocuments"
    link = linkComponent.url!
    try await self.openLink(link: link)
    return
  }

  private func handlePickStorageItem(
    type: String,
    multiply: Bool,
    location: String,
    name: String,
  ) async throws -> Array<String> {
    guard type == "load_file" || type == "load_directory" || type == "save_file" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    if type == "save_file" {
      throw NSError(domain: "unsupported type.", code: 0)
    }
    var picker: UIDocumentPickerViewController!
    if type == "load_file" || type == "load_directory" {
      picker = UIDocumentPickerViewController(forOpeningContentTypes: [type == "load_file" ? .item : .folder])
      picker.allowsMultipleSelection = multiply
    }
    picker.shouldShowFileExtensions = true
    picker.directoryURL = URL(fileURLWithPath: location)
    picker.delegate = self
    (try self.getCurrentSceneWindow().rootViewController as! FlutterViewController).present(picker, animated: true)
    let targetUrl = await withCheckedContinuation { (continuation) in self.continuation = continuation } as! [URL]
    self.continuation = nil
    let target = try targetUrl.map({ (item) in try self.resolveFileUrl(url: item) })
    return target
  }

  // MARK: - implement UIDocumentPickerDelegate

  public func documentPicker(
    _ controller: UIDocumentPickerViewController,
    didPickDocumentsAt urls: [URL],
  ) -> Void {
    controller.dismiss(animated: true)
    self.continuation!.resume(returning: [URL](urls))
    return
  }

  public func documentPickerWasCancelled(
    _ controller: UIDocumentPickerViewController,
  ) -> Void {
    controller.dismiss(animated: true)
    self.continuation!.resume(returning: [URL]())
    return
  }

  // MARK: - utility

  private func queryApplicationIdentifier(
  ) throws -> String {
    guard let identifier = Bundle.main.bundleIdentifier else {
      throw NSError(domain: "failed to get bundle identifier.", code: 0)
    }
    return identifier
  }

  // ----------------

  private func resolveFileUrl(
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

  private func openLink(
    link: URL,
  ) async throws -> Void {
    guard await UIApplication.shared.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
    }
    return
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
