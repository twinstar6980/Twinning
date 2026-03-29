#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.UI.StartScreen;
using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Xaml.Media;
using Colors = Microsoft.UI.Colors;

namespace Twinning.AssistantPlus {

	public record SettingData {
		public String                                  Version                      { get; set; } = default!;
		public CustomThemeBackdrop                     ThemeBackdrop                { get; set; } = default!;
		public CustomThemeMode                         ThemeMode                    { get; set; } = default!;
		public Boolean                                 ThemeColorState              { get; set; } = default!;
		public Integer                                 ThemeColorLight              { get; set; } = default!;
		public Integer                                 ThemeColorDark               { get; set; } = default!;
		public Boolean                                 WindowPositionState          { get; set; } = default!;
		public Integer                                 WindowPositionX              { get; set; } = default!;
		public Integer                                 WindowPositionY              { get; set; } = default!;
		public Boolean                                 WindowSizeState              { get; set; } = default!;
		public Integer                                 WindowSizeWidth              { get; set; } = default!;
		public Integer                                 WindowSizeHeight             { get; set; } = default!;
		public Dictionary<String, String>              StoragePickerHistoryLocation { get; set; } = default!;
		public ModuleType                              ForwarderDefaultTarget       { get; set; } = default!;
		public Boolean                                 ForwarderImmediateJump       { get; set; } = default!;
		public String                                  ModuleConfigurationDirectory { get; set; } = default!;
		public ModuleLauncherSetting                   ModuleLauncher               { get; set; } = default!;
		public View.CoreTaskWorker.Setting             CoreTaskWorker               { get; set; } = default!;
		public View.CoreCommandSender.Setting          CoreCommandSender            { get; set; } = default!;
		public View.CoreResourceShipper.Setting        CoreResourceShipper          { get; set; } = default!;
		public View.PopcapAnimationViewer.Setting      PopcapAnimationViewer        { get; set; } = default!;
		public View.PopcapReflectionDescriptor.Setting PopcapReflectionDescriptor   { get; set; } = default!;
		public View.PopcapPackageBuilder.Setting       PopcapPackageBuilder         { get; set; } = default!;
	}

	public record SettingState {
		public CustomThemeBackdrop?                            ThemeBackdrop                   { get; set; } = default!;
		public CustomThemeMode?                                ThemeMode                       { get; set; } = default!;
		public List<List<View.CoreTaskWorker.ValueExpression>> CoreTaskWorkerSubmissionHistory { get; set; } = default!;
	}

	public class SettingProvider {

		#region constructor

		public SettingData Data { get; private set; }

		public SettingState State { get; }

		// ----------------

		public SettingProvider(
		) {
			this.Data = SettingProvider.CreateDefaultData();
			this.State = SettingProvider.CreateDefaultState();
			return;
		}

		#endregion

		#region action

		public async Task Reset(
		) {
			this.Data = SettingProvider.CreateDefaultData();
			return;
		}

		public async Task Apply(
		) {
			// ThemeBackdrop
			if (this.State.ThemeBackdrop != this.Data.ThemeBackdrop && App.Instance.MainWindowIsInitialized) {
				App.Instance.MainWindow.SystemBackdrop = this.Data.ThemeBackdrop switch {
					CustomThemeBackdrop.Solid          => null,
					CustomThemeBackdrop.MicaBase       => new MicaBackdrop() { Kind = MicaKind.Base },
					CustomThemeBackdrop.MicaAlt        => new MicaBackdrop() { Kind = MicaKind.BaseAlt },
					CustomThemeBackdrop.AcrylicDesktop => new DesktopAcrylicBackdrop() {},
					_                                  => throw new UnreachableException(),
				};
				App.Instance.MainWindow.uBackground.Visibility = this.Data.ThemeBackdrop == CustomThemeBackdrop.Solid ? Visibility.Visible : Visibility.Collapsed;
				this.State.ThemeBackdrop = this.Data.ThemeBackdrop;
			}
			// ThemeMode
			if (this.State.ThemeMode != this.Data.ThemeMode && App.Instance.MainWindowIsInitialized) {
				App.Instance.MainWindow.Content.As<FrameworkElement>().RequestedTheme = this.Data.ThemeMode switch {
					CustomThemeMode.System => ElementTheme.Default,
					CustomThemeMode.Light  => ElementTheme.Light,
					CustomThemeMode.Dark   => ElementTheme.Dark,
					_                      => throw new UnreachableException(),
				};
				App.Instance.MainWindow.AppWindow.TitleBar.ButtonForegroundColor = this.Data.ThemeMode switch {
					CustomThemeMode.System => null,
					CustomThemeMode.Light  => Colors.Black,
					CustomThemeMode.Dark   => Colors.White,
					_                      => throw new UnreachableException(),
				};
				await ControlHelper.IterateDialog(async (it) => {
					it.RequestedTheme = App.Instance.MainWindow.Content.As<FrameworkElement>().RequestedTheme;
				});
				this.State.ThemeMode = this.Data.ThemeMode;
			}
			// ThemeColor
			{
				var customColorOnLight = ConvertHelper.ParseColorFromInteger(this.Data.ThemeColorLight);
				var customColorOnDark = ConvertHelper.ParseColorFromInteger(this.Data.ThemeColorDark);
				foreach (var resourceKey in new[] { "SystemAccentColorDark1", "SystemAccentColorDark2", "SystemAccentColorDark3" }) {
					if (!this.Data.ThemeColorState) {
						App.Instance.Resources.Remove(resourceKey);
					}
					else {
						App.Instance.Resources[resourceKey] = customColorOnLight;
					}
				}
				foreach (var resourceKey in new[] { "SystemAccentColorLight1", "SystemAccentColorLight2", "SystemAccentColorLight3" }) {
					if (!this.Data.ThemeColorState) {
						App.Instance.Resources.Remove(resourceKey);
					}
					else {
						App.Instance.Resources[resourceKey] = customColorOnDark;
					}
				}
			}
			// ModuleLauncher
			{
				var list = new List<JumpListItem>();
				var generateItem = (ModuleLauncherCategory category, ModuleLauncherConfiguration configuration) => {
					return JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandString(null, ModuleHelper.GenerateArgument(configuration)), "").SelfAlso((it) => {
						it.Logo = new ("ms-appx:///Asset/Logo.png");
						it.GroupName = category switch {
							ModuleLauncherCategory.Module => "",
							ModuleLauncherCategory.Pinned => "Pinned",
							ModuleLauncherCategory.Recent => "Recent",
							_                             => throw new UnreachableException(),
						};
						it.DisplayName = configuration.Title;
						it.Description = "";
					});
				};
				list.AddRange(App.Instance.Setting.Data.ModuleLauncher.Module.Select((it) => generateItem(ModuleLauncherCategory.Module, it)));
				list.AddRange(App.Instance.Setting.Data.ModuleLauncher.Pinned.Select((it) => generateItem(ModuleLauncherCategory.Pinned, it)));
				list.AddRange(App.Instance.Setting.Data.ModuleLauncher.Recent.Select((it) => generateItem(ModuleLauncherCategory.Recent, it)));
				await ApplicationJumpListManager.Instance.Apply(list);
			}
			// CoreTaskWorker.MessageFont
			{
				App.Instance.Resources["CoreTaskWorker.MessageFont"] = this.Data.CoreTaskWorker.MessageFont.Length == 0 ? FontFamily.XamlAutoFontFamily : new (this.Data.CoreTaskWorker.MessageFont);
			}
			return;
		}

