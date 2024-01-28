import UIKit
import UniformTypeIdentifiers

class ActionViewController: UIViewController {

	// MARK: - implement - UIViewController

	public override func viewWillAppear(
		_ animated: Bool
	) -> Void {
		super.viewWillAppear(animated)
		Task {
			do {
				var selection: Array<String> = []
				for item in self.extensionContext!.inputItems as! [NSExtensionItem] {
					for provider in item.attachments! {
						let data = try await provider.loadItem(forTypeIdentifier: UTType.fileURL.identifier)
						guard let url = data as? URL else {
							throw NSError(domain: "unknown data", code: 0)
						}
						guard let urlComponent = NSURLComponents(url: url, resolvingAgainstBaseURL: true) else {
							throw NSError(domain: "invalid url", code: 0)
						}
						guard urlComponent.scheme == "file" && urlComponent.host == "" && urlComponent.port == nil && urlComponent.path != nil else {
							throw NSError(domain: "unknown url", code: 0)
						}
						var path = urlComponent.path!
						if path != "/" && path.last == "/" {
							path.removeLast()
						}
						selection.append(path)
					}
				}
				var command: Array<String> = []
				command.append("-additional_argument")
				command.append(contentsOf: selection)
				let link = "twinstar.toolkit.shell-gui:/run?\(command.map({ (item) in return "command=\(item.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed)!)" }).joined(separator: "&"))"
				if !self.openURL(url: URL(string: link)!) {
					throw NSError(domain: "failed to open application link", code: 0)
				}
				self.extensionContext!.completeRequest(returningItems: [], completionHandler: nil)
			}
			catch {
				let alter = UIAlertController(title: "Exception", message: error.localizedDescription, preferredStyle: .alert)
				alter.addAction(UIAlertAction(title: "Close", style: .destructive, handler: { (_) in
					self.extensionContext!.completeRequest(returningItems: [], completionHandler: nil)
					return
				}))
				self.present(alter, animated: true, completion: nil)
			}
		}
		return
	}

	// MARK: - utility

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

}
