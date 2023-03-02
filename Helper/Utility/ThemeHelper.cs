#pragma warning disable 0,
// ReSharper disable

using Helper;
using Windows.Storage;

namespace Helper.Utility {

	public static class ThemeHelper {

		#region utility

		private const String SettingKey = "Theme";

		// ----------------

		public static ElementTheme RootTheme {
			get {
				foreach (var window in WindowHelper.ActiveWindow) {
					if (window.Content is FrameworkElement rootElement) {
						return rootElement.RequestedTheme;
					}
				}
				return ElementTheme.Default;
			}
			set {
				foreach (var window in WindowHelper.ActiveWindow) {
					if (window.Content is FrameworkElement rootElement) {
						rootElement.RequestedTheme = value;
					}
				}
				ApplicationData.Current.LocalSettings.Values[ThemeHelper.SettingKey] = value.ToString();
				return;
			}
		}

		// ----------------

		public static void Initialize (
		) {
			var theme = ApplicationData.Current.LocalSettings.Values[ThemeHelper.SettingKey]?.ToString();
			if (theme != null) {
				ThemeHelper.RootTheme = Enum.Parse<ElementTheme>(theme);
			}
			return;
		}

		#endregion

	}

}
