import AppKit
import UserNotifications
import FlutterMacOS

class PlatformIntegrationManager: NSObject, UNUserNotificationCenterDelegate {

  // MARK: - variable

  private var channel: FlutterMethodChannel!

  // MARK: - construct

  private override init(
  ) {
    self.channel = nil
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

  public func inject_AppDelegate_applicationWillFinishLaunching(
    _ host: AppDelegate,
    _ with_notification: Notification,
  ) -> Void {
    NSAppleEventManager.shared().setEventHandler(
      self,
      andSelector: #selector(self.resolveAppleEventForUrl(_:withReplyEvent:)),
      forEventClass: AEEventClass(kInternetEventClass),
      andEventID: AEEventID(kAEGetURL),
    )
    return
  }

  public func inject_MainFlutterWindow_awakeFromNib(
    _ host: MainFlutterWindow,
  ) -> Void {
    self.channel = FlutterMethodChannel(
      name: "\(try! self.queryApplicationIdentifier())/PlatformIntegrationManager",
      binaryMessenger: (host.contentViewController as! FlutterViewController).engine.binaryMessenger,
    )
    self.channel.setMethodCallHandler({ [weak self] (call, result) in
      Task {
        await self?.handle(call, result)
      }
      return
    })
    UNUserNotificationCenter.current().delegate = self
    UNUserNotificationCenter.current().requestAuthorization(options: [.sound, .alert], completionHandler: { _, _ in })
    return
  }

  // MARK: - handle

  private func handle(
    _ call: FlutterMethodCall,
    _ result: @escaping FlutterResult,
  ) async -> Void {
    do {
      let method = call.method
      var rawArgument = call.arguments as? [String: Any?]
      guard rawArgument != nil else {
        throw NSError(domain: "invalid argument.", code: 0)
      }
      let getArgument = { (name: String) in
        return try self.extractFlutterValueMap(&rawArgument!, name)
      }
      var rawResult: [String: Any?] = [:]
      let setResult = { (name: String, value: Any?) in
        return try self.infuseFlutterValueMap(&rawResult, name, value)
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
      case "push_system_notification":
        let _ = try await self.handlePushSystemNotification(
          try self.decodeFlutterValue(try getArgument("title")),
          try self.decodeFlutterValue(try getArgument("description")),
        )
      default:
        throw NSError(domain: "invalid method.", code: 0)
      }
      result(rawResult)
    }
    catch {
      result(FlutterError(code: "", message: error.localizedDescription, details: nil))
    }
    return
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
      try await self.openExternalLink(URL(string: "x-apple.systempreferences:com.apple.Notifications-Settings.extension?id=\(self.queryApplicationIdentifier())")!)
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
      let process = Process()
      process.executableURL = URL(fileURLWithPath: "/usr/bin/pluginkit")
      process.arguments = [
        "-m",
        "-i", "\(try self.queryApplicationIdentifier()).Forwarder",
      ]
      let processOutput = Pipe()
      process.standardOutput = processOutput
      try process.run()
      process.waitUntilExit()
      guard process.terminationStatus == 0 else {
        throw NSError(domain: "failed to run pluginkit.", code: 0)
      }
      let processOutputString = String(data: try processOutput.fileHandleForReading.readToEnd()!, encoding: .utf8)!
      state = processOutputString.starts(with: "+")
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
      let process = Process()
      process.executableURL = URL(fileURLWithPath: "/usr/bin/pluginkit")
      process.arguments = [
        "-e", "\(!state ? "ignore" : "use")",
        "-i", "\(try self.queryApplicationIdentifier()).Forwarder",
      ]
      let processOutput = Pipe()
      process.standardOutput = processOutput
      try process.run()
      process.waitUntilExit()
      guard process.terminationStatus == 0 else {
        throw NSError(domain: "failed to run pluginkit.", code: 0)
      }
    }
    return
  }

  // ----------------

