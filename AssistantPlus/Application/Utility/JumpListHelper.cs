#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Windows.UI.StartScreen;

namespace Twinning.AssistantPlus.Utility {

	public static class JumpListHelper {

		#region utility

		public static async Task Apply (
			List<JumpListItem> itemList
		) {
			#if DEBUG
			return;
			#endif
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
