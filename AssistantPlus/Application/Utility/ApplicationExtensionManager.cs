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

		public async Task<Boolean> CheckForwarder(
		) {
			var stateFile = $"{App.Instance.SharedDirectory}/forwarder";
			return await StorageHelper.Exist(stateFile);
		}

		public async Task ToggleForwarder(
			Boolean state
		) {
			var stateFile = $"{App.Instance.SharedDirectory}/forwarder";
			var exist = await StorageHelper.Exist(stateFile);
			if (!state && exist) {
				await StorageHelper.Remove(stateFile);
			}
			if (state && !exist) {
				await StorageHelper.CreateFile(stateFile);
			}
			return;
		}

		#endregion

	}

}
