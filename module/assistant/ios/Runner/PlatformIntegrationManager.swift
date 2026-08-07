import UIKit
import UniformTypeIdentifiers
import UserNotifications
import Flutter

class PlatformIntegrationManager: NSObject, UIDocumentPickerDelegate, UNUserNotificationCenterDelegate, UIDropInteractionDelegate {

  // MARK: - variable

  private var channel: FlutterMethodChannel!

  private var continuation: CheckedContinuation<Any?, Never>?

  // MARK: - construct

  private override init(
  ) {
    self.channel = nil
    self.continuation = nil
    return
  }

  // MARK: - singleton

  public static func instance(
  ) -> PlatformIntegrationManager {
    struct Holder {
      static let instance = PlatformIntegrationManager()
    }
    return Holder.instance
  }

  // MARK: - inject

  public func inject_AppDelegate_didInitializeImplicitFlutterEngine(
    _ host: AppDelegate,
    _ with_engineBridge: FlutterImplicitEngineBridge,
  ) -> Void {
    return try! self.executePlatformTask(
      false,
      {
        return
      },
      {
        self.channel = FlutterMethodChannel(
          name: "\(try! self.queryApplicationIdentifier())/PlatformIntegrationManager",
          binaryMessenger: with_engineBridge.applicationRegistrar.messenger(),
        )
        self.channel.setMethodCallHandler({ (call, result) in
          self.executePlatformTaskSwitch(false, {
            do {
              let callMethod = call.method
              let callArgument = call.arguments as? Dictionary<String, Any?>
              guard callArgument != nil else {
                throw NSError(domain: "invalid argument.", code: 0)
              }
              let callResult = try await self.handle(callMethod, callArgument!)
              result(callResult)
            }
            catch {
              let callException = error.localizedDescription
              result(FlutterError(code: "", message: callException, details: nil))
            }
          })
          return
        })
        return
      },
    )
  }

  public func inject_SceneDelegate_scene(
    _ host: SceneDelegate,
    _ with_scene: UIScene,
    _ with_session: UISceneSession,
    _ with_connectionOptions: UIScene.ConnectionOptions,
  ) -> Void {
    return try! self.executePlatformTask(
      false,
      {
        return
      },
      {
        let link = with_connectionOptions.urlContexts.first?.url.absoluteString
        if link != nil {
          self.executePlatformTaskSwitch(false, {
            try await self.invokeReceiveApplicationLink(link!)
          })
        }
        return
      },
    )
  }

  public func inject_SceneDelegate_scene(
    _ host: SceneDelegate,
    _ with_scene: UIScene,
    _ with_URLContexts: Set<UIOpenURLContext>,
  ) -> Void {
    return try! self.executePlatformTask(
      false,
      {
        return
      },
      {
        let link = with_URLContexts.first?.url.absoluteString
        if link != nil {
          self.executePlatformTaskSwitch(false, {
            try await self.invokeReceiveApplicationLink(link!)
          })
        }
        return
      },
    )
  }

  public func inject_SceneDelegate_sceneDidBecomeActive(
    _ host: SceneDelegate,
    _ with_scene: UIScene,
  ) -> Void {
    return try! self.executePlatformTask(
      false,
      {
        return
      },
      {
        self.executePlatformTaskSwitch(false, {
          try await self.registerNotificationSupport()
          try await self.registerDragDropSupport()
        })
        return
      },
    )
  }

  // MARK: - handle

