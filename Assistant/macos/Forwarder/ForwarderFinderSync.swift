import AppKit
import FinderSync

class ForwarderFinderSync: FIFinderSync {

	// MARK: - variable

	private let applicationName: String

	private let applicationLogo: NSImage

	// MARK: - construct

	public override init(
	) {
		self.applicationName = "Twinning Assistant"
		self.applicationLogo = NSImage(named: "Logo")!
		super.init()
		FIFinderSyncController.default().directoryURLs = [URL(fileURLWithPath: "/")]
	}

	// MARK: - implement - FIFinderSync

	public override func menu(
		for menuKind: FIMenuKind
	) -> NSMenu {
		let menu = NSMenu(title: "")
		let item = NSMenuItem(title: self.applicationName, action: #selector(self.actionForward(_:)), keyEquivalent: "")
		item.image = self.applicationLogo
		menu.addItem(item)
		return menu
	}

	public override func beginObservingDirectory(
		at url: URL
	) -> Void {
		return
	}

	public override func endObservingDirectory(
		at url: URL
	) -> Void {
		return
	}

	public override func requestBadgeIdentifier(
		for url: URL
	) -> Void {
		return
	}

	public override var toolbarItemName: String {
		return self.applicationName
	}

	public override var toolbarItemToolTip: String {
		return ""
	}

	public override var toolbarItemImage: NSImage {
		return self.applicationLogo
	}

	// MARK: - action

	@IBAction
	private func actionForward(
		_ sender: AnyObject?
	) -> Void {
		do {
			let resource = FIFinderSyncController.default().selectedItemURLs()!
			try self.forwardResource(resource: resource)
		}
		catch {
			self.showException(exception: error, action: { () in })
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
		NSWorkspace.shared.open(link)
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
