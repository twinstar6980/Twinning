#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.UI.StartScreen;
using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Xaml.Media;
using Colors = Microsoft.UI.Colors;

namespace Twinning.AssistantPlus {

	public record SettingData {
		public Integer                           Version                      = default!;
		public CustomThemeBackdrop               ThemeBackdrop                = default!;
		public CustomThemeMode                   ThemeMode                    = default!;
		public Boolean                           ThemeColorState              = default!;
		public Integer                           ThemeColorLight              = default!;
		public Integer                           ThemeColorDark               = default!;
		public Boolean                           WindowPositionState          = default!;
		public Integer                           WindowPositionX              = default!;
		public Integer                           WindowPositionY              = default!;
		public Boolean                           WindowSizeState              = default!;
		public Integer                           WindowSizeWidth              = default!;
		public Integer                           WindowSizeHeight             = default!;
		public Dictionary<String, String>        StoragePickerHistoryLocation = default!;
		public ModuleType                        ForwarderDefaultTarget       = default!;
		public Boolean                           ForwarderImmediateJump       = default!;
		public ModuleLauncherSetting             ModuleLauncher               = default!;
		public View.ModdingWorker.Setting        ModdingWorker                = default!;
		public View.CommandSender.Setting        CommandSender                = default!;
		public View.ResourceShipper.Setting      ResourceShipper              = default!;
		public View.AnimationViewer.Setting      AnimationViewer              = default!;
		public View.ReflectionDescriptor.Setting ReflectionDescriptor         = default!;
		public View.PackageBuilder.Setting       PackageBuilder               = default!;
	}

	public record SettingState {
		public CustomThemeBackdrop?                           ThemeBackdrop                  = default!;
		public CustomThemeMode?                               ThemeMode                      = default!;
		public List<List<View.ModdingWorker.ValueExpression>> ModdingWorkerSubmissionHistory = default!;
	}

	public class SettingProvider {

		#region constructor

		public SettingData Data;

		public SettingState State;

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
			// ModdingWorker.MessageFont
			{
				App.Instance.Resources["ModdingWorker.MessageFont"] = this.Data.ModdingWorker.MessageFont.Length == 0 ? FontFamily.XamlAutoFontFamily : new (this.Data.ModdingWorker.MessageFont);
			}
			return;
		}

		#endregion

		#region storage

		public String File {
			get {
				return $"{App.SharedDirectory}/Setting.json";
			}
		}

		// ----------------

		public async Task Load (
			String? file = null
		) {
			file ??= this.File;
			this.Data = await JsonHelper.DeserializeFile<SettingData>(file);
			GF.AssertTest(this.Data.Version == Package.Current.Id.Version.Major);
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
				Version = Package.Current.Id.Version.Major,
				ThemeBackdrop = CustomThemeBackdrop.MicaBase,
				ThemeMode = CustomThemeMode.System,
				ThemeColorState = false,
				ThemeColorLight = 0xFF0078D4L,
				ThemeColorDark = 0xFF4CC2FFL,
				WindowPositionState = false,
				WindowPositionX = 0,
				WindowPositionY = 0,
				WindowSizeState = false,
				WindowSizeWidth = 0,
				WindowSizeHeight = 0,
				StoragePickerHistoryLocation = [],
				ForwarderDefaultTarget = ModuleType.ResourceShipper,
				ForwarderImmediateJump = false,
				ModuleLauncher = new () {
					Module = Enum.GetValues<ModuleType>().Select((value) => (new ModuleLauncherConfiguration() {
						Title = ModuleHelper.Query(value).Name,
						Type = value,
						Option = [],
						Command = [],
					})).ToList(),
					Pinned = [],
					Recent = [],
				},
				ModdingWorker = new () {
					Kernel = "",
					Script = "",
					Argument = [],
					AutomaticScroll = true,
					ImmediateLaunch = true,
					MessageFont = "",
				},
				CommandSender = new () {
					MethodConfiguration = "",
					ParallelForward = false,
				},
				ResourceShipper = new () {
					OptionConfiguration = "",
					ParallelForward = false,
					EnableFilter = true,
					EnableBatch = false,
				},
				AnimationViewer = new () {
					ImmediateSelect = true,
					AutomaticPlay = true,
					RepeatPlay = true,
					KeepSpeed = true,
					ShowBoundary = true,
					ImageFilterRule = "",
					SpriteFilterRule = "",
				},
				ReflectionDescriptor = new () {
				},
				PackageBuilder = new () {
				},
			};
		}

		private static SettingState CreateDefaultState (
		) {
			return new () {
				ThemeBackdrop = null,
				ThemeMode = null,
				ModdingWorkerSubmissionHistory = Enum.GetValues<View.ModdingWorker.SubmissionType>().Select((value) => (new List<View.ModdingWorker.ValueExpression>())).ToList(),
			};
		}

		#endregion

	}

}
