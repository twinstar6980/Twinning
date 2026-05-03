import UIKit
import UniformTypeIdentifiers
import UserNotifications
import Flutter

class CustomMethodChannel: NSObject, UIDocumentPickerDelegate, UNUserNotificationCenterDelegate {

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
    UNUserNotificationCenter.current().delegate = self
    UNUserNotificationCenter.current().requestAuthorization(options: [.sound, .alert], completionHandler: { _, _ in })
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
      case "check_application_permission":
        let detail = try await self.handleCheckApplicationPermission(
          name: argumentMap["name"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
        resultMap["state"] = detail
      case "update_application_permission":
        let _ = try await self.handleUpdateApplicationPermission(
          name: argumentMap["name"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
      case "check_application_extension":
        let detail = try await self.handleCheckApplicationExtension(
          name: argumentMap["name"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
        resultMap["state"] = detail
      case "update_application_extension":
        let _ = try await self.handleUpdateApplicationExtension(
          name: argumentMap["name"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          state: argumentMap["state"] as? Bool ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
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
      case "push_system_notification":
        let _ = try await self.handlePushSystemNotification(
          title: argumentMap["title"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
          description: argumentMap["description"] as? String ?? {
            throw NSError(domain: "invalid argument.", code: 0)
          }(),
        )
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

  private func handleCheckApplicationPermission(
    name: String,
  ) async throws -> Bool {
    guard name == "storage" || name == "notification" else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    var state = false
    if name == "storage" {
      state = true
    }
    if name == "notification" {
      let settings = await UNUserNotificationCenter.current().notificationSettings()
      state = settings.authorizationStatus == .authorized
    }
    return state
  }

  private func handleUpdateApplicationPermission(
    name: String,
  ) async throws -> Void {
    guard name == "storage" || name == "notification" else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    if name == "storage" {
    }
    if name == "notification" {
      try await self.openLink(link: URL(string: UIApplication.openSettingsURLString)!)
    }
    return
  }

  private func handleCheckApplicationExtension(
    name: String,
  ) async throws -> Bool {
    guard name == "forwarder" else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    var state = false
    if name == "forwarder" {
      state = true
    }
    return state
  }

  private func handleUpdateApplicationExtension(
    name: String,
    state: Bool,
  ) async throws -> Void {
    guard name == "forwarder" else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    if name == "forwarder" {
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

  private func handlePushSystemNotification(
    title: String,
    description: String,
  ) async throws -> Void {
    let content = UNMutableNotificationContent()
    content.title = title
    content.body = description
    content.sound = .default
    let request = UNNotificationRequest(identifier: UUID().uuidString, content: content, trigger: nil)
    try await UNUserNotificationCenter.current().add(request)
    return
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

  // MARK: - implement UNUserNotificationCenterDelegate

  public func userNotificationCenter(
    _ center: UNUserNotificationCenter,
    willPresent notification: UNNotification,
  ) async -> UNNotificationPresentationOptions {
    return [.sound, .list, .banner]
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
