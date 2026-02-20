#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Windows.UI.StartScreen;

namespace Twinning.AssistantPlus.Utility {

	public static class JumpListHelper {

		#region utility

		private static Boolean Initialized { get; set; } = false;

		// ----------------

		public static void Initialize (
		) {
			AssertTest(!JumpListHelper.Initialized);
			JumpListHelper.Initialized = true;
			return;
		}

		// ----------------

		public static async Task Apply (
			List<JumpListItem> itemList
		) {
			#if DEBUG
			return;
			#endif
			AssertTest(JumpListHelper.Initialized);
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
