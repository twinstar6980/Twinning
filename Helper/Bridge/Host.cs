#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Helper;
using Helper.Utility;

namespace Helper.Bridge {

	public abstract class Host {

		#region function

		public abstract void Start (
		);

		public abstract void Finish (
		);

		// ----------------

		public abstract List<String> Execute (
			List<String> argument
		);

		#endregion

	}

}
