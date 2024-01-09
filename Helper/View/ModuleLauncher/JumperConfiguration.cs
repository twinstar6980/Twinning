#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.ModuleLauncher {

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
			return [
				..configuration.WindowOption,
				"-ModuleType",
				configuration.ModuleType.ToString(),
				"-ModuleOption",
				..configuration.ModuleOption,
			];
		}

		public static async Task Launch (
			JumperConfiguration configuration,
			Boolean             forNewWindow
		) {
			if (forNewWindow) {
				await ProcessHelper.CreateProcess(App.ProgramFile, JumperConfiguration.GenerateArgument(configuration), false);
			}
			else {
				await App.MainWindow.InsertTabItem(configuration.ModuleType, configuration.ModuleOption);
			}
			return;
		}

	}

	#endregion

}
