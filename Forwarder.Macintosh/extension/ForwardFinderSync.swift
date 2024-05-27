import AppKit
import FinderSync

class ForwardFinderSync: FIFinderSync {

	// MARK: - variable

	private let applicationName: String

	private let applicationLogo: NSImage

	// MARK: - construct

	public override init(
	) {
		self.applicationName = "TwinStar ToolKit - Forwarder"
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
			let script = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).first! + "/forward.sh"
			let argument = FIFinderSyncController.default().selectedItemURLs()!.map({ (item) in item.path })
			let process = Process()
			process.executableURL = URL(filePath: script)
			process.arguments = argument
			try process.run()
		}
		catch {
		}
		return
	}

}
