#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Microsoft.UI.Windowing;

namespace Twinning.AssistantPlus.Utility {

	public static class WindowHelper {

		#region utility

		public static IntPtr GetHandle (
			Window window
		) {
			return WinRT.Interop.WindowNative.GetWindowHandle(window);
		}

		// ----------------

		public static void SetIcon (
			Window window,
			String icon
		) {
			window.AppWindow.SetIcon(icon);
			return;
		}

		public static void SetTitle (
			Window window,
			String title
		) {
			window.AppWindow.Title = title;
			return;
		}

		public static void SetTitleBar (
			Window     window,
			Boolean    extend,
			UIElement? element,
			Boolean?   mode
		) {
			if (!extend) {
				window.ExtendsContentIntoTitleBar = false;
				window.SetTitleBar(null);
			}
			else {
				window.ExtendsContentIntoTitleBar = true;
				window.SetTitleBar(element);
				window.AppWindow.TitleBar.PreferredHeightOption = mode == null ? TitleBarHeightOption.Collapsed : !mode.AsNotNull() ? TitleBarHeightOption.Standard : TitleBarHeightOption.Tall;
			}
			return;
		}

		// ----------------

		public static void SetPosition (
			Window window,
			Size   x,
			Size   y
		) {
			var handle = WindowHelper.GetHandle(window);
			var ratio = Win32.PInvoke.GetDpiForWindow(new (handle)) / 96.0;
			window.AppWindow.Move(new ((x * ratio).CastPrimitive<Int32>(), (y * ratio).CastPrimitive<Int32>()));
			return;
		}

		public static void SetSize (
			Window window,
			Size   width,
			Size   height
		) {
			var handle = WindowHelper.GetHandle(window);
			var ratio = Win32.PInvoke.GetDpiForWindow(new (handle)) / 96.0;
			window.AppWindow.Resize(new ((width * ratio).CastPrimitive<Int32>(), (height * ratio).CastPrimitive<Int32>()));
			return;
		}

		public static void SetAtCenter (
			Window window
		) {
			var handle = WindowHelper.GetHandle(window);
			var monitor = Win32.PInvoke.MonitorFromWindow(new (handle), Win32.Graphics.Gdi.MONITOR_FROM_FLAGS.MONITOR_DEFAULTTONEAREST);
			var monitorInformation = new Win32.Graphics.Gdi.MONITORINFO() {
				cbSize = 40u,
			};
			GF.AssertTest(Win32.PInvoke.GetMonitorInfo(monitor, ref monitorInformation));
			GF.AssertTest(Win32.PInvoke.GetWindowRect(new (handle), out var rect));
			var x = (monitorInformation.rcMonitor.left + monitorInformation.rcMonitor.right - (rect.right - rect.left)) / 2;
			var y = (monitorInformation.rcMonitor.bottom + monitorInformation.rcMonitor.top - (rect.bottom - rect.top)) / 2;
			window.AppWindow.Move(new (x, y));
			return;
		}

		// ----------------

		public static void SetAsForeground (
			Window window
		) {
			var handle = WindowHelper.GetHandle(window);
			GF.AssertTest(Win32.PInvoke.ShowWindow(new (handle), Win32.UI.WindowsAndMessaging.SHOW_WINDOW_CMD.SW_RESTORE));
			GF.AssertTest(Win32.PInvoke.SetForegroundWindow(new (handle)));
			return;
		}

		// ----------------

		public static void Activate (
			Window window
		) {
			window.Activate();
			return;
		}

		public static void Close (
			Window window
		) {
			window.Close();
			return;
		}

		#endregion

	}

}