  private func handleQueryStorageItem(
    _ type: String,
  ) async throws -> String {
    guard type == "user_home" || type == "application_shared" || type == "application_temporary" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    var target: String? = nil
    if type == "user_home" {
      target = "\(try self.resolveFileUrl(FileManager.default.homeDirectoryForCurrentUser))"
    }
    if type == "application_shared" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!))/\(try self.queryApplicationIdentifier())"
    }
    if type == "application_temporary" {
      target = "\(try self.resolveFileUrl(FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!))/\(try self.queryApplicationIdentifier())/temporary"
    }
    return target!
  }

  private func handleRevealStorageItem(
    _ target: String,
  ) async throws -> Void {
    let link = URL(fileURLWithPath: target)
    try await self.openExternalLink(link)
    return
  }

  @MainActor
  private func handlePickStorageItem(
    _ type: String,
    _ multiply: Bool,
    _ location: String,
    _ name: String,
  ) async throws -> Array<String> {
    guard type == "load_file" || type == "load_directory" || type == "save_file" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    var picker: NSSavePanel!
    if type == "load_file" || type == "load_directory" {
      let pickerView = NSOpenPanel()
      pickerView.resolvesAliases = true
      pickerView.canResolveUbiquitousConflicts = true
      pickerView.canDownloadUbiquitousContents = true
      pickerView.isAccessoryViewDisclosed = false
      pickerView.allowsMultipleSelection = multiply
      pickerView.canChooseFiles = type == "load_file"
      pickerView.canChooseDirectories = type == "load_directory"
      picker = pickerView
    }
    if type == "save_file" {
      let pickerView = NSSavePanel()
      pickerView.canSelectHiddenExtension = false
      pickerView.isExtensionHidden = false
      pickerView.showsTagField = false
      pickerView.nameFieldStringValue = name
      picker = pickerView
    }
    picker.showsHiddenFiles = true
    picker.treatsFilePackagesAsDirectories = true
    picker.canCreateDirectories = true
    picker.allowedContentTypes = []
    picker.directoryURL = URL(fileURLWithPath: location)
    var targetUrl: [URL] = []
    if (picker.runModal() == .OK) {
      if type == "load_file" || type == "load_directory" {
        let pickerView = picker as! NSOpenPanel
        targetUrl.append(contentsOf: pickerView.urls)
      }
      if type == "save_file" {
        let pickerView = picker!
        if pickerView.url != nil {
          targetUrl.append(pickerView.url!)
        }
      }
    }
    let target = try targetUrl.map({ (item) in try self.resolveFileUrl(item) })
    return target
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

  // MARK: - invoke

  private func invoke(
    _ method: String,
    _ argument: [String: Any?],
  ) async throws -> Void {
    self.channel.invokeMethod(method, arguments: argument)
    return
  }

  // ----------------

  private func invokeReceiveApplicationLink(
    _ target: String,
  ) async throws -> Void {
    return try await self.invoke("receive_application_link", [
      "target": self.encodeFlutterValue(target),
    ])
  }

  // MARK: - utility

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
    _ map: inout [String: Any?],
    _ name: String,
  ) throws -> Any? {
    guard map.index(forKey: name) != nil else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    return map[name] ?? nil
  }

  private func infuseFlutterValueMap(
    _ map: inout [String: Any?],
    _ name: String,
    _ value: Any?,
  ) throws -> Void {
    map[name] = value
    return
  }

  // ----------------

  private func queryApplicationIdentifier(
  ) throws -> String {
    guard let identifier = Bundle.main.bundleIdentifier else {
      throw NSError(domain: "failed to get bundle identifier.", code: 0)
    }
    return identifier
  }

  // ----------------

  private func resolveFileUrl(
    _ url: URL,
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

  private func openExternalLink(
    _ link: URL,
  ) async throws -> Void {
    guard NSWorkspace.shared.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
    }
    return
  }

  // MARK: - resolve AppleEvent

  @objc
  private func resolveAppleEventForUrl(
    _ event: NSAppleEventDescriptor,
    withReplyEvent replyEvent: NSAppleEventDescriptor,
  ) {
    let link = event.paramDescriptor(forKeyword: AEKeyword(keyDirectObject))?.stringValue
    if link != nil {
      Task {
        _ = try? await self.invokeReceiveApplicationLink(link!)
      }
    }
    return
  }

  // MARK: - implement UNUserNotificationCenterDelegate

  public func userNotificationCenter(
    _ center: UNUserNotificationCenter,
    willPresent notification: UNNotification,
  ) async -> UNNotificationPresentationOptions {
    return [.sound, .list, .banner]
  }

}
