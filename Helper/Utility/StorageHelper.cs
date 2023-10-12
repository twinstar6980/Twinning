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

		public static String Regularize (
			String path
		) {
			return StorageHelper.ToPosixStyle(path);
		}

		public static String ToPosixStyle (
			String path
		) {
			return path.Replace('\\', '/');
		}

		public static String ToWindowsStyle (
			String path
		) {
			return path.Replace('/', '\\');
		}

		// ----------------

		public static String? Parent (
			String path
		) {
			var parent = Path.GetDirectoryName(path);
			return parent is null ? null : StorageHelper.Regularize(parent);
		}

		public static String Name (
			String path
		) {
			var name = Path.GetFileName(path);
			return StorageHelper.Regularize(name);
		}

		// ----------------

		public static String Temporary (
		) {
			return StorageHelper.Regularize(Path.Combine(Path.GetTempPath(), Path.GetTempFileName()));
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

		#region general

		public static void CopyFile (
			String source,
			String destination
		) {
			File.Copy(source, destination, true);
			return;
		}

		public static void RemoveFile (
			String target
		) {
			File.Delete(target);
			return;
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

		// ----------------

		public static void WriteFileTextSync (
			String path,
			String content
		) {
			File.WriteAllText(path, content);
			return;
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
			Window host,
			String filter = "*"
		) {
			var picker = new FileOpenPicker() {
				FileTypeFilter = { filter },
			};
			WinRT.Interop.InitializeWithWindow.Initialize(picker, WindowHelper.Handle(host));
			var target = await picker.PickSingleFileAsync();
			return target is null ? null : StorageHelper.Regularize(target.Path);
		}

		public static async Task<String?> PickDirectory (
			Window host
		) {
			var picker = new FolderPicker() {
			};
			WinRT.Interop.InitializeWithWindow.Initialize(picker, WindowHelper.Handle(host));
			var target = await picker.PickSingleFolderAsync();
			return target is null ? null : StorageHelper.Regularize(target.Path);
		}

		#endregion

	}

}
