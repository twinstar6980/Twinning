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
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AppearanceThemeMode), ElementTheme.Default.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.CommandForwarderLaunchScript), "");
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.CommandForwarderCommandConfiguration), "");
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.CommandForwarderQuickWindowSizeWidth), 480.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.CommandForwarderQuickWindowSizeHeight), 960.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerAutomaticPlay), true.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerRepeatPlay), true.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerRemainFrameRate), true.ToString());
			Setting.AppearanceThemeMode = Setting.AppearanceThemeMode;
			return;
		}

		#endregion

		#region property

		public static ElementTheme AppearanceThemeMode {
			get {
				return ApplicationData.Current.LocalSettings.Values[nameof(Setting.AppearanceThemeMode)]?.ToString() is String value ? Enum.Parse<ElementTheme>(value) : throw new Exception();
			}
			set {
				ThemeHelper.RootTheme = value;
				ApplicationData.Current.LocalSettings.Values[nameof(Setting.AppearanceThemeMode)] = value.ToString();
				return;
			}
		}

		// ----------------

		public static String CommandForwarderLaunchScript {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderLaunchScript)]?.ToString() is String value ? value : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderLaunchScript)] = value;
		}

		public static String CommandForwarderCommandConfiguration {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderCommandConfiguration)]?.ToString() is String value ? value : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderCommandConfiguration)] = value;
		}

		public static Size CommandForwarderQuickWindowSizeWidth {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderQuickWindowSizeWidth)]?.ToString() is String value ? Size.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderQuickWindowSizeWidth)] = value;
		}

		public static Size CommandForwarderQuickWindowSizeHeight {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderQuickWindowSizeHeight)]?.ToString() is String value ? Size.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.CommandForwarderQuickWindowSizeHeight)] = value;
		}

		// ----------------

		public static Boolean AnimationViewerAutomaticPlay {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerAutomaticPlay)]?.ToString() is String value ? Boolean.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerAutomaticPlay)] = value.ToString();
		}

		public static Boolean AnimationViewerRepeatPlay {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRepeatPlay)]?.ToString() is String value ? Boolean.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRepeatPlay)] = value.ToString();
		}

		public static Boolean AnimationViewerRemainFrameRate {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRemainFrameRate)]?.ToString() is String value ? Boolean.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerRemainFrameRate)] = value.ToString();
		}

		#endregion

	}

}
