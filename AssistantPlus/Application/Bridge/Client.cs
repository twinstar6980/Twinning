#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.Bridge {

	public abstract class Client {

		#region constructor

		public Client (
		) {
			return;
		}

		#endregion

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
