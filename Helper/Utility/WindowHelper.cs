#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class WindowHelper {

		#region utility

		public static List<Window> ActiveWindow { get; } = new ();

		// ----------------

		public static Window TrackWindow (
			Window window
		) {
			window.Closed += (
				sender,
				args
			) => {
				WindowHelper.ActiveWindow.Remove(window);
			};
			WindowHelper.ActiveWindow.Add(window);
			return window;
		}

		// ----------------

		public static Window? GetWindowForElement (
			UIElement element
		) {
			if (element.XamlRoot != null) {
				foreach (var window in WindowHelper.ActiveWindow) {
					if (element.XamlRoot == window.Content.XamlRoot) {
						return window;
					}
				}
			}
			return null;
		}

		#endregion

	}

}
