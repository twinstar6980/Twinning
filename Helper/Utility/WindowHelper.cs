#pragma warning disable 0,
// ReSharper disable

using Helper;
using Windows.Graphics;
using Microsoft.UI.Windowing;

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
			var ratio = WinUIEx.HwndExtensions.GetDpiForWindow(WindowHelper.Handle(window)) / 96.0;
			window.AppWindow.Move(new PointInt32((Int32)(x * ratio), (Int32)(y * ratio)));
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
			var ratio = WinUIEx.HwndExtensions.GetDpiForWindow(WindowHelper.Handle(window)) / 96.0;
			window.AppWindow.Resize(new SizeInt32((Int32)(width * ratio), (Int32)(height * ratio)));
			return;
		}

		public static void Minimize (
			Window window
		) {
			(window.AppWindow.Presenter as OverlappedPresenter)!.Minimize();
			return;
		}

		public static void Maximize (
			Window window
		) {
			(window.AppWindow.Presenter as OverlappedPresenter)!.Maximize();
			return;
		}

		// ----------------

		public static void AlwaysOnTop (
			Window  window,
			Boolean state
		) {
			(window.AppWindow.Presenter as OverlappedPresenter)!.IsAlwaysOnTop = state;
			return;
		}

		// ----------------

		public static void Theme (
			Window       window,
			ElementTheme theme
		) {
			if (window.Content is FrameworkElement content) {
				content.RequestedTheme = theme;
			}
			return;
		}

		// ----------------

		public static Window GetForElement (
			UIElement element
		) {
			return WindowHelper.Current.Find((value) => (value.Content.XamlRoot == element.XamlRoot)) ?? throw new Exception("Could not get window.");
		}

		public static Boolean ShowAsForeground (
			Window window
		) {
			var state = true;
			var handle = WindowHelper.Handle(window);
			state &= ExternalLibrary.User32.ShowWindow(handle, 0x00000009);
			state &= ExternalLibrary.User32.SetForegroundWindow(handle);
			return state;
		}

		#endregion

	}

}
