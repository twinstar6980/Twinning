#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;

namespace Twinning.AssistantPlus.Utility {

	public class ApplicationExtensionManager {

		#region singleton

		public static readonly ApplicationExtensionManager Instance = new ();

		#endregion

		#region constructor

		// ----------------

		private ApplicationExtensionManager(
		) {
			return;
		}

		#endregion

		#region utility

		public Boolean CheckForwarder(
		) {
			var stateFile = $"{App.Instance.SharedDirectory}/forwarder";
			return StorageHelper.Exist(stateFile);
		}

		public void ToggleForwarder(
			Boolean state
		) {
			var stateFile = $"{App.Instance.SharedDirectory}/forwarder";
			var exist = StorageHelper.Exist(stateFile);
			if (!state && exist) {
				StorageHelper.Remove(stateFile);
			}
			if (state && !exist) {
				StorageHelper.CreateFile(stateFile);
			}
			return;
		}

		#endregion

	}

}
