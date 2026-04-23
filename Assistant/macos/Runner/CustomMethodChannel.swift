import AppKit
import FlutterMacOS

class CustomMethodChannel: NSObject {

  // MARK: - variable

  private let host: MainFlutterWindow

  // MARK: - construct

  public init(
    host: MainFlutterWindow,
  ) {
    self.host = host
    return
  }

  // MARK: - register

  public func register_awakeFromNib(
  ) -> Void {
    FlutterMethodChannel(
      name: "\(try! self.queryApplicationIdentifier()).CustomMethodChannel",
      binaryMessenger: (self.host.contentViewController as! FlutterViewController).engine.binaryMessenger,
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
    guard type == "user_home" || type == "application_shared" || type == "application_cache" else {
      throw NSError(domain: "invalid type.", code: 0)
    }
    var target: String? = nil
    if type == "user_home" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.homeDirectoryForCurrentUser))"
    }
    if type == "application_shared" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!))/\(try self.queryApplicationIdentifier())"
    }
    if type == "application_cache" {
      target = "\(try self.resolveFileUrl(url: FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!))/\(try self.queryApplicationIdentifier())/cache"
    }
    return target!
  }

  private func handleRevealStorageItem(
    target: String,
  ) async throws -> Void {
    let link = URL(fileURLWithPath: target)
    try await self.openLink(link: link)
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
    guard NSWorkspace.shared.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
    }
    return
  }

}
