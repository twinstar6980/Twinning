#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class ProcessHelper {

		#region command line

		private static void EncodeCommandLineProgramString (
			String        source,
			StringBuilder destination
		) {
			destination.Append('"');
			foreach (var element in source) {
				if (element == '/') {
					destination.Append('\\');
				} else {
					destination.Append(element);
				}
			}
			destination.Append('"');
			return;
		}

		private static void EncodeCommandLineArgumentString (
			String        source,
			StringBuilder destination
		) {
			var currentBackslashCount = 0;
			destination.Append('"');
			foreach (var element in source) {
				if (element == '"') {
					for (var index = 0; index < currentBackslashCount; index++) {
						destination.Append('\\');
					}
					destination.Append('\\');
				}
				destination.Append(element);
				if (element == '\\') {
					currentBackslashCount += 1;
				} else {
					currentBackslashCount = 0;
				}
			}
			for (var index = 0; index < currentBackslashCount; index++) {
				destination.Append('\\');
			}
			destination.Append('"');
			return;
		}

		// ----------------

		public static String EncodeCommandLineString (
			String       program,
			List<String> argument
		) {
			var destination = new StringBuilder();
			ProcessHelper.EncodeCommandLineProgramString(program, destination);
			foreach (var element in argument) {
				destination.Append(' ');
				ProcessHelper.EncodeCommandLineArgumentString(element, destination);
			}
			return destination.ToString();
		}

		#endregion

		#region create

		public static Task CreateProcess (
			String       program,
			List<String> argument
		) {
			var process = new Process();
			process.StartInfo.FileName = program;
			process.StartInfo.Arguments = ProcessHelper.EncodeCommandLineString(program, argument);
			var stared = process.Start();
			if (!stared) {
				throw new Exception();
			}
			return process.WaitForExitAsync();
		}

		// ----------------

		public static Task CreateProcessForCommandScript (
			String       script,
			List<String> argument
		) {
			if (!StorageHelper.ExistFile(script)) {
				throw new FileNotFoundException($"Could not find file '{script}'.", script);
			}
			var processProgram = "C:\\Windows\\System32\\cmd.exe";
			var processArgument = new List<String>();
			processArgument.Add("/C");
			processArgument.Add(script);
			processArgument.AddRange(argument);
			return ProcessHelper.CreateProcess(processProgram, processArgument);
		}

		#endregion

	}

}
