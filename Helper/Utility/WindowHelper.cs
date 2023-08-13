#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class WindowHelper {

		#region utility

		public static List<Window> ActiveWindow { get; } = new List<Window>();

		// ----------------

		public static Window TrackWindow (
			Window window
		) {
			window.Closed += (sender, args) => { WindowHelper.ActiveWindow.Remove(window); };
			WindowHelper.ActiveWindow.Add(window);
			return window;
		}

		// ----------------

		public static Window? GetWindowForElement (
			UIElement element
		) {
			return WindowHelper.ActiveWindow.Find((value) => (value.Content.XamlRoot == element.XamlRoot));
		}

		#endregion

	}

}
