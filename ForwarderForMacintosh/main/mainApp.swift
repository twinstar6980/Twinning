import SwiftUI

@main
struct mainApp: App {

	// MARK: - variable

	private let exception: String

	// MARK: - construct

 	public init(
 	) {
		do {
			guard let libraryDirectory = NSSearchPathForDirectoriesInDomains(.libraryDirectory, .userDomainMask, true).first else {
				throw NSError(domain: "failed to get library directory", code: 0)
			}
			let dataDirectory = "\(libraryDirectory)/Containers/\(Bundle.main.bundleIdentifier!).extension/Data/Documents"
			let scriptFile = "\(dataDirectory)/forward.sh"
			var pathIsDirectory = ObjCBool(false)
			if FileManager.default.fileExists(atPath: dataDirectory, isDirectory: &pathIsDirectory) {
				if !pathIsDirectory.boolValue {
					try FileManager.default.removeItem(atPath: scriptFile)
				}
			}
			if !FileManager.default.fileExists(atPath: dataDirectory) {
				try FileManager.default.createDirectory(atPath: dataDirectory, withIntermediateDirectories: true)
			}
			if FileManager.default.fileExists(atPath: scriptFile, isDirectory: &pathIsDirectory) {
				if pathIsDirectory.boolValue {
					try FileManager.default.removeItem(atPath: scriptFile)
				}
			}
			if !FileManager.default.fileExists(atPath: scriptFile) {
				FileManager.default.createFile(atPath: scriptFile, contents: nil)
			}
			if !FileManager.default.isExecutableFile(atPath: scriptFile) {
				try FileManager.default.setAttributes([.posixPermissions: NSNumber(0o744)], ofItemAtPath: scriptFile)
			}
			if !NSWorkspace.shared.selectFile(nil, inFileViewerRootedAtPath: dataDirectory) {
				throw NSError(domain: "failed to reveal data directory", code: 0)
			}
			exit(0)
		}
		catch {
			self.exception = error.localizedDescription
		}
 	}

 	// MARK: - view

	public var body: some Scene {
		WindowGroup {
			Text(self.exception)
		}
	}

}
