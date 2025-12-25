#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreModdingWorker {

	public static class ForwardHelper {

		#region utility

		public static async Task Forward (
			List<String> argument
		) {
			await App.Instance.HandleLaunch(
				ModuleHelper.Query(ModuleType.CoreModdingWorker).Name,
				ModuleType.CoreModdingWorker,
				["-additional_argument", ..argument]
			);
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

		public static String MakeMethodMaybeBatch (
			String  method,
			Boolean batch
		) {
			return $"{method}{(!batch ? "" : "!batch")}";
		}

		public static List<String> MakeArgumentForCommand (
			String?                     input,
			String?                     method,
			Dictionary<String, Object>? argument
		) {
			var command = new List<String>();
			command.Add(input ?? "?none");
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
