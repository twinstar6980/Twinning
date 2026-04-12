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
      name: "\(Bundle.main.bundleIdentifier!).CustomMethodChannel",
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

  @MainActor
  private func handlePickStorageItem(
    type: String,
    location: String,
    name: String,
  ) async throws -> String? {
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
      pickerView.allowsMultipleSelection = false
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
    var targetUrl = nil as URL?
    if (picker.runModal() == .OK) {
      targetUrl = picker.url
    }
    let target = targetUrl == nil ? nil : try self.getFileActualPath(url: targetUrl!)
    return target
  }

  // MARK: - utility

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

}
