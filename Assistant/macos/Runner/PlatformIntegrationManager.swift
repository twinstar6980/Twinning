import AppKit
import UserNotifications
import FlutterMacOS

class PlatformIntegrationManager: NSObject, UNUserNotificationCenterDelegate {

  // MARK: - variable

  private let host: MainFlutterWindow

  private var channel: FlutterMethodChannel?

  // MARK: - construct

  public init(
    host: MainFlutterWindow,
  ) {
    self.host = host
    self.channel = nil
    return
  }

  // MARK: - register

  public func register_awakeFromNib(
  ) -> Void {
    self.channel = FlutterMethodChannel(
      name: "\(try! self.queryApplicationIdentifier())/PlatformIntegrationManager",
      binaryMessenger: (self.host.contentViewController as! FlutterViewController).engine.binaryMessenger,
    )
    self.channel!.setMethodCallHandler({ [weak self] (call, result) in
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
      let method = call.method
      var rawArgument = call.arguments as? [String: Any?]
      guard rawArgument != nil else {
        throw NSError(domain: "invalid argument.", code: 0)
      }
      let getArgument = { (name: String) in
        return try self.extractFlutterValueMap(map: &rawArgument!, name: name)
      }
      var rawResult: [String: Any?] = [:]
      let setResult = { (name: String, value: Any?) in
        return try self.infuseFlutterValueMap(map: &rawResult, name: name, value: value)
      }
      switch method {
      case "check_application_permission":
        let detail = try await self.handleCheckApplicationPermission(
          name: try self.decodeFlutterValue(raw: try getArgument("name")),
        )
        try setResult("state", try self.encodeFlutterValue(ripe: detail))
      case "update_application_permission":
        let _ = try await self.handleUpdateApplicationPermission(
          name: try self.decodeFlutterValue(raw: try getArgument("name")),
        )
      case "check_application_extension":
        let detail = try await self.handleCheckApplicationExtension(
          name: try self.decodeFlutterValue(raw: try getArgument("name")),
        )
        try setResult("state", try self.encodeFlutterValue(ripe: detail))
      case "update_application_extension":
        let _ = try await self.handleUpdateApplicationExtension(
          name: try self.decodeFlutterValue(raw: try getArgument("name")),
          state: try self.decodeFlutterValue(raw: try getArgument("state")),
        )
      case "query_storage_item":
        let detail = try await self.handleQueryStorageItem(
          type: try self.decodeFlutterValue(raw: try getArgument("type")),
        )
        try setResult("target", try self.encodeFlutterValue(ripe: detail))
      case "reveal_storage_item":
        let _ = try await self.handleRevealStorageItem(
          target: try self.decodeFlutterValue(raw: try getArgument("target")),
        )
      case "pick_storage_item":
        let detail = try await self.handlePickStorageItem(
          type: try self.decodeFlutterValue(raw: try getArgument("type")),
          multiply: try self.decodeFlutterValue(raw: try getArgument("multiply")),
          location: try self.decodeFlutterValue(raw: try getArgument("location")),
          name: try self.decodeFlutterValue(raw: try getArgument("name")),
        )
        try setResult("target", try self.encodeFlutterValue(ripe: detail))
      case "push_system_notification":
        let _ = try await self.handlePushSystemNotification(
          title: try self.decodeFlutterValue(raw: try getArgument("title")),
          description: try self.decodeFlutterValue(raw: try getArgument("description")),
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
      try await self.openExternalLink(link: URL(string: "x-apple.systempreferences:com.apple.Notifications-Settings.extension?id=\(self.queryApplicationIdentifier())")!)
    }
    return
  }

  // ----------------

  private func handleCheckApplicationExtension(
    name: String,
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
    name: String,
    state: Bool,
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
    type: String,
  ) async throws -> String {
    guard type == "user_home" || type == "application_shared" || type == "application_temporary" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    var target: String? = nil
    if type == "user_home" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.homeDirectoryForCurrentUser))"
    }
    if type == "application_shared" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!))/\(try self.queryApplicationIdentifier())"
    }
    if type == "application_temporary" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!))/\(try self.queryApplicationIdentifier())/temporary"
    }
    return target!
  }

  private func handleRevealStorageItem(
    target: String,
  ) async throws -> Void {
    let link = URL(fileURLWithPath: target)
    try await self.openExternalLink(link: link)
    return
  }

  @MainActor
  private func handlePickStorageItem(
    type: String,
    multiply: Bool,
    location: String,
    name: String,
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
    let target = try targetUrl.map({ (item) in try self.resolveFileUrl(url: item) })
    return target
  }

  // ----------------

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

  // MARK: - invoke

  private func invoke(
    method: String,
    argument: [String: Any?],
  ) async throws -> Void {
    self.channel?.invokeMethod(method, arguments: argument)
    return
  }

  // ----------------

  private func invokeReceiveApplicationLink(
    target: String,
  ) async throws -> Void {
    return try await self.invoke(method: "receive_application_link", argument: [
      "target": target,
    ])
  }

  // MARK: - utility

  private func encodeFlutterValue<TValue>(
    ripe: TValue,
  ) throws -> Any? {
    return ripe
  }

  private func decodeFlutterValue<TValue>(
    raw: Any?,
  ) throws -> TValue {
    let ripe = raw as? TValue
    let nilValue = Optional<Any>.none as? TValue 
    guard ripe != nil || nilValue != nil else {
      throw NSError(domain: "invalid value.", code: 0)
    }
    return ripe!
  }

  private func extractFlutterValueMap(
    map: inout [String: Any?],
    name: String,
  ) throws -> Any? {
    guard map.index(forKey: name) != nil else {
      throw NSError(domain: "invalid name.", code: 0)
    }
    return map[name] ?? nil
  }

  private func infuseFlutterValueMap(
    map: inout [String: Any?],
    name: String,
    value: Any?,
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

  private func openExternalLink(
    link: URL,
  ) async throws -> Void {
    guard NSWorkspace.shared.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
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
