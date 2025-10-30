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
		return (try? self.getApplicationName()) ?? ""
	}

	public override var toolbarItemToolTip: String {
		return ""
	}

	public override var toolbarItemImage: NSImage {
		return (try? self.getApplicationLogo()) ?? NSImage()
	}

	// MARK: - action

	@IBAction
	private func actionForward(
		_ sender: AnyObject?,
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

	private func getApplicationIdentifier(
	) throws -> String {
		guard let extensionIdentifier = Bundle.main.bundleIdentifier else {
			throw NSError(domain: "failed to get extension identifier.", code: 0)
		}
		let extensionSuffix = ".Forwarder"
		guard extensionIdentifier.hasSuffix(extensionSuffix) else {
			throw NSError(domain: "unknown extension identifier.", code: 0)
		}
		return String(extensionIdentifier.prefix(extensionIdentifier.count - extensionSuffix.count))
	}

	private func getApplicationName(
	) throws -> String {
		guard let extensionName = Bundle.main.object(forInfoDictionaryKey: "CFBundleDisplayName") as? String else {
			throw NSError(domain: "failed to get extension name.", code: 0)
		}
		return extensionName
	}

	private func getApplicationLogo(
	) throws -> NSImage {
		return NSImage(named: "Logo")!
	}

	// ----------------

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

	private func encodePercentString(
		source: String,
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
		link: URL,
	) throws -> Void {
		NSWorkspace.shared.open(link)
		return
	}

	// ----------------

	private func showException(
		exception: Error,
		action: @escaping (() -> Void),
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

	private func forwardResource(
		resource: Array<URL>,
	) throws -> Void {
		var command: Array<String> = []
		command.append("-forward")
		command.append(contentsOf: try resource.map({ (item) in try self.getFileActualPath(url: item) }))
		let link = URL(string: "\(try self.getApplicationIdentifier()):/application?\(try command.map({ (item) in "command=\(try self.encodePercentString(source: item))" }).joined(separator: "&"))")!
		try self.openLink(link: link)
		return
	}

}
