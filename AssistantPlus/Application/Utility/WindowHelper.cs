#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public static class WindowHelper {

		#region utility

		public static List<Window> Current { get; } = [];

		// ----------------

		public static void Track (
			Window window
		) {
			window.Closed += (_, _) => { WindowHelper.Current.Remove(window); };
			WindowHelper.Current.Add(window);
			return;
		}

		public static Window Find (
			UIElement element
		) {
			return WindowHelper.Current.Find((value) => (Object.ReferenceEquals(value.Content.XamlRoot, element.XamlRoot))) ?? throw new ("Could not find window.");
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
			window.AppWindow.Move(new ((x * ratio).CastPrimitive<Int32>(), (y * ratio).CastPrimitive<Int32>()));
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
			window.AppWindow.Resize(new ((width * ratio).CastPrimitive<Int32>(), (height * ratio).CastPrimitive<Int32>()));
			return;
		}

		// ----------------

		public static void Activate (
			Window window
		) {
			window.Activate();
			return;
		}

		public static Boolean ShowAsForeground (
			Window window
		) {
			var state = true;
			var handle = WindowHelper.Handle(window);
			state &= Win32.PInvoke.ShowWindow(new (handle), Win32.UI.WindowsAndMessaging.SHOW_WINDOW_CMD.SW_RESTORE);
			state &= Win32.PInvoke.SetForegroundWindow(new (handle));
			return state;
		}

		#endregion

	}

}
