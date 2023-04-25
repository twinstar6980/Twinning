
import Cocoa
import FinderSync

let kApplicationName: String = "TwinStar ToolKit - Forwarder"

func execute(script: String, argument: [String]) -> Void {
	let task = Process()
	task.environment = ProcessInfo.processInfo.environment
	task.launchPath = script
	task.arguments = argument
	task.launch()
	return
}

func execute(argument: [String]) -> Void {
	return execute(script: NSHomeDirectory() + "/launch.sh", argument: argument)
}

class FinderSync: FIFinderSync {

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
		return NSImage(named: "BarIcon")!
	}
	
	override func menu(for menuKind: FIMenuKind) -> NSMenu {
		let menu = NSMenu(title: "")
		let item = NSMenuItem(title: kApplicationName, action: #selector(forwardAction(_:)), keyEquivalent: "")
		item.image = NSImage(named: "BarIcon")!
		menu.addItem(item)
		return menu
	}
	
	@IBAction func forwardAction(_ sender: AnyObject?) -> Void {
		let argument = FIFinderSyncController.default().selectedItemURLs()!.map { (selection) -> String in
			return selection.path
		}
		execute(argument: argument)
		return
	}
	
}
