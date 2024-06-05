import SwiftUI

@main
struct MainApp: App {

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
						self.showException(exception: error, action: { () in })
					}
				}
			) {
				Text(
					verbatim: "Forward"
				)
			}
			.buttonStyle(.borderedProminent)
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
		NSWorkspace.shared.open(link)
		return
	}

	private func forwardResource(
		resource: Array<URL>
	) throws -> Void {
		var command: Array<String> = []
		command.append("-insert_tab")
		command.append("Resource Forwarder")
		command.append("resource_forwarder")
		command.append("-input")
		command.append(contentsOf: try resource.map({ (item) in try self.parsePathOfFileURL(url: item) }))
		let link = URL(string: "twinstar.twinning.assistant:/launch?\(try command.map({ (item) in "command=\(try self.encodePercentString(source: item))" }).joined(separator: "&"))")!
		try self.openLink(link: link)
		return
	}

	private func showException(
		exception: Error,
		action: @escaping (() -> Void)
	) -> Void {
		DispatchQueue.main.async {
			let alert = NSAlert()
			alert.messageText = "Exception"
			alert.informativeText = exception.localizedDescription
			alert.addButton(withTitle: "Close")
			alert.alertStyle = .critical
			alert.runModal()
			action()
		}
		return
	}

}
