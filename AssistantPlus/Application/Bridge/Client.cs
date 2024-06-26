#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	public abstract class Client {

		#region life

		public abstract void Start (
		);

		public abstract void Finish (
		);

		// ----------------

		public abstract List<String> Callback (
			List<String> argument
		);

		#endregion

	}

}
