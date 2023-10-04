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
			var result = default(String?);
			var exception = default(Exception?);
			try {
				Invoker.Version(library);
				Invoker.Prepare(library);
				result = Invoker.Execute(library, host.Execute, script, argument);
			} catch (Exception e) {
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
