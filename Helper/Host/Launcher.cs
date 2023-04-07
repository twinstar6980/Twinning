#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Helper.Core;

namespace Helper {

	public static class Launcher {

		#region function

		public static String Launch (
			Host         host,
			Library      core,
			String       script,
			List<String> argument
		) {
			host.Start();
			Invoker.Version(core);
			Invoker.Prepare(core);
			var callback = host.Execute;
			var result = Invoker.Execute(core, callback, script, argument);
			host.Finish();
			return result;
		}

		#endregion

	}

}
