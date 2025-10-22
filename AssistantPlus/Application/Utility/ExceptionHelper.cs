#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;

namespace Twinning.AssistantPlus.Utility {

	public static class ExceptionHelper {

		#region utility

		private static Action<Exception> Handler { get; set; } = (_) => { };

		// ----------------

		public static void Initialize (
			App               application,
			Action<Exception> handler
		) {
			ExceptionHelper.Handler = handler;
			application.UnhandledException += async (_, args) => {
				args.Handled = true;
				ExceptionHelper.Handler(args.Exception);
				return;
			};
			TaskScheduler.UnobservedTaskException += async (_, args) => {
				args.SetObserved();
				ExceptionHelper.Handler(args.Exception);
				return;
			};
			return;
		}

		// ----------------

		public static Task WrapTask (
			Task task
		) {
			return task.ContinueWith((it) => {
				if (it.Exception?.InnerException != null) {
					ExceptionHelper.Handler(it.Exception.InnerException);
				}
				return;
			});
		}

		public static String GenerateMessage (
			Exception exception
		) {
			var message = $"{exception.Message}";
			var stack = new StackTrace(exception);
			if (exception.StackTrace != null) {
				foreach (var frame in exception.StackTrace.Split(Environment.NewLine)) {
					if (!frame.StartsWith("   at ")) {
						continue;
					}
					message += $"\n@ {frame.Substring("   at ".Length)}";
				}
			}
			return message;
		}

		#endregion

	}

}
