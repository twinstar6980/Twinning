#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModdingWorker {

	public static class ForwardHelper {

		public static async Task<Boolean> Forward (
			List<String> argument
		) {
			var state = true;
			try {
				if (Setting.Data.ModdingWorker.AlternativeLaunchScript.Length == 0) {
					var option = new List<String>();
					option.Add("-AdditionalArgument");
					option.AddRange(argument);
					await App.MainWindow.InsertTabItem(ModuleType.ModdingWorker, option);
				}
				else {
					ProcessHelper.CreateProcessForCommandScript(Setting.Data.ModdingWorker.AlternativeLaunchScript, argument).Wait(0);
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Failed to forward.", e.ToString());
				state = false;
			}
			return state;
		}

		public static async Task<Boolean> Forward (
			List<List<String>> argument,
			Boolean            parallel
		) {
			var state = true;
			if (argument.Count != 0) {
				if (parallel) {
					foreach (var argumentItem in argument) {
						state &= await ForwardHelper.Forward(argumentItem);
						if (!state) {
							break;
						}
					}
				}
				else {
					state &= await ForwardHelper.Forward(argument.SelectMany(GF.ReturnSelf).ToList());
				}
			}
			return state;
		}

		// ----------------

		public static List<String> MakeArgumentForCommand (
			String? input,
			String? method,
			Object? argument
		) {
			var command = new List<String>();
			command.Add(input ?? "?");
			if (method is not null) {
				command.Add("-method");
				command.Add(method);
			}
			if (argument is not null) {
				command.Add("-argument");
				command.Add(JsonHelper.SerializeText(argument, false));
			}
			return command;
		}

	}

}