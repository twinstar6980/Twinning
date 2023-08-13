#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Helper;

namespace Helper.Bridge {

	public static class Launcher {

		#region function

		public static String Launch (
			Host         host,
			Library      library,
			String       script,
			List<String> argument
		) {
			host.Start();
			Invoker.Version(library);
			Invoker.Prepare(library);
			var callback = host.Execute;
			var result = Invoker.Execute(library, callback, script, argument);
			host.Finish();
			return result;
		}

		#endregion

	}

}
