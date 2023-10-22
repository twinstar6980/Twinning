#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	#region type

	public class JumperConfiguration {

		public String       Title        = default!;
		public ModuleType   ModuleType   = default!;
		public List<String> ModuleOption = default!;
		public List<String> WindowOption = default!;

		// ----------------

		public static Boolean CompareForModule (
			JumperConfiguration thix,
			JumperConfiguration that
		) {
			return thix.ModuleType.Equals(that.ModuleType)
				&& thix.ModuleOption.SequenceEqual(that.ModuleOption);
		}

		public static List<String> GenerateArgument (
			JumperConfiguration configuration
		) {
			var argument = new List<String>();
			argument.AddRange(configuration.WindowOption);
			argument.Add("-ModuleType");
			argument.Add(configuration.ModuleType.ToString());
			argument.Add("-ModuleOption");
			argument.AddRange(configuration.ModuleOption);
			return argument;
		}

		public static void Launch (
			JumperConfiguration configuration,
			Boolean             asNewWindow
		) {
			if (asNewWindow) {
				ProcessHelper.CreateProcess(App.ProgramFile, JumperConfiguration.GenerateArgument(configuration)).Wait(0);
			} else {
				App.MainWindow.Controller.InsertTabItem(configuration.ModuleType, configuration.ModuleOption).Wait(0);
			}
			return;
		}

	}

	#endregion

}
