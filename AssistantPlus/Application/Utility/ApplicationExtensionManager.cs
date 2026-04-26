#pragma warning disable 0,

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
			var stateFile = (await StorageHelper.Query(StorageQueryType.ApplicationShared)).Join("forwarder");
			return await StorageHelper.Exist(stateFile);
		}

		public async Task ToggleForwarder(
			Boolean state
		) {
			var stateFile = (await StorageHelper.Query(StorageQueryType.ApplicationShared)).Join("forwarder");
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
