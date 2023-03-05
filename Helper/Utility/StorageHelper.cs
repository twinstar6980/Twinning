#pragma warning disable 0,
// ReSharper disable

using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Graphics.Imaging;
using Helper;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Microsoft.UI.Xaml.Media.Imaging;

namespace Helper.Utility {

	public static class StorageHelper {

		#region text

		public static async Task WriteFileText (
			String path,
			String content
		) {
			await System.IO.File.WriteAllTextAsync(path, content);
			return;
		}

		public static async Task<String> ReadFileText (
			String path
		) {
			return await System.IO.File.ReadAllTextAsync(path);
		}

		#endregion

		#region image

		public static async Task<WriteableBitmap> ReadFileImage (
			String path
		) {
			var data = await System.IO.File.ReadAllBytesAsync(path);
			var stream = data.AsBuffer().AsStream().AsRandomAccessStream();
			var decoder = await BitmapDecoder.CreateAsync(stream);
			var image = new WriteableBitmap((Size)decoder.PixelWidth, (Size)decoder.PixelHeight);
			stream.Seek(0);
			await image.SetSourceAsync(stream);
			return image;
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
