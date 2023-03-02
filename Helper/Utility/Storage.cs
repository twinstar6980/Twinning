#pragma warning disable 0,
// ReSharper disable

using Helper;
using Windows.Storage.Pickers;

namespace Helper.Utility {

	public static class Storage {

		#region read & write

		public static async Task<String> LoadFileText (
			String path
		) {
			return await System.IO.File.ReadAllTextAsync(path);
		}

		#endregion

		#region pick

		public static async Task<String?> PickFile (
			Window rootWindow,
			String filter
		) {
			var picker = new FileOpenPicker() {
				FileTypeFilter = { filter },
			};
			var handle = WinRT.Interop.WindowNative.GetWindowHandle(rootWindow);
			WinRT.Interop.InitializeWithWindow.Initialize(picker, handle);
			var target = await picker.PickSingleFileAsync();
			return target?.Path;
		}

		public static async Task<String?> PickDirectory (
			Window rootWindow
		) {
			var picker = new FolderPicker() {
			};
			var handle = WinRT.Interop.WindowNative.GetWindowHandle(rootWindow);
			WinRT.Interop.InitializeWithWindow.Initialize(picker, handle);
			var target = await picker.PickSingleFolderAsync();
			return target?.Path;
		}

		#endregion

	}

}