		#endregion

		#region storage

		public String File {
			get {
				return $"{App.Instance.SharedDirectory}/setting.json";
			}
		}

		// ----------------

		public async Task Load(
			String? file = null
		) {
			file ??= this.File;
			this.Data = (await JsonHelper.DeserializeFile<SettingData>(file)).SelfAlso((it) => AssertTest(it.Version == ApplicationInformation.Version));
			return;
		}

		public async Task Save(
			String? file  = null,
			Boolean apply = true
		) {
			file ??= this.File;
			if (apply) {
				await this.Apply();
			}
			if (!await StorageHelper.ExistFile(file)) {
				await StorageHelper.CreateFile(file);
			}
			await JsonHelper.SerializeFile(file, this.Data);
			return;
		}

		#endregion

		#region utility

		private static SettingData CreateDefaultData(
		) {
			return new () {
				Version = ApplicationInformation.Version,
				ThemeBackdrop = CustomThemeBackdrop.MicaBase,
				ThemeMode = CustomThemeMode.System,
				ThemeColorState = false,
				ThemeColorLight = 0xFF0078D4L,
				ThemeColorDark = 0xFF4CC2FFL,
				WindowPositionState = false,
				WindowPositionX = 0,
				WindowPositionY = 0,
				WindowSizeState = true,
				WindowSizeWidth = 480,
				WindowSizeHeight = 840,
				StoragePickerHistoryLocation = [],
				ForwarderDefaultTarget = ModuleType.CoreResourceShipper,
				ForwarderImmediateJump = false,
				ModuleConfigurationDirectory = "",
				ModuleLauncher = new () {
					Module = Enum.GetValues<ModuleType>().Select(ModuleHelper.Query).Select((value) => new ModuleLauncherConfiguration() {
						Title = value.Name,
						Type = value.Type,
						Option = [],
						Command = [
							"-window_size",
							$"{value.StandardSize.Item1}",
							$"{value.StandardSize.Item2}",
						],
					}).ToList(),
					Pinned = [],
					Recent = [],
				},
				CoreTaskWorker = new () {
					Kernel = "",
					Script = "",
					Argument = [],
					AutomaticScroll = true,
					ImmediateLaunch = true,
					MessageFont = "",
				},
				CoreCommandSender = new () {
					ParallelForward = false,
				},
				CoreResourceShipper = new () {
					ParallelForward = false,
					EnableFilter = true,
					EnableBatch = false,
				},
				PopcapAnimationViewer = new () {
					ImmediateSelect = true,
					AutomaticPlay = true,
					RepeatPlay = true,
					KeepSpeed = false,
					ShowBoundary = false,
					ImageFilterRule = "",
					SpriteFilterRule = "",
				},
				PopcapReflectionDescriptor = new () {
				},
				PopcapPackageBuilder = new () {
				},
			};
		}

		private static SettingState CreateDefaultState(
		) {
			return new () {
				ThemeBackdrop = null,
				ThemeMode = null,
				CoreTaskWorkerSubmissionHistory = Enum.GetValues<View.CoreTaskWorker.SubmissionType>().Select((value) => new List<View.CoreTaskWorker.ValueExpression>()).ToList(),
			};
		}

		// ----------------

		public async Task QuickSetup(
			String homeDirectory
		) {
			this.Data.ModuleConfigurationDirectory = $"{homeDirectory}/assistant_plus";
			this.Data.CoreTaskWorker.Kernel = $"{homeDirectory}/kernel";
			this.Data.CoreTaskWorker.Script = $"{homeDirectory}/script/main.js";
			this.Data.CoreTaskWorker.Argument = [$"{homeDirectory}"];
			return;
		}

		#endregion

	}

}
