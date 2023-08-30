#pragma warning disable 0,
// ReSharper disable

using Helper;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Storage.Pickers;
using Windows.Graphics.Imaging;
using Microsoft.UI.Xaml.Media.Imaging;

namespace Helper.Utility {

	public static class StorageHelper {

		#region path

		public static String Normalize (
			String path
		) {
			return path.Replace('\\', '/');
		}

		// ----------------

		public static String? GetPathParent (
			String path
		) {
			var parent = Path.GetDirectoryName(path);
			return parent is null ? null : StorageHelper.Normalize(parent);
		}

		public static String GetPathName (
			String path
		) {
			var name = Path.GetFileName(path);
			return StorageHelper.Normalize(name);
		}

		#endregion

		#region exist

		public static Boolean Exist (
			String path
		) {
			return File.Exists(path) || Directory.Exists(path);
		}

		public static Boolean ExistFile (
			String path
		) {
			return File.Exists(path);
		}

		public static Boolean ExistDirectory (
			String path
		) {
			return Directory.Exists(path);
		}

		#endregion

		#region list

		public static List<String> ListFile (
			String parent,
			Size   depth,
			String pattern
		) {
			var parentFullName = new DirectoryInfo(parent).FullName;
			return Directory.EnumerateFiles(parent, pattern, new EnumerationOptions() { MaxRecursionDepth = depth }).Select((value) => (value[(parentFullName.Length + 1)..])).ToList();
		}

		#endregion

		#region text

		public static async Task WriteFileText (
			String path,
			String content
		) {
			await File.WriteAllTextAsync(path, content);
			return;
		}

		public static async Task<String> ReadFileText (
			String path
		) {
			return await File.ReadAllTextAsync(path);
		}

		public static String ReadFileTextSync (
			String path
		) {
			return File.ReadAllText(path);
		}

		#endregion

		#region image

		public static async Task<WriteableBitmap> ReadFileImage (
			String path
		) {
			var data = await File.ReadAllBytesAsync(path);
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
			String filter = "*"
		) {
			var picker = new FileOpenPicker() {
				FileTypeFilter = { filter },
			};
			var handle = WinRT.Interop.WindowNative.GetWindowHandle(rootWindow);
			WinRT.Interop.InitializeWithWindow.Initialize(picker, handle);
			var target = await picker.PickSingleFileAsync();
			return target is null ? null : StorageHelper.Normalize(target.Path);
		}

		public static async Task<String?> PickDirectory (
			Window rootWindow
		) {
			var picker = new FolderPicker() {
			};
			var handle = WinRT.Interop.WindowNative.GetWindowHandle(rootWindow);
			WinRT.Interop.InitializeWithWindow.Initialize(picker, handle);
			var target = await picker.PickSingleFolderAsync();
			return target is null ? null : StorageHelper.Normalize(target.Path);
		}

		#endregion

	}

}
