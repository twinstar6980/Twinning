import AppKit

let kApplicationName: String = "TwinStar ToolKit - Forwarder"

let kApplicationLogo: NSImage = NSImage(named: "Logo")!

func createProcess(program: String, argument: [String]) throws -> Void {
	let task = Process()
	task.executableURL = URL(filePath: program)
	task.arguments = argument
	try task.run()
	return
}
