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
		public String                                  Version                      = default!;
		public CustomThemeBackdrop                     ThemeBackdrop                = default!;
		public CustomThemeMode                         ThemeMode                    = default!;
		public Boolean                                 ThemeColorState              = default!;
		public Integer                                 ThemeColorLight              = default!;
		public Integer                                 ThemeColorDark               = default!;
		public Boolean                                 WindowPositionState          = default!;
		public Integer                                 WindowPositionX              = default!;
		public Integer                                 WindowPositionY              = default!;
		public Boolean                                 WindowSizeState              = default!;
		public Integer                                 WindowSizeWidth              = default!;
		public Integer                                 WindowSizeHeight             = default!;
		public Dictionary<String, String>              StoragePickerHistoryLocation = default!;
		public ModuleType                              ForwarderDefaultTarget       = default!;
		public Boolean                                 ForwarderImmediateJump       = default!;
		public String                                  ModuleConfigurationDirectory = default!;
		public ModuleLauncherSetting                   ModuleLauncher               = default!;
		public View.CoreTaskWorker.Setting             CoreTaskWorker               = default!;
		public View.CoreCommandSender.Setting          CoreCommandSender            = default!;
		public View.CoreResourceShipper.Setting        CoreResourceShipper          = default!;
		public View.PopcapAnimationViewer.Setting      PopcapAnimationViewer        = default!;
		public View.PopcapReflectionDescriptor.Setting PopcapReflectionDescriptor   = default!;
		public View.PopcapPackageBuilder.Setting       PopcapPackageBuilder         = default!;
	}

	public record SettingState {
		public CustomThemeBackdrop?                            ThemeBackdrop                   = default!;
		public CustomThemeMode?                                ThemeMode                       = default!;
		public List<List<View.CoreTaskWorker.ValueExpression>> CoreTaskWorkerSubmissionHistory = default!;
	}

	public class SettingProvider {

		#region constructor

		public SettingData Data { get; private set; }

		public SettingState State { get; }

		// ----------------

		public SettingProvider (
		) {
			this.Data = SettingProvider.CreateDefaultData();
			this.State = SettingProvider.CreateDefaultState();
			return;
		}

		#endregion

		#region action

		public async Task Reset (
		) {
			this.Data = SettingProvider.CreateDefaultData();
			return;
		}

		public async Task Apply (
		) {
			// ThemeBackdrop
			if (this.State.ThemeBackdrop != this.Data.ThemeBackdrop && App.MainWindowIsInitialized) {
				App.MainWindow.SystemBackdrop = this.Data.ThemeBackdrop switch {
					CustomThemeBackdrop.Solid          => null,
					CustomThemeBackdrop.MicaBase       => new MicaBackdrop() { Kind = MicaKind.Base },
					CustomThemeBackdrop.MicaAlt        => new MicaBackdrop() { Kind = MicaKind.BaseAlt },
					CustomThemeBackdrop.AcrylicDesktop => new DesktopAcrylicBackdrop() { },
					_                                  => throw new UnreachableException(),
				};
				App.MainWindow.uBackground.Visibility = this.Data.ThemeBackdrop == CustomThemeBackdrop.Solid ? Visibility.Visible : Visibility.Collapsed;
				this.State.ThemeBackdrop = this.Data.ThemeBackdrop;
			}
			// ThemeMode
			if (this.State.ThemeMode != this.Data.ThemeMode && App.MainWindowIsInitialized) {
				App.MainWindow.Content.As<FrameworkElement>().RequestedTheme = this.Data.ThemeMode switch {
					CustomThemeMode.System => ElementTheme.Default,
					CustomThemeMode.Light  => ElementTheme.Light,
					CustomThemeMode.Dark   => ElementTheme.Dark,
					_                      => throw new UnreachableException(),
				};
				App.MainWindow.AppWindow.TitleBar.ButtonForegroundColor = this.Data.ThemeMode switch {
					CustomThemeMode.System => null,
					CustomThemeMode.Light  => Colors.Black,
					CustomThemeMode.Dark   => Colors.White,
					_                      => throw new UnreachableException(),
				};
				await ControlHelper.IterateDialog(async (it) => {
					it.RequestedTheme = App.MainWindow.Content.As<FrameworkElement>().RequestedTheme;
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
				list.AddRange(App.Setting.Data.ModuleLauncher.Module.Select((it) => generateItem(ModuleLauncherCategory.Module, it)));
				list.AddRange(App.Setting.Data.ModuleLauncher.Pinned.Select((it) => generateItem(ModuleLauncherCategory.Pinned, it)));
				list.AddRange(App.Setting.Data.ModuleLauncher.Recent.Select((it) => generateItem(ModuleLauncherCategory.Recent, it)));
				await JumpListHelper.Apply(list);
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
				return $"{App.SharedDirectory}/setting.json";
			}
		}

		// ----------------

		public async Task Load (
			String? file = null
		) {
			file ??= this.File;
			this.Data = await JsonHelper.DeserializeFile<SettingData>(file);
			AssertTest(this.Data.Version == ApplicationInformation.Version);
			return;
		}

		public async Task Save (
			String? file  = null,
			Boolean apply = true
		) {
			file ??= this.File;
			if (apply) {
				await this.Apply();
			}
			await JsonHelper.SerializeFile<SettingData>(file, this.Data);
			return;
		}

		#endregion

		#region utility

		private static SettingData CreateDefaultData (
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
					Module = Enum.GetValues<ModuleType>().Select(ModuleHelper.Query).Select((value) => (new ModuleLauncherConfiguration() {
						Title = value.Name,
						Type = value.Type,
						Option = [],
						Command = [
							"-window_size",
							$"{value.StandardSize.Item1}",
							$"{value.StandardSize.Item2}",
						],
					})).ToList(),
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

		private static SettingState CreateDefaultState (
		) {
			return new () {
				ThemeBackdrop = null,
				ThemeMode = null,
				CoreTaskWorkerSubmissionHistory = Enum.GetValues<View.CoreTaskWorker.SubmissionType>().Select((value) => (new List<View.CoreTaskWorker.ValueExpression>())).ToList(),
			};
		}

		#endregion

	}

}
