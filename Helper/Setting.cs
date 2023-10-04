#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.Storage;

namespace Helper {

	public class Setting {

		#region utility

		public static void Initialize (
		) {
			Setting.InitializeValue(nameof(Setting.AppearanceThemeMode), ElementTheme.Default);
			Setting.InitializeValue(nameof(Setting.ModdingWorkerKernel), "");
			Setting.InitializeValue(nameof(Setting.ModdingWorkerScript), "");
			Setting.InitializeValue(nameof(Setting.ModdingWorkerArgument), new List<String>());
			Setting.InitializeValue(nameof(Setting.ModdingWorkerAutomaticClose), false);
			Setting.InitializeValue(nameof(Setting.ModdingWorkerAutomaticScroll), true);
			Setting.InitializeValue(nameof(Setting.ModdingWorkerImmediateLaunch), true);
			Setting.InitializeValue(nameof(Setting.ModdingWorkerAlternativeLaunchScript), "");
			Setting.InitializeValue(nameof(Setting.ResourceForwarderOptionConfiguration), "");
			Setting.InitializeValue(nameof(Setting.ResourceForwarderAutomaticClose), false);
			Setting.InitializeValue(nameof(Setting.ResourceForwarderParallelExecute), false);
			Setting.InitializeValue(nameof(Setting.ResourceForwarderEnableFilter), true);
			Setting.InitializeValue(nameof(Setting.ResourceForwarderEnableBatch), false);
			Setting.InitializeValue(nameof(Setting.ResourceForwarderRemainInput), false);
			Setting.InitializeValue(nameof(Setting.CommandSenderMethodConfiguration), "");
			Setting.InitializeValue(nameof(Setting.AnimationViewerImmediateSelect), true);
			Setting.InitializeValue(nameof(Setting.AnimationViewerAutomaticPlay), true);
			Setting.InitializeValue(nameof(Setting.AnimationViewerRepeatPlay), true);
			Setting.InitializeValue(nameof(Setting.AnimationViewerRemainFrameRate), true);
			Setting.InitializeValue(nameof(Setting.AnimationViewerShowSpriteBoundary), false);
			Setting.InitializeValue(nameof(Setting.AnimationViewerSpriteFilterRule), "");
			Setting.AppearanceThemeMode = Setting.AppearanceThemeMode;
			return;
		}

		// ----------------

		private static TValue GetValue<TValue> (
			String key
		) {
			var value = ApplicationData.Current.LocalSettings.Values[key]?.ToString();
			if (value is null) {
				throw new Exception();
			}
			return JsonHelper.Deserialize<TValue>(value);
		}

		private static void SetValue<TValue> (
			String key,
			TValue value
		) {
			ApplicationData.Current.LocalSettings.Values[key] = JsonHelper.Serialize<TValue>(value, false);
			return;
		}

		private static void InitializeValue<TValue> (
			String key,
			TValue value
		) {
			ApplicationData.Current.LocalSettings.Values.TryAdd(key, JsonHelper.Serialize<TValue>(value, false));
			return;
		}

		#endregion

		#region property

		public static ElementTheme AppearanceThemeMode {
			get {
				return Setting.GetValue<ElementTheme>(nameof(Setting.AppearanceThemeMode));
			}
			set {
				ThemeHelper.RootTheme = value;
				Setting.SetValue<ElementTheme>(nameof(Setting.AppearanceThemeMode), value);
				return;
			}
		}

		// ----------------

		public static String ModdingWorkerKernel {
			get => Setting.GetValue<String>(nameof(Setting.ModdingWorkerKernel));
			set => Setting.SetValue<String>(nameof(Setting.ModdingWorkerKernel), value);
		}

		public static String ModdingWorkerScript {
			get => Setting.GetValue<String>(nameof(Setting.ModdingWorkerScript));
			set => Setting.SetValue<String>(nameof(Setting.ModdingWorkerScript), value);
		}

		public static List<String> ModdingWorkerArgument {
			get => Setting.GetValue<List<String>>(nameof(Setting.ModdingWorkerArgument));
			set => Setting.SetValue<List<String>>(nameof(Setting.ModdingWorkerArgument), value);
		}

		public static Boolean ModdingWorkerAutomaticClose {
			get => Setting.GetValue<Boolean>(nameof(Setting.ModdingWorkerAutomaticClose));
			set => Setting.SetValue<Boolean>(nameof(Setting.ModdingWorkerAutomaticClose), value);
		}

