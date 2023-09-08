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
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerImmediateSelect), true.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerAutomaticPlay), true.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerRepeatPlay), true.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerRemainFrameRate), true.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerShowSpriteBoundary), false.ToString());
			ApplicationData.Current.LocalSettings.Values.TryAdd(nameof(Setting.AnimationViewerSpriteFilterRule), "");
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

		// ----------------

		public static Boolean AnimationViewerImmediateSelect {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerImmediateSelect)]?.ToString() is String value ? Boolean.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerImmediateSelect)] = value.ToString();
		}

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

		public static Boolean AnimationViewerShowSpriteBoundary {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerShowSpriteBoundary)]?.ToString() is String value ? Boolean.Parse(value) : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerShowSpriteBoundary)] = value.ToString();
		}

		public static String AnimationViewerSpriteFilterRule {
			get => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerSpriteFilterRule)]?.ToString() is String value ? value : throw new Exception();
			set => ApplicationData.Current.LocalSettings.Values[nameof(Setting.AnimationViewerSpriteFilterRule)] = value;
		}

		#endregion

	}

}