  private func handle(
    _ method: String,
    _ argument: Dictionary<String, Any?>,
  ) async throws -> Dictionary<String, Any?> {
    var argumentProxy = argument
    var result: Dictionary<String, Any?> = [:]
    let getArgument = { (name: String) in
      return try self.extractFlutterValueMap(&argumentProxy, name)
    }
    let setResult = { (name: String, value: Any?) in
      return try self.infuseFlutterValueMap(&result, name, value)
    }
    switch method {
    case "check_application_permission":
      let detail = try await self.handleCheckApplicationPermission(
        try self.decodeFlutterValue(try getArgument("name")),
      )
      try setResult("state", try self.encodeFlutterValue(detail))
    case "update_application_permission":
      let _ = try await self.handleUpdateApplicationPermission(
        try self.decodeFlutterValue(try getArgument("name")),
      )
    case "check_application_extension":
      let detail = try await self.handleCheckApplicationExtension(
        try self.decodeFlutterValue(try getArgument("name")),
      )
      try setResult("state", try self.encodeFlutterValue(detail))
    case "update_application_extension":
      let _ = try await self.handleUpdateApplicationExtension(
        try self.decodeFlutterValue(try getArgument("name")),
        try self.decodeFlutterValue(try getArgument("state")),
      )
    case "query_storage_item":
      let detail = try await self.handleQueryStorageItem(
        try self.decodeFlutterValue(try getArgument("type")),
      )
      try setResult("target", try self.encodeFlutterValue(detail))
    case "reveal_storage_item":
      let _ = try await self.handleRevealStorageItem(
        try self.decodeFlutterValue(try getArgument("target")),
      )
    case "pick_storage_item":
      let detail = try await self.handlePickStorageItem(
        try self.decodeFlutterValue(try getArgument("type")),
        try self.decodeFlutterValue(try getArgument("multiply")),
        try self.decodeFlutterValue(try getArgument("location")),
        try self.decodeFlutterValue(try getArgument("name")),
      )
      try setResult("target", try self.encodeFlutterValue(detail))
    case "query_system_theme":
      let detail = try await self.handleQuerySystemTheme(
      )
      try setResult("accent", try self.encodeFlutterValue(detail))
    case "push_system_notification":
      let _ = try await self.handlePushSystemNotification(
        try self.decodeFlutterValue(try getArgument("title")),
        try self.decodeFlutterValue(try getArgument("description")),
      )
    case "query_screen_placement":
      let detail = try await self.handleQueryScreenPlacement(
      )
      try setResult("x", try self.encodeFlutterValue(detail.x))
      try setResult("y", try self.encodeFlutterValue(detail.y))
      try setResult("width", try self.encodeFlutterValue(detail.width))
      try setResult("height", try self.encodeFlutterValue(detail.height))
    case "query_window_placement":
      let detail = try await self.handleQueryWindowPlacement(
      )
      try setResult("x", try self.encodeFlutterValue(detail.x))
      try setResult("y", try self.encodeFlutterValue(detail.y))
      try setResult("width", try self.encodeFlutterValue(detail.width))
      try setResult("height", try self.encodeFlutterValue(detail.height))
    case "update_window_placement":
      let _ = try await self.handleUpdateWindowPlacement(
        try self.decodeFlutterValue(try getArgument("x")),
        try self.decodeFlutterValue(try getArgument("y")),
        try self.decodeFlutterValue(try getArgument("width")),
        try self.decodeFlutterValue(try getArgument("height")),
      )
    default:
      throw NSError(domain: "invalid method.", code: 0)
    }
    return result
  }

  // ----------------

  private func handleCheckApplicationPermission(
    _ name: String,
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
    _ name: String,
  ) async throws -> Void {
    guard name == "storage" || name == "notification" else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    if name == "storage" {
    }
    if name == "notification" {
      try await self.openExternalLink(URL(string: UIApplication.openSettingsURLString)!)
    }
    return
  }

  // ----------------

  private func handleCheckApplicationExtension(
    _ name: String,
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
    _ name: String,
    _ state: Bool,
  ) async throws -> Void {
    guard name == "forwarder" else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    if name == "forwarder" {
    }
    return
  }

  // ----------------

