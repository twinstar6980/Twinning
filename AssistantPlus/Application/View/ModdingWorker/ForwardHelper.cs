#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ModdingWorker {

	public static class ForwardHelper {

		#region utility

		public static async Task Forward (
			List<String> argument
		) {
			await App.MainWindow.InsertTabItem(new () {
				Title = ModuleHelper.Query(ModuleType.ModdingWorker).Name,
				Type = ModuleType.ModdingWorker,
				Option = ["-AdditionalArgument", ..argument],
			});
			return;
		}

		public static async Task ForwardMany (
			List<List<String>> argument,
			Boolean            parallel
		) {
			if (!parallel) {
				await ForwardHelper.Forward(argument.SelectMany((value) => (value)).ToList());
			}
			else {
				foreach (var argumentItem in argument) {
					await ForwardHelper.Forward(argumentItem);
				}
			}
			return;
		}

		// ----------------

		public static String MakeMethodForBatchable (
			String  method,
			Boolean enableBatch
		) {
			return $"{method}{(!enableBatch ? "" : ".batch")}";
		}

		public static List<String> MakeArgumentForCommand (
			String?                     input,
			String?                     method,
			Dictionary<String, Object>? argument
		) {
			var command = new List<String>();
			command.Add(input ?? "?");
			if (method != null) {
				command.Add("-method");
				command.Add(method);
			}
			if (argument != null) {
				command.Add("-argument");
				command.Add(JsonHelper.SerializeText(argument, false));
			}
			return command;
		}

		#endregion

	}

}
