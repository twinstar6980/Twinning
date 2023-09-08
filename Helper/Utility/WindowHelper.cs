#pragma warning disable 0,
// ReSharper disable

using Helper;
using Windows.Graphics;

namespace Helper.Utility {

	public static class WindowHelper {

		#region utility

		public static List<Window> Current { get; } = new List<Window>();

		// ----------------

		public static void Track (
			Window window
		) {
			window.Closed += (sender, args) => { WindowHelper.Current.Remove(window); };
			WindowHelper.Current.Add(window);
			return;
		}

		public static void Activate (
			Window window
		) {
			window.Activate();
			return;
		}

		// ----------------

		public static IntPtr Handle (
			Window window
		) {
			return WinRT.Interop.WindowNative.GetWindowHandle(window);
		}

		// ----------------

		public static void Title (
			Window window,
			String title
		) {
			window.AppWindow.Title = title;
			return;
		}

		public static void Icon (
			Window window,
			String icon
		) {
			window.AppWindow.SetIcon(icon);
			return;
		}

		// ----------------

		public static void Position (
			Window window,
			Size   x,
			Size   y
		) {
			var rate = WinUIEx.HwndExtensions.GetDpiForWindow(WindowHelper.Handle(window)) / 96.0;
			window.AppWindow.Move(new PointInt32((Int32)(x * rate), (Int32)(y * rate)));
			return;
		}

		public static void Center (
			Window window
		) {
			WinUIEx.WindowExtensions.CenterOnScreen(window);
			return;
		}

		public static void Size (
			Window window,
			Size   width,
			Size   height
		) {
			var rate = WinUIEx.HwndExtensions.GetDpiForWindow(WindowHelper.Handle(window)) / 96.0;
			window.AppWindow.Resize(new SizeInt32((Int32)(width * rate), (Int32)(height * rate)));
			return;
		}

		public static void Minimize (
			Window window
		) {
			WinUIEx.WindowExtensions.Minimize(window);
			return;
		}

		public static void Maximize (
			Window window
		) {
			WinUIEx.WindowExtensions.Maximize(window);
			return;
		}

		// ----------------

		public static void AlwaysOnTop (
			Window  window,
			Boolean state
		) {
			WinUIEx.WindowExtensions.SetIsAlwaysOnTop(window, state);
			return;
		}

		// ----------------

		public static Window GetForElement (
			UIElement element
		) {
			return WindowHelper.Current.Find((value) => (value.Content.XamlRoot == element.XamlRoot)) ?? throw new Exception("Could not get window.");
		}

		public static Boolean ApplyMicaBackdrop (
			Window window
		) {
			var state = new BackdropHelper(window).TryApplyMica();
			if (state) {
				if (window.Content is Panel content) {
					content.Background = null;
				}
			}
			return state;
		}

		#endregion

	}

}
