#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.Storage;

namespace Helper {

	public class Setting {

		#region initialize

		public static void Initialize (
		) {
			Setting.AppearanceThemeMode = Setting.AppearanceThemeMode;
			return;
		}

		#endregion

		#region property

		public static ElementTheme AppearanceThemeMode {
			get {
				return Enum.Parse<ElementTheme>(ApplicationData.Current.LocalSettings.Values[nameof(Setting.AppearanceThemeMode)]?.ToString() ?? ElementTheme.Default.ToString());
			}
			set {
				ThemeHelper.RootTheme = value;
				ApplicationData.Current.LocalSettings.Values[nameof(Setting.AppearanceThemeMode)] = value.ToString();
				return;
			}
		}

		// ----------------

		public static String CommandForwarderLaunchScript {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderLaunchScript)]?.ToString() is String value ? value : String.Empty;
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderLaunchScript)] = value;
		}

		public static String CommandForwarderMethodConfiguration {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderMethodConfiguration)]?.ToString() is String value ? value : String.Empty;
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderMethodConfiguration)] = value;
		}

		// ----------------

		public static Boolean AnimationViewerAutomaticPlay {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerAutomaticPlay)]?.ToString() is String value ? Boolean.Parse(value) : true;
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerAutomaticPlay)] = value.ToString();
		}

		public static Boolean AnimationViewerRepeatPlay {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRepeatPlay)]?.ToString() is String value ? Boolean.Parse(value) : true;
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRepeatPlay)] = value.ToString();
		}

		public static Boolean AnimationViewerRemainFrameRate {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRemainFrameRate)]?.ToString() is String value ? Boolean.Parse(value) : true;
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRemainFrameRate)] = value.ToString();
		}

		#endregion

	}

}
