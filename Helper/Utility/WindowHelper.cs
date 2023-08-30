#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class WindowHelper {

		#region utility

		public static List<Window> Active { get; } = new List<Window>();

		// ----------------

		public static void Track (
			Window window
		) {
			window.Closed += (sender, args) => { WindowHelper.Active.Remove(window); };
			WindowHelper.Active.Add(window);
			return;
		}

		// ----------------

		public static void Resize (
			Window window,
			Size   width,
			Size   height
		) {
			WinUIEx.WindowExtensions.SetWindowSize(window, width, height);
			return;
		}

		public static void Center (
			Window window
		) {
			WinUIEx.WindowExtensions.CenterOnScreen(window);
			return;
		}

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
			return WindowHelper.Active.Find((value) => (value.Content.XamlRoot == element.XamlRoot)) ?? throw new Exception("Could not get window.");
		}

		#endregion

	}

}
