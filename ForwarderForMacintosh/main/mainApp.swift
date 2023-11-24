import SwiftUI

@main
struct mainApp: App {
	var error: Bool
	init() {
		self.error = false
		do {
			let dataDirectory = NSSearchPathForDirectoriesInDomains(.libraryDirectory, .userDomainMask, true).first! + "/Containers/" + Bundle.main.bundleIdentifier! + ".extension/Data/Documents"
			let scriptFile = dataDirectory + "/forward.sh"
			var pathIsDirectory = ObjCBool(false)
			if (FileManager.default.fileExists(atPath: dataDirectory, isDirectory: &pathIsDirectory)) {
				if (!pathIsDirectory.boolValue) {
					try FileManager.default.removeItem(atPath: scriptFile)
				}
			}
			if (!FileManager.default.fileExists(atPath: dataDirectory)) {
				try FileManager.default.createDirectory(atPath: dataDirectory, withIntermediateDirectories: true)
			}
			if (FileManager.default.fileExists(atPath: scriptFile, isDirectory: &pathIsDirectory)) {
				if (pathIsDirectory.boolValue) {
					try FileManager.default.removeItem(atPath: scriptFile)
				}
			}
			if (!FileManager.default.fileExists(atPath: scriptFile)) {
				FileManager.default.createFile(atPath: scriptFile, contents: nil)
			}
			if (!FileManager.default.isExecutableFile(atPath: scriptFile)) {
				try FileManager.default.setAttributes([.posixPermissions: NSNumber(0o744)], ofItemAtPath: scriptFile)
			}
			NSWorkspace.shared.selectFile(nil, inFileViewerRootedAtPath: dataDirectory)
		}
		catch {
			self.error = true
		}
		return
	}
	var body: some Scene {
		WindowGroup {
			Text(!self.error ? "succeeded" : "failed")
		}
	}
}