  private func handleQueryStorageItem(
    _ type: String,
  ) async throws -> String {
    guard type == "user_home" || type == "application_shared" || type == "application_persistent" || type == "application_temporary" || type == "application_cache" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    var target: String? = nil
    if type == "user_home" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))"
    }
    if type == "application_shared" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))"
    }
    if type == "application_persistent" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))/persistent"
    }
    if type == "application_temporary" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))/temporary"
    }
    if type == "application_cache" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!))/cache"
    }
    return target!
  }

  private func handleRevealStorageItem(
    _ target: String,
  ) async throws -> Void {
    var link = URL(fileURLWithPath: target)
    let linkComponent = NSURLComponents(url: link, resolvingAgainstBaseURL: true)!
    linkComponent.scheme = "shareddocuments"
    link = linkComponent.url!
    try await self.openExternalLink(link)
    return
  }

  private func handlePickStorageItem(
    _ type: String,
    _ multiply: Bool,
    _ location: String,
    _ name: String,
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
    (try self.getCurrentWindow().rootViewController as! FlutterViewController).present(picker, animated: true)
    let targetUrl = await withCheckedContinuation { (continuation) in self.continuation = continuation } as! [URL]
    self.continuation = nil
    let target = try targetUrl.map({ (item) in try self.resolveFileUrl(item) })
    return target
  }

  // ----------------

  private func handleQuerySystemTheme(
  ) async throws -> Int? {
    return nil
  }

  // ----------------

  private func handlePushSystemNotification(
    _ title: String,
    _ description: String,
  ) async throws -> Void {
    let content = UNMutableNotificationContent()
    content.title = title
    content.body = description
    content.sound = .default
    let request = UNNotificationRequest(identifier: UUID().uuidString, content: content, trigger: nil)
    try await UNUserNotificationCenter.current().add(request)
    return
  }

  // ----------------

  @MainActor
  private func handleQueryScreenPlacement(
  ) async throws -> (x: Int, y: Int, width: Int, height: Int) {
    let screen = try self.getCurrentScreen()
    let rect = screen.bounds
    let x = Int(rect.origin.x)
    let y = Int(rect.origin.y)
    let width = Int(rect.size.width)
    let height = Int(rect.size.height)
    return (x, y, width, height)
  }

  @MainActor
  private func handleQueryWindowPlacement(
  ) async throws -> (x: Int, y: Int, width: Int, height: Int) {
    let screen = try self.getCurrentScreen()
    let window = try self.getCurrentWindow()
    let rect = window.convert(window.bounds, to: screen.coordinateSpace)
    let x = Int(rect.origin.x)
    let y = Int(rect.origin.y)
    let width = Int(rect.size.width)
    let height = Int(rect.size.height)
    return (x, y, width, height)
  }

  @MainActor
  private func handleUpdateWindowPlacement(
    _ x: Int,
    _ y: Int,
    _ width: Int,
    _ height: Int,
  ) async throws -> Void {
    throw NSError(domain: "unsupported method.", code: 0)
  }

  // MARK: - invoke

  private func invoke(
    _ method: String,
    _ argument: Dictionary<String, Any?>,
  ) async throws -> Void {
    self.executePlatformTaskSwitch(true, { @MainActor in
      self.channel.invokeMethod(method, arguments: argument)
    })
    return
  }

  // ----------------

  private func invokeReceivePlatformException(
    _ message: String,
  ) async throws -> Void {
    return try await self.invoke("receive_platform_exception", [
      "message": self.encodeFlutterValue(message),
    ])
  }

  // ----------------

  private func invokeReceiveApplicationLink(
    _ target: String,
  ) async throws -> Void {
    return try await self.invoke("receive_application_link", [
      "target": self.encodeFlutterValue(target),
    ])
  }

  // ----------------

  private func invokeReceiveApplicationDragEnter(
  ) async throws -> Void {
    return try await self.invoke("receive_application_drag_enter", [:])
  }

  private func invokeReceiveApplicationDragOver(
    _ locationX: Int,
    _ locationY: Int,
  ) async throws -> Void {
    return try await self.invoke("receive_application_drag_over", [
      "location_x": self.encodeFlutterValue(locationX),
      "location_y": self.encodeFlutterValue(locationY),
    ])
  }

  private func invokeReceiveApplicationDragLeave(
  ) async throws -> Void {
    return try await self.invoke("receive_application_drag_leave", [:])
  }

  private func invokeReceiveApplicationDragDrop(
    _ target: Array<String>,
  ) async throws -> Void {
    return try await self.invoke("receive_application_drag_drop", [
      "target": self.encodeFlutterValue(target),
    ])
  }

  // MARK: - support

  private func registerNotificationSupport(
  ) async throws -> Void {
    let center = UNUserNotificationCenter.current()
    center.delegate = self
    _ = try? await center.requestAuthorization(options: [.sound, .alert])
    return
  }

  // ----------------

  private func registerDragDropSupport(
  ) async throws -> Void {
    let window = try self.getCurrentWindow()
    window.rootViewController!.view.isUserInteractionEnabled = true
    window.rootViewController!.view.addInteraction(UIDropInteraction(delegate: self))
    return
  }

  // MARK: - utility

  private func executePlatformTask<TResult>(
    _ handleException: Bool,
    _ fallbackAction: @escaping () -> TResult,
    _ taskAction: @escaping () throws -> TResult,
  ) throws -> TResult {
    let task: () throws -> TResult = {
      do {
        return try taskAction()
      }
      catch {
        if !handleException {
          throw error
        }
        Task { @MainActor in
          try await self.invokeReceivePlatformException("\(error.localizedDescription)")
        }
        return fallbackAction()
      }
    }
    return try task()
  }

  private func executePlatformTaskSwitch(
    _ onMain: Bool,
    _ taskAction: @escaping () async throws -> Void,
  ) -> Void {
    let task: @Sendable () async -> Void = {
      do {
        try await taskAction()
      }
      catch {
        try? await self.invokeReceivePlatformException("\(error.localizedDescription)")
      }
      return
    }
    if onMain {
      Task { @MainActor in
        await task()
      }
    }
    else {
      Task {
        await task()
      }
    }
    return
  }

  // ----------------

  private func encodeFlutterValue<TValue>(
    _ ripe: TValue,
  ) throws -> Any? {
    return ripe
  }

  private func decodeFlutterValue<TValue>(
    _ raw: Any?,
  ) throws -> TValue {
    let ripe = raw as? TValue
    let nilValue = Optional<Any>.none as? TValue 
    guard ripe != nil || nilValue != nil else {
      throw NSError(domain: "invalid value.", code: 0)
    }
    return ripe!
  }

  private func extractFlutterValueMap(
    _ map: inout Dictionary<String, Any?>,
    _ name: String,
  ) throws -> Any? {
    guard map.index(forKey: name) != nil else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    return map[name] ?? nil
  }

  private func infuseFlutterValueMap(
    _ map: inout Dictionary<String, Any?>,
    _ name: String,
    _ value: Any?,
  ) throws -> Void {
    map[name] = value
    return
  }

  // ----------------

  private func queryApplicationIdentifier(
  ) throws -> String {
    let identifier = Bundle.main.bundleIdentifier
    guard identifier != nil else {
      throw NSError(domain: "failed to get bundle identifier.", code: 0)
    }
    return identifier!
  }

  // ----------------

  private func resolveFileUrl(
    _ url: URL,
  ) throws -> String {
    let urlComponent = NSURLComponents(url: url, resolvingAgainstBaseURL: true)
    guard urlComponent != nil else {
      throw NSError(domain: "invalid url.", code: 0)
    }
    guard urlComponent!.scheme == "file" && urlComponent!.host == "" && urlComponent!.port == nil && urlComponent!.path != nil else {
      throw NSError(domain: "unknown url.", code: 0)
    }
    var path = urlComponent!.path!
    if path.count > 1 && path.last == "/" {
      path.removeLast()
    }
    return path
  }

  private func openExternalLink(
    _ link: URL,
  ) async throws -> Void {
    guard await UIApplication.shared.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
    }
    return
  }

  private func getCurrentScreen(
  ) throws -> UIScreen {
    let scene = UIApplication.shared.connectedScenes.first(where: { $0.activationState == .foregroundActive }) as? UIWindowScene
    guard scene != nil else {
      throw NSError(domain: "invalid scene.", code: 0)
    }
    let screen = scene!.screen
    return screen
  }

  private func getCurrentWindow(
  ) throws -> UIWindow {
    let scene = UIApplication.shared.connectedScenes.first(where: { $0.activationState == .foregroundActive }) as? UIWindowScene
    guard scene != nil else {
      throw NSError(domain: "invalid scene.", code: 0)
    }
    let window = scene!.windows.first(where: { $0.isKeyWindow })
    guard window != nil else {
      throw NSError(domain: "invalid window.", code: 0)
    }
    return window!
  }

  // MARK: - implement UIDocumentPickerDelegate

  public func documentPicker(
    _ controller: UIDocumentPickerViewController,
    didPickDocumentsAt urls: [URL],
  ) -> Void {
    return try! self.executePlatformTask(
      true,
      {
        return
      },
      {
        controller.dismiss(animated: true)
        self.continuation!.resume(returning: [URL](urls))
        return
      },
    )
  }

  public func documentPickerWasCancelled(
    _ controller: UIDocumentPickerViewController,
  ) -> Void {
    return try! self.executePlatformTask(
      true,
      {
        return
      },
      {
        controller.dismiss(animated: true)
        self.continuation!.resume(returning: [URL]())
        return
      },
    )
  }

  // MARK: - implement UNUserNotificationCenterDelegate

  public func userNotificationCenter(
    _ center: UNUserNotificationCenter,
    willPresent notification: UNNotification,
  ) async -> UNNotificationPresentationOptions {
    return try! self.executePlatformTask(
      true,
      {
        return [.sound, .list, .banner]
      },
      {
        return [.sound, .list, .banner]
      },
    )
  }

  // MARK: - implement UIDropInteractionDelegate

  public func dropInteraction(
    _ interaction: UIDropInteraction,
    sessionDidEnter session: any UIDropSession,
  ) -> Void {
    return try! self.executePlatformTask(
      true,
      {
        return
      },
      {
        let allow = session.canLoadObjects(ofClass: URL.self)
        if allow {
          self.executePlatformTaskSwitch(false, {
            try await self.invokeReceiveApplicationDragEnter()
          })
        }
        return
      },
    )
  }

  public func dropInteraction(
    _ interaction: UIDropInteraction,
    sessionDidUpdate session: any UIDropSession,
  ) -> UIDropProposal {
    return try! self.executePlatformTask(
      true,
      {
        return UIDropProposal(operation: .forbidden)
      },
      {
        let allow = session.canLoadObjects(ofClass: URL.self)
        if allow {
          let point = session.location(in: interaction.view!)
          let locationX = Int(point.x)
          let locationY = Int(point.y)
          self.executePlatformTaskSwitch(false, {
            try await self.invokeReceiveApplicationDragOver(locationX, locationY)
          })
        }
        return UIDropProposal(operation: !allow ? .forbidden : .copy)
      },
    )
  }

  public func dropInteraction(
    _ interaction: UIDropInteraction,
    sessionDidExit session: any UIDropSession,
  ) -> Void {
    return try! self.executePlatformTask(
      true,
      {
        return
      },
      {
        let allow = session.canLoadObjects(ofClass: URL.self)
        if allow {
          self.executePlatformTaskSwitch(false, {
            try await self.invokeReceiveApplicationDragLeave()
          })
        }
        return
      },
    )
  }

  public func dropInteraction(
    _ interaction: UIDropInteraction,
    performDrop session: any UIDropSession,
  ) -> Void {
    return try! self.executePlatformTask(
      true,
      {
        return
      },
      {
        let allow = session.canLoadObjects(ofClass: URL.self)
        if allow {
          _ = session.loadObjects(ofClass: URL.self, completion: { sessionObject in
            let targetUrl = sessionObject
            self.executePlatformTaskSwitch(false, {
              let target = try targetUrl.map({ (item) in try self.resolveFileUrl(item) })
              try await self.invokeReceiveApplicationDragDrop(target)
            })
          })
        }
        return
      },
    )
  }

}
