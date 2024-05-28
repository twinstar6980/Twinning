import UIKit
import UniformTypeIdentifiers

class ActionViewController: UIViewController {

	// MARK: - variable

	// MARK: - construct

	public required init?(
		coder: NSCoder
	) {
		super.init(coder: coder)
	}

	// MARK: - implement - UIViewController

	public override func viewWillAppear(
		_ animated: Bool
	) -> Void {
		super.viewWillAppear(animated)
		Task {
			do {
				var resource: Array<URL> = []
				for item in self.extensionContext!.inputItems as! [NSExtensionItem] {
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
				self.showException(exception: error, handler: { () in self.extensionContext!.completeRequest(returningItems: [], completionHandler: nil) })
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

	private func openURL(
		url: URL
	) -> Bool {
		var responder = self as UIResponder?
		while responder != nil {
			if let application = responder as? UIApplication {
				if application.canOpenURL(url) {
					let selector_openURL = sel_registerName("openURL:")
					if application.responds(to: selector_openURL) {
						application.perform(selector_openURL, with: url)
						return true
					}
				}
				break
			}
			responder = responder!.next
		}
		return false
	}

	private func showException(
		exception: Error,
		handler: @escaping (() -> Void)
	) -> Void {
		let alter = UIAlertController(title: "Exception", message: exception.localizedDescription, preferredStyle: .alert)
		alter.addAction(UIAlertAction(title: "Close", style: .destructive, handler: { (_) in handler() }))
		self.present(alter, animated: true, completion: nil)
		return
	}

	private func forwardResource(
		resource: Array<URL>
	) throws -> Void {
		var command: Array<String> = []
		command.append("-initial_tab")
		command.append("Resource Forwarder")
		command.append("resource_forwarder")
		command.append("-input")
		command.append(contentsOf: try resource.map({ (item) in try self.parsePathOfFileURL(url: item) }))
		let link = "twinstar.twinning.assistant:/run?\(command.map({ (item) in "command=\(item.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed)!)" }).joined(separator: "&"))"
		if !self.openURL(url: URL(string: link)!) {
			throw NSError(domain: "failed to open application link.", code: 0)
		}
		return
	}

}
