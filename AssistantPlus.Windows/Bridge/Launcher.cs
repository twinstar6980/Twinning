#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	public static class Launcher {

		#region function

		public static unsafe List<String> Launch (
			Client       client,
			Library      library,
			String       script,
			List<String> argument
		) {
			var result = default(List<String>);
			var exception = default(Exception?);
			client.Start();
			try {
				var executorCallback = new ExecutorProxy();
				executorCallback.Value = (callbackProxy, argumentProxy, resultProxy) => {
					resultProxy.Value = client.Callback(argumentProxy.Value);
					return;
				};
				var executorArgument = new MessageProxy();
				executorArgument.Value.Add("execute");
				executorArgument.Value.Add(script);
				executorArgument.Value.AddRange(argument);
				var executorResult = new MessageProxy();
				ExecutorProxy.Parse(library.Symbol().executor).Value(
					executorCallback,
					executorArgument,
					executorResult
				);
				result = executorResult.Value;
			}
			catch (Exception e) {
				exception = e;
			}
			client.Finish();
			if (exception != null) {
				throw exception;
			}
			return result!;
		}

		#endregion

	}

}
