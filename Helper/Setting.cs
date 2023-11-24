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
			public required List<Module.ModuleLauncher.JumperConfiguration> ModuleJumperConfiguration;
			public required List<Module.ModuleLauncher.JumperConfiguration> PinnedJumperConfiguration;
			public required List<Module.ModuleLauncher.JumperConfiguration> RecentJumperConfiguration;
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
			public required String  SpriteFilterRule;
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
		}

		#endregion

		#region loader

		public static SettingData Data = null!;

		public static readonly String File = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "Setting.json");

		// ----------------

		public static void Load (
		) {
			Setting.Data = JsonHelper.DeserializeFileSync<SettingData>(Setting.File);
			return;
		}

		public static void Save (
		) {
			WindowHelper.Current.ForEach((item) => { WindowHelper.Theme(item, Setting.Data.Application.ThemeMode); });
			App.Instance.RegisterShellJumpList().Wait(0);
			App.Instance.Resources["ModdingWorkerMessageFontFamily"] = Setting.Data.ModdingWorker.MessageFontFamily.Length == 0 ? FontFamily.XamlAutoFontFamily.Source : Setting.Data.ModdingWorker.MessageFontFamily;
			JsonHelper.SerializeFileSync<SettingData>(Setting.File, Setting.Data);
			return;
		}

		public static void Initialize (
		) {
			try {
				Setting.Load();
				if (Setting.Data.Version != Package.Current.Id.Version.Major) {
					throw new Exception();
				}
			}
			catch (Exception) {
				Setting.Data = new SettingData() {
					Version = Package.Current.Id.Version.Major,
					Application = new ApplicationSettingData() {
						ThemeMode = ElementTheme.Default,
					},
					ModuleLauncher = new ModuleLauncherSettingData() {
						ModuleJumperConfiguration = new List<Module.ModuleLauncher.JumperConfiguration>(Module.ModuleInformationConstant.Value.Select((value) => (new Module.ModuleLauncher.JumperConfiguration() {
							Title = value.Name,
							ModuleType = value.Type,
							ModuleOption = new List<String>(),
							WindowOption = new List<String>(),
						}))),
						PinnedJumperConfiguration = new List<Module.ModuleLauncher.JumperConfiguration>(),
						RecentJumperConfiguration = new List<Module.ModuleLauncher.JumperConfiguration>(),
					},
					ModdingWorker = new ModdingWorkerSettingData() {
						Kernel = "",
						Script = "",
						Argument = new List<String>(),
						AutomaticScroll = true,
						ImmediateLaunch = true,
						AlternativeLaunchScript = "",
						MessageFontFamily = "",
					},
					ResourceForwarder = new ResourceForwarderSettingData() {
						OptionConfiguration = "",
						AutomaticClose = false,
						ParallelExecute = false,
						EnableFilter = true,
						EnableBatch = false,
						RemainInput = false,
					},
					CommandSender = new CommandSenderSettingData() {
						MethodConfiguration = "",
					},
					AnimationViewer = new AnimationViewerSettingData() {
						ImmediateSelect = true,
						AutomaticPlay = true,
						RepeatPlay = true,
						RemainFrameRate = true,
						ShowSpriteBoundary = false,
						SpriteFilterRule = "",
					},
				};
			}
			Setting.Save();
			return;
		}

		#endregion

	}

}
