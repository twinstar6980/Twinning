#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.Bridge {

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
				var executorArgument = new MessageProxy(["execute", script, ..argument]);
				var executorResult = new MessageProxy();
				ExecutorProxy.Parse(library.Symbol().executor).Value(executorCallback, executorArgument, executorResult);
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
