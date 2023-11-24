#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Helper;
using Helper.Utility;

namespace Helper.Bridge {

	public static class Launcher {

		#region function

		public static String Launch (
			Host         host,
			Library      library,
			String       script,
			List<String> argument
		) {
			var result = default(String?);
			var exception = default(Exception?);
			host.Start();
			try {
				result = Invoker.Execute(library, host.Execute, script, argument);
			}
			catch (Exception e) {
				exception = e;
			}
			host.Finish();
			if (exception is not null) {
				throw exception;
			}
			return result!;
		}

		#endregion

	}

}
