#pragma warning disable 0,
// ReSharper disable MemberHidesStaticFromOuterClass

using Helper;
using Helper.Utility;
using Windows.ApplicationModel;
using Microsoft.UI.Xaml.Media;
using Newtonsoft.Json;

namespace Helper {

	public class Setting {

		#region data

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class ApplicationSettingData {
			public required ElementTheme ThemeMode;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class ModdingWorkerSettingData {
			public required String       Kernel;
			public required String       Script;
			public required List<String> Argument;
			public required Boolean      AutomaticScroll;
			public required Boolean      ImmediateLaunch;
			public required String       AlternativeLaunchScript;
			public required String       MessageFontFamily;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class ModuleLauncherSettingData {
			public required List<View.ModuleLauncher.JumperConfiguration> ModuleJumperConfiguration;
			public required List<View.ModuleLauncher.JumperConfiguration> PinnedJumperConfiguration;
			public required List<View.ModuleLauncher.JumperConfiguration> RecentJumperConfiguration;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class ResourceForwarderSettingData {
			public required String  OptionConfiguration;
			public required Boolean AutomaticClose;
			public required Boolean ParallelExecute;
			public required Boolean EnableFilter;
			public required Boolean EnableBatch;
			public required Boolean RemainInput;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class CommandSenderSettingData {
			public required String MethodConfiguration;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class AnimationViewerSettingData {
			public required Boolean ImmediateSelect;
			public required Boolean AutomaticPlay;
			public required Boolean RepeatPlay;
			public required Boolean RemainFrameRate;
			public required Boolean ShowSpriteBoundary;
			public required String  ImageFilterRule;
			public required String  SpriteFilterRule;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class PackageBuilderSettingData {
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class SettingData {
			public required Integer                      Version;
			public required ApplicationSettingData       Application;
			public required ModuleLauncherSettingData    ModuleLauncher;
			public required ModdingWorkerSettingData     ModdingWorker;
			public required ResourceForwarderSettingData ResourceForwarder;
			public required CommandSenderSettingData     CommandSender;
			public required AnimationViewerSettingData   AnimationViewer;
			public required PackageBuilderSettingData    PackageBuilder;
		}

		#endregion

		#region loader

		public static SettingData Data = null!;

		public static readonly String File = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "Setting.json");

		// ----------------

		public static async Task Load (
		) {
			Setting.Data = await JsonHelper.DeserializeFile<SettingData>(Setting.File);
			return;
		}

		public static async Task Save (
		) {
			WindowHelper.Current.ForEach((item) => { WindowHelper.Theme(item, Setting.Data.Application.ThemeMode); });
			await App.Instance.RegisterShellJumpList();
			App.Instance.Resources["ModdingWorkerMessageFontFamily"] = Setting.Data.ModdingWorker.MessageFontFamily.Length == 0 ? FontFamily.XamlAutoFontFamily.Source : Setting.Data.ModdingWorker.MessageFontFamily;
			await JsonHelper.SerializeFile<SettingData>(Setting.File, Setting.Data);
			return;
		}

		public static async Task Initialize (
		) {
			try {
				await Setting.Load();
				GF.AssertTest(Setting.Data.Version == Package.Current.Id.Version.Major);
			}
			catch (Exception) {
				Setting.Data = new () {
					Version = Package.Current.Id.Version.Major,
					Application = new () {
						ThemeMode = ElementTheme.Default,
					},
					ModuleLauncher = new () {
						ModuleJumperConfiguration = [
							..View.ModuleInformationConstant.Value.Select((value) => (new View.ModuleLauncher.JumperConfiguration() {
								Title = value.Name,
								ModuleType = value.Type,
								ModuleOption = [],
								WindowOption = [],
							})),
						],
						PinnedJumperConfiguration = [],
						RecentJumperConfiguration = [],
					},
					ModdingWorker = new () {
						Kernel = "",
						Script = "",
						Argument = [],
						AutomaticScroll = true,
						ImmediateLaunch = true,
						AlternativeLaunchScript = "",
						MessageFontFamily = "",
					},
					ResourceForwarder = new () {
						OptionConfiguration = "",
						AutomaticClose = false,
						ParallelExecute = false,
						EnableFilter = true,
						EnableBatch = false,
						RemainInput = false,
					},
					CommandSender = new () {
						MethodConfiguration = "",
					},
					AnimationViewer = new () {
						ImmediateSelect = true,
						AutomaticPlay = true,
						RepeatPlay = true,
						RemainFrameRate = true,
						ShowSpriteBoundary = false,
						ImageFilterRule = "",
						SpriteFilterRule = "",
					},
					PackageBuilder = new () {
					},
				};
			}
			await Setting.Save();
			return;
		}

		#endregion

	}

}
