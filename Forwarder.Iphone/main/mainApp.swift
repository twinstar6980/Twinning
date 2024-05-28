import SwiftUI

@main
struct mainApp: App {

	// MARK: - variable

	// MARK: - construct

	public init(
	) {
	}

	// MARK: - view

	public var body: some Scene {
		WindowGroup() {
			Button(
				action: { () in
					do {
						try self.forwardResource(resource: [])
					}
					catch {
						self.showException(exception: error, handler: { () in })
					}
				}
			) {
				Text(
					verbatim: "Forward"
				)
			}.buttonStyle(.borderedProminent)
		}
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
		if UIApplication.shared.canOpenURL(url) {
			UIApplication.shared.open(url)
			return true
		}
		return false
	}

	private func showException(
		exception: Error,
		handler: @escaping (() -> Void)
	) -> Void {
		let rootView = UIApplication.shared.connectedScenes
			.compactMap({ (item) in item as? UIWindowScene })
			.filter({ (item) in item.activationState == .foregroundActive })
			.first?.keyWindow?.rootViewController
		let alter = UIAlertController(title: "Exception", message: exception.localizedDescription, preferredStyle: .alert)
		alter.addAction(UIAlertAction(title: "Close", style: .destructive, handler: { (_) in handler() }))
		rootView?.present(alter, animated: true, completion: nil)
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
