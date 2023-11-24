#pragma warning disable 0,
// ReSharper disable

using Helper;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Storage.Pickers;
using Windows.Graphics.Imaging;
using Windows.Storage;
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

		// ----------------

		private static readonly Character[] InvalidPathNameCharacter = Path.GetInvalidFileNameChars();

		public static Boolean CheckName (
			String name
		) {
			if (name == "") {
				return false;
			}
			if (name[0] == ' ' || name.Last() == ' ' || name.Last() == '.') {
				return false;
			}
			return name.All((value) => (!StorageHelper.InvalidPathNameCharacter.Contains(value)));
		}

		#endregion

		#region general

		public static Boolean Exist (
			String target
		) {
			return File.Exists(target) || Directory.Exists(target);
		}

		public static Boolean ExistFile (
			String target
		) {
			return File.Exists(target);
		}

		public static Boolean ExistDirectory (
			String target
		) {
			return Directory.Exists(target);
		}

		// ----------------

		public static void CreateFile (
			String target
		) {
			File.Create(target).Close();
			return;
		}

		public static void CreateDirectory (
			String target
		) {
			Directory.CreateDirectory(target);
			return;
		}

		// ----------------

		public static void RemoveFile (
			String target
		) {
			File.Delete(target);
			return;
		}

		public static void RemoveDirectory (
			String target
		) {
			Directory.Delete(target, true);
			return;
		}

		// ----------------

		public static void RenameFile (
			String source,
			String destination
		) {
			File.Move(source, destination);
			return;
		}

		public static void RenameDirectory (
			String source,
			String destination
		) {
			Directory.Move(source, destination);
			return;
		}

		// ----------------

		public static void CopyFile (
			String source,
			String destination
		) {
			File.Copy(source, destination, true);
			return;
		}

		public static void CopyDirectory (
			String source,
			String destination
		) {
			if (!Directory.Exists(destination)) {
				Directory.CreateDirectory(destination);
			}
			foreach (var item in Directory.GetFiles(source)) {
				StorageHelper.CopyFile(item, $"{destination}/{StorageHelper.Name(item)}");
			}
			foreach (var item in Directory.GetDirectories(source)) {
				StorageHelper.CopyDirectory(item, $"{destination}/{StorageHelper.Name(item)}");
			}
			return;
		}

		// ----------------

		public static List<String> ListFile (
			String target,
			Size   depth,
			String pattern = "*"
		) {
			var parentFullName = new DirectoryInfo(target).FullName;
			return Directory.EnumerateFiles(target, pattern, new EnumerationOptions() { MaxRecursionDepth = depth }).Select((value) => (value[(parentFullName.Length + 1)..])).ToList();
		}

		public static List<String> ListDirectory (
			String target,
			Size   depth,
			String pattern = "*"
		) {
			var parentFullName = new DirectoryInfo(target).FullName;
			return Directory.EnumerateDirectories(target, pattern, new EnumerationOptions() { MaxRecursionDepth = depth }).Select((value) => (value[(parentFullName.Length + 1)..])).ToList();
		}

		#endregion

		#region text

		public static async Task WriteFileText (
			String target,
			String text
		) {
			await File.WriteAllTextAsync(target, text);
			return;
		}

		public static async Task<String> ReadFileText (
			String target
		) {
			return await File.ReadAllTextAsync(target);
		}

		// ----------------

		public static void WriteFileTextSync (
			String target,
			String content
		) {
			File.WriteAllText(target, content);
			return;
		}

		public static String ReadFileTextSync (
			String target
		) {
			return File.ReadAllText(target);
		}

		#endregion

		#region image

		public static async Task<WriteableBitmap> ReadFileImage (
			String target
		) {
			var data = await File.ReadAllBytesAsync(target);
			var stream = data.AsBuffer().AsStream().AsRandomAccessStream();
			var decoder = await BitmapDecoder.CreateAsync(stream);
			var image = new WriteableBitmap((Size)decoder.PixelWidth, (Size)decoder.PixelHeight);
			stream.Seek(0);
			await image.SetSourceAsync(stream);
			return image;
		}

		#endregion

		#region shell

		public static async Task RevealFile (
			String target
		) {
			await Windows.System.Launcher.LaunchFileAsync(await StorageFile.GetFileFromPathAsync(StorageHelper.ToWindowsStyle(target)));
			return;
		}

		public static async Task RevealDirectory (
			String target
		) {
			await Windows.System.Launcher.LaunchFolderAsync(await StorageFolder.GetFolderFromPathAsync(StorageHelper.ToWindowsStyle(target)));
			return;
		}

		// ----------------

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
