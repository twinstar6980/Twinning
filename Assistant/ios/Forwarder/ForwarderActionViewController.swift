import UIKit
import UniformTypeIdentifiers

class ForwarderActionViewController: UIViewController {

	// MARK: - variable

	// MARK: - construct

	public override init(
		nibName nibNameOrNil: String?,
		bundle nibBundleOrNil: Bundle?
	) {
		super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
	}

	public required init?(
		coder: NSCoder
	) {
		super.init(coder: coder)
	}

	// MARK: - implement - UIViewController

	public override func viewDidLoad(
	) -> Void {
		super.viewDidLoad()
		Task {
			do {
				var resource: Array<URL> = []
				for item in self.extensionContext!.inputItems as! Array<NSExtensionItem> {
					for provider in item.attachments! {
						let data = try await provider.loadItem(forTypeIdentifier: UTType.fileURL.identifier)
						guard let url = data as? URL else {
							throw NSError(domain: "unknown data.", code: 0)
						}
						resource.append(url)
					}
				}
				try self.forwardResource(resource: resource)
				self.extensionContext!.completeRequest(returningItems: [], completionHandler: nil)
			}
			catch {
				self.showException(exception: error, action: { () in self.extensionContext!.completeRequest(returningItems: [], completionHandler: nil) })
			}
		}
		return
	}

	// MARK: - utility

	private func parsePathOfFileURL(
		url: URL
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

	private func encodePercentString(
		source: String
	) throws -> String {
		let unreserved = NSMutableCharacterSet()
		unreserved.formUnion(with: .alphanumerics)
		unreserved.addCharacters(in: "-._~")
		guard let destination = source.addingPercentEncoding(withAllowedCharacters: unreserved as CharacterSet) else {
			throw NSError(domain: "failed to encode percent string.", code: 0)
		}
		return destination
	}

	private func openLink(
		link: URL
	) throws -> Void {
		var accepted = false
		var responder = self as UIResponder?
		while responder != nil {
			if let application = responder as? UIApplication {
				if application.canOpenURL(link) {
					let selector_openURL = sel_registerName("openURL:")
					if application.responds(to: selector_openURL) {
						application.perform(selector_openURL, with: link)
						accepted = true
					}
				}
				break
			}
			responder = responder!.next
		}
		if !accepted {
			throw NSError(domain: "failed to open link.", code: 0)
		}
		return
	}

	private func forwardResource(
		resource: Array<URL>
	) throws -> Void {
		var command: Array<String> = []
		command.append("-forward")
		command.append(contentsOf: try resource.map({ (item) in try self.parsePathOfFileURL(url: item) }))
		let link = URL(string: "twinstar.twinning.assistant:/application?\(try command.map({ (item) in "command=\(try self.encodePercentString(source: item))" }).joined(separator: "&"))")!
		try self.openLink(link: link)
		return
	}

	private func showException(
		exception: Error,
		action: @escaping (() -> Void)
	) -> Void {
		let alter = UIAlertController(title: "Exception", message: exception.localizedDescription, preferredStyle: .alert)
		alter.addAction(UIAlertAction(title: "Close", style: .destructive, handler: { (_) in action() }))
		self.present(alter, animated: true, completion: nil)
		return
	}

}
