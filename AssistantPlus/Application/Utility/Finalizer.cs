#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public partial class Finalizer : IDisposable {

		#region structor

		private Action mAction;

		// ----------------

		public Finalizer (
			Action action
		) {
			this.mAction = action;
			return;
		}

		#endregion

		#region implement IDisposable

		public void Dispose (
		) {
			this.mAction();
			GC.SuppressFinalize(this);
			return;
		}

		#endregion

	}

}
