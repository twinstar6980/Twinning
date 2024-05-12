#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ModdingWorker {

	public static class ForwardHelper {

		#region utility

		public static async Task<Boolean> Forward (
			List<String> argument
		) {
			var state = true;
			try {
				if (App.Setting.Data.ModdingWorker.AlternativeLaunchScript.Length == 0) {
					await App.MainWindow.InsertTabItem(ModuleHelper.Query(ModuleType.ModdingWorker).Name, ModuleType.ModdingWorker, ["-AdditionalArgument", ..argument]);
				}
				else {
					await ProcessHelper.CreateProcessForCommandScript(App.Setting.Data.ModdingWorker.AlternativeLaunchScript, argument, false);
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to forward.", e.ToString());
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

		#endregion

	}

}