		public static Boolean ModdingWorkerAutomaticScroll {
			get => Setting.GetValue<Boolean>(nameof(Setting.ModdingWorkerAutomaticScroll));
			set => Setting.SetValue<Boolean>(nameof(Setting.ModdingWorkerAutomaticScroll), value);
		}

		public static Boolean ModdingWorkerImmediateLaunch {
			get => Setting.GetValue<Boolean>(nameof(Setting.ModdingWorkerImmediateLaunch));
			set => Setting.SetValue<Boolean>(nameof(Setting.ModdingWorkerImmediateLaunch), value);
		}

		public static String ModdingWorkerAlternativeLaunchScript {
			get => Setting.GetValue<String>(nameof(Setting.ModdingWorkerAlternativeLaunchScript));
			set => Setting.SetValue<String>(nameof(Setting.ModdingWorkerAlternativeLaunchScript), value);
		}

		// ----------------

		public static String ResourceForwarderOptionConfiguration {
			get => Setting.GetValue<String>(nameof(Setting.ResourceForwarderOptionConfiguration));
			set => Setting.SetValue<String>(nameof(Setting.ResourceForwarderOptionConfiguration), value);
		}

		public static Boolean ResourceForwarderAutomaticClose {
			get => Setting.GetValue<Boolean>(nameof(Setting.ResourceForwarderAutomaticClose));
			set => Setting.SetValue<Boolean>(nameof(Setting.ResourceForwarderAutomaticClose), value);
		}

		public static Boolean ResourceForwarderParallelExecute {
			get => Setting.GetValue<Boolean>(nameof(Setting.ResourceForwarderParallelExecute));
			set => Setting.SetValue<Boolean>(nameof(Setting.ResourceForwarderParallelExecute), value);
		}

		public static Boolean ResourceForwarderEnableFilter {
			get => Setting.GetValue<Boolean>(nameof(Setting.ResourceForwarderEnableFilter));
			set => Setting.SetValue<Boolean>(nameof(Setting.ResourceForwarderEnableFilter), value);
		}

		public static Boolean ResourceForwarderEnableBatch {
			get => Setting.GetValue<Boolean>(nameof(Setting.ResourceForwarderEnableBatch));
			set => Setting.SetValue<Boolean>(nameof(Setting.ResourceForwarderEnableBatch), value);
		}

		public static Boolean ResourceForwarderRemainInput {
			get => Setting.GetValue<Boolean>(nameof(Setting.ResourceForwarderRemainInput));
			set => Setting.SetValue<Boolean>(nameof(Setting.ResourceForwarderRemainInput), value);
		}

		// ----------------

		public static String CommandSenderMethodConfiguration {
			get => Setting.GetValue<String>(nameof(Setting.CommandSenderMethodConfiguration));
			set => Setting.SetValue<String>(nameof(Setting.CommandSenderMethodConfiguration), value);
		}

		// ----------------

		public static Boolean AnimationViewerImmediateSelect {
			get => Setting.GetValue<Boolean>(nameof(Setting.AnimationViewerImmediateSelect));
			set => Setting.SetValue<Boolean>(nameof(Setting.AnimationViewerImmediateSelect), value);
		}

		public static Boolean AnimationViewerAutomaticPlay {
			get => Setting.GetValue<Boolean>(nameof(Setting.AnimationViewerAutomaticPlay));
			set => Setting.SetValue<Boolean>(nameof(Setting.AnimationViewerAutomaticPlay), value);
		}

		public static Boolean AnimationViewerRepeatPlay {
			get => Setting.GetValue<Boolean>(nameof(Setting.AnimationViewerRepeatPlay));
			set => Setting.SetValue<Boolean>(nameof(Setting.AnimationViewerRepeatPlay), value);
		}

		public static Boolean AnimationViewerRemainFrameRate {
			get => Setting.GetValue<Boolean>(nameof(Setting.AnimationViewerRemainFrameRate));
			set => Setting.SetValue<Boolean>(nameof(Setting.AnimationViewerRemainFrameRate), value);
		}

		public static Boolean AnimationViewerShowSpriteBoundary {
			get => Setting.GetValue<Boolean>(nameof(Setting.AnimationViewerShowSpriteBoundary));
			set => Setting.SetValue<Boolean>(nameof(Setting.AnimationViewerShowSpriteBoundary), value);
		}

		public static String AnimationViewerSpriteFilterRule {
			get => Setting.GetValue<String>(nameof(Setting.AnimationViewerSpriteFilterRule));
			set => Setting.SetValue<String>(nameof(Setting.AnimationViewerSpriteFilterRule), value);
		}

		#endregion

	}

}
