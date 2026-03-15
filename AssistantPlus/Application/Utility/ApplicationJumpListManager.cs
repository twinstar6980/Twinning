#pragma warning disable 0, CS0162
// ReSharper disable HeuristicUnreachableCode

using Twinning.AssistantPlus;
using Windows.UI.StartScreen;

namespace Twinning.AssistantPlus.Utility {

	public class ApplicationJumpListManager {

		#region singleton

		public static readonly ApplicationJumpListManager Instance = new ();

		#endregion

		#region constructor

		private Boolean mInitialized;

		// ----------------

		private ApplicationJumpListManager(
		) {
			this.mInitialized = false;
			return;
		}

		#endregion

		#region utility

		public async Task Initialize(
		) {
			AssertTest(!this.mInitialized);
			this.mInitialized = true;
			return;
		}

		// ----------------

		public async Task Apply(
			List<JumpListItem> itemList
		) {
			#if DEBUG
			return;
			#endif
			AssertTest(this.mInitialized);
			if (!JumpList.IsSupported()) {
				return;
			}
			var jumpList = await JumpList.LoadCurrentAsync();
			jumpList.Items.Clear();
			foreach (var item in itemList) {
				jumpList.Items.Add(item);
			}
			await jumpList.SaveAsync();
			return;
		}

		#endregion

	}

}
