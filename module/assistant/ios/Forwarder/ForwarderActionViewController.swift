import UIKit
import UniformTypeIdentifiers

class ForwarderActionViewController: UIViewController {

  // MARK: - variable

  // MARK: - construct

  public override init(
    nibName nibNameOrNil: String?,
    bundle nibBundleOrNil: Bundle?,
  ) {
    super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
    return
  }

  public required init?(
    coder: NSCoder,
  ) {
    super.init(coder: coder)
    return
  }

  // MARK: - implement UIViewController

  public override func viewDidLoad(
  ) -> Void {
    super.viewDidLoad()
    Task {
      do {
        var resource: Array<URL> = []
        for item in self.extensionContext!.inputItems as! Array<NSExtensionItem> {
          for provider in item.attachments! {
            let data = try await provider.loadItem(forTypeIdentifier: UTType.fileURL.identifier)
            let url = data as? URL
            guard url != nil else {
              throw NSError(domain: "unknown data.", code: 0)
            }
            resource.append(url!)
          }
        }
        try await self.forwardResource(resource)
      }
      catch {
        await self.showException(error)
      }
      self.extensionContext!.completeRequest(returningItems: [], completionHandler: nil)
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
    var application: UIApplication? = nil
    var responder = self as UIResponder?
    while responder != nil {
      if responder is UIApplication {
        application = responder as? UIApplication
        break
      }
      responder = responder!.next
    }
    if application == nil {
      throw NSError(domain: "failed to get application.", code: 0)
    }
    guard await application!.open(link) else {
      throw NSError(domain: "failed to open link.", code: 0)
    }
    return
  }

  // ----------------

  private func showException(
    _ exception: Error,
  ) async -> Void {
    await withCheckedContinuation { (continuation) in
      let alter = UIAlertController(title: "Exception", message: exception.localizedDescription, preferredStyle: .alert)
      alter.addAction(UIAlertAction(title: "Close", style: .destructive, handler: { (_) in continuation.resume(returning: 0) }))
      self.present(alter, animated: true, completion: nil)
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
