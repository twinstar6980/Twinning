import AppKit
import FinderSync

class ForwarderFinderSync: FIFinderSync {

  // MARK: - variable

  // MARK: - construct

  public override init(
  ) {
    super.init()
    FIFinderSyncController.default().directoryURLs = [URL(fileURLWithPath: "/")]
    return
  }

  // MARK: - implement FIFinderSync

  public override func menu(
    for menuKind: FIMenuKind,
  ) -> NSMenu {
    let menu = NSMenu(title: "")
    let item = NSMenuItem(title: self.toolbarItemName, action: #selector(self.actionForward(_:)), keyEquivalent: "")
    item.image = self.toolbarItemImage
    menu.addItem(item)
    return menu
  }

  public override func beginObservingDirectory(
    at url: URL,
  ) -> Void {
    return
  }

  public override func endObservingDirectory(
    at url: URL,
  ) -> Void {
    return
  }

  public override func requestBadgeIdentifier(
    for url: URL,
  ) -> Void {
    return
  }

  public override var toolbarItemName: String {
    return (try? self.queryApplicationName()) ?? ""
  }

  public override var toolbarItemToolTip: String {
    return ""
  }

  public override var toolbarItemImage: NSImage {
    return (try? self.queryApplicationLogo()) ?? NSImage()
  }

  // MARK: - action

  @IBAction
  private func actionForward(
    _ sender: AnyObject?,
  ) -> Void {
    let resource = FIFinderSyncController.default().selectedItemURLs()!
    Task {
      do {
        try await self.forwardResource(resource)
      }
      catch {
        await self.showException(error)
      }
    }
    return
  }

  // MARK: - utility

  private func queryApplicationIdentifier(
  ) throws -> String {
    let extensionIdentifier = Bundle.main.bundleIdentifier
    guard extensionIdentifier != nil else {
      throw NSError(domain: "failed to get bundle identifier.", code: 0)
    }
    let extensionSuffix = ".Forwarder"
    guard extensionIdentifier!.hasSuffix(extensionSuffix) else {
      throw NSError(domain: "invalid bundle identifier.", code: 0)
    }
    let identifier = String(extensionIdentifier!.dropLast(extensionSuffix.count))
    return identifier
  }

  private func queryApplicationName(
  ) throws -> String {
    let extensionName = Bundle.main.object(forInfoDictionaryKey: "CFBundleDisplayName") as? String
    guard extensionName != nil else {
      throw NSError(domain: "failed to get bundle name.", code: 0)
    }
    let name = extensionName!
    return name
  }

  private func queryApplicationLogo(
  ) throws -> NSImage {
    let logo = NSImage(named: "Logo")
    guard logo != nil else {
      throw NSError(domain: "failed to get logo.", code: 0)
    }
    return logo!
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

  private func encodePercentString(
    _ source: String,
  ) throws -> String {
    let unreserved = NSMutableCharacterSet()
    unreserved.formUnion(with: .alphanumerics)
    unreserved.addCharacters(in: "-._~")
    let destination = source.addingPercentEncoding(withAllowedCharacters: unreserved as CharacterSet)
    guard destination != nil else {
      throw NSError(domain: "failed to encode percent string.", code: 0)
    }
    return destination!
  }

  private func openExternalLink(
    _ link: URL,
  ) async throws -> Void {
    guard NSWorkspace.shared.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
    }
    return
  }

  // ----------------

  private func showException(
    _ exception: Error,
  ) async -> Void {
    await MainActor.run {
      let alert = NSAlert()
      alert.messageText = "Exception"
      alert.informativeText = exception.localizedDescription
      alert.addButton(withTitle: "Close")
      alert.alertStyle = .critical
      alert.runModal()
    }
    return
  }

  private func forwardResource(
    _ resource: Array<URL>,
  ) async throws -> Void {
    var command: Array<String> = []
    command.append("-forward")
    command.append(contentsOf: try resource.map({ (item) in try self.resolveFileUrl(item) }))
    let link = URL(string: "\(try self.queryApplicationIdentifier()):/application?\(try command.map({ (item) in "command=\(try self.encodePercentString(item))" }).joined(separator: "&"))")!
    try await self.openExternalLink(link)
    return
  }

}
