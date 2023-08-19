import AppKit
import FinderSync

class ForwardFinderSync: FIFinderSync {
	
	override init() {
		super.init()
		FIFinderSyncController.default().directoryURLs = [URL(fileURLWithPath: "/")]
		return
	}
	
	// MARK: - Primary Finder Sync protocol methods
	
	override func beginObservingDirectory(at url: URL) {
		return
	}
	
	override func endObservingDirectory(at url: URL) {
		return
	}
	
	override func requestBadgeIdentifier(for url: URL) {
		return
	}
	
	// MARK: - Menu and toolbar item support
	
	override var toolbarItemName: String {
		return kApplicationName
	}
	
	override var toolbarItemToolTip: String {
		return ""
	}
	
	override var toolbarItemImage: NSImage {
		return kApplicationLogo
	}
	
	override func menu(for menuKind: FIMenuKind) -> NSMenu {
		let menu = NSMenu(title: "")
		let item = NSMenuItem(title: kApplicationName, action: #selector(self.forwardAction(_:)), keyEquivalent: "")
		item.image = kApplicationLogo
		menu.addItem(item)
		return menu
	}
	
	// MARK: - Action
	
	@IBAction func forwardAction(_ sender: AnyObject?) -> Void {
		do {
			let script = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).first! + "/forward.sh"
			let argument = FIFinderSyncController.default().selectedItemURLs()!.map { (selection) -> String in return selection.path }
			try createProcess(program: script, argument: argument)
		} catch {
		}
		return
	}
	
}
