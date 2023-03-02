#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Helper.Core;

namespace Helper {

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
