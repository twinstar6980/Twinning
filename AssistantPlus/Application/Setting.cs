#pragma warning disable 0,
// ReSharper disable MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.UI;
using Microsoft.UI;
using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Xaml.Media;
using Colors = Microsoft.UI.Colors;

namespace AssistantPlus {

	public record SettingData {
		public Integer                           Version              = default!;
		public CustomThemeSetting                Theme                = default!;
		public CustomWindowSetting               Window               = default!;
		public ModuleLauncherSetting             ModuleLauncher       = default!;
		public View.ModdingWorker.Setting        ModdingWorker        = default!;
		public View.CommandSender.Setting        CommandSender        = default!;
		public View.ResourceShipper.Setting      ResourceShipper      = default!;
		public View.AnimationViewer.Setting      AnimationViewer      = default!;
		public View.ReflectionDescriptor.Setting ReflectionDescriptor = default!;
		public View.PackageBuilder.Setting       PackageBuilder       = default!;
	}

	public record SettingState {
		public CustomThemeMode?     ThemeMode     = default!;
		public CustomThemeColor?    ThemeColor    = default!;
		public CustomThemeBackdrop? ThemeBackdrop = default!;
	}

	public class SettingProvider {

		#region structor

		public SettingData Data;

		public SettingState State;

		// ----------------

		public SettingProvider (
		) {
			this.Data = SettingProvider.CreateDefaultData();
			this.State = SettingProvider.CreateDefaultState();
		}

		#endregion

		#region action

		public async Task Reset (
		) {
			this.Data = SettingProvider.CreateDefaultData();
			this.State = SettingProvider.CreateDefaultState();
			return;
		}

		public async Task Apply (
		) {
			// Theme.Mode
			if (this.State.ThemeMode != this.Data.Theme.Mode && App.MainWindowIsInitialized) {
				App.MainWindow.Content.As<FrameworkElement>().RequestedTheme = this.Data.Theme.Mode switch {
					CustomThemeMode.System => ElementTheme.Default,
					CustomThemeMode.Light  => ElementTheme.Light,
					CustomThemeMode.Dark   => ElementTheme.Dark,
					_                      => throw new (),
				};
				App.MainWindow.AppWindow.TitleBar.ButtonForegroundColor = this.Data.Theme.Mode switch {
					CustomThemeMode.System => null,
					CustomThemeMode.Light  => Colors.Black,
					CustomThemeMode.Dark   => Colors.White,
					_                      => throw new (),
				};
				this.State.ThemeMode = this.Data.Theme.Mode;
			}
			// Theme.Color
			if (this.State.ThemeColor != this.Data.Theme.Color) {
				var customColorOnLight = Color.FromArgb(
					0xFF,
					(Byte)this.Data.Theme.Color.LightRed,
					(Byte)this.Data.Theme.Color.LightGreen,
					(Byte)this.Data.Theme.Color.LightBlue
				);
				var customColorOnDark = Color.FromArgb(
					0xFF,
					(Byte)this.Data.Theme.Color.DarkRed,
					(Byte)this.Data.Theme.Color.DarkGreen,
					(Byte)this.Data.Theme.Color.DarkBlue
				);
				foreach (var resourceKey in new[] { "SystemAccentColorDark1", "SystemAccentColorDark2", "SystemAccentColorDark3" }) {
					if (!this.Data.Theme.Color.State) {
						App.Instance.Resources.Remove(resourceKey);
					}
					else {
						App.Instance.Resources[resourceKey] = customColorOnLight;
					}
				}
				foreach (var resourceKey in new[] { "SystemAccentColorLight1", "SystemAccentColorLight2", "SystemAccentColorLight3" }) {
					if (!this.Data.Theme.Color.State) {
						App.Instance.Resources.Remove(resourceKey);
					}
					else {
						App.Instance.Resources[resourceKey] = customColorOnDark;
					}
				}
				this.State.ThemeColor = this.Data.Theme.Color;
			}
			// Theme.Backdrop
			if (this.State.ThemeBackdrop != this.Data.Theme.Backdrop && App.MainWindowIsInitialized) {
				App.MainWindow.SystemBackdrop = this.Data.Theme.Backdrop switch {
					CustomThemeBackdrop.Solid          => null,
					CustomThemeBackdrop.MicaBase       => new MicaBackdrop() { Kind = MicaKind.Base },
					CustomThemeBackdrop.MicaAlt        => new MicaBackdrop() { Kind = MicaKind.BaseAlt },
					CustomThemeBackdrop.AcrylicDesktop => new DesktopAcrylicBackdrop() { },
					_                                  => throw new (),
				};
				App.MainWindow.uBackground.Visibility = this.Data.Theme.Backdrop == CustomThemeBackdrop.Solid ? Visibility.Visible : Visibility.Collapsed;
				this.State.ThemeBackdrop = this.Data.Theme.Backdrop;
			}
			// ModuleLauncher
			await App.Instance.RegisterShellJumpList();
			// ModdingWorker.MessageFont
			App.Instance.Resources["ModdingWorker.MessageFont"] = this.Data.ModdingWorker.MessageFont.Length == 0 ? FontFamily.XamlAutoFontFamily : new (this.Data.ModdingWorker.MessageFont);
			return;
		}

		#endregion

		#region storage

		public String File {
			get {
				return StorageHelper.ToWindowsStyle($"{App.SharedDirectory}/Setting.json");
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
				Theme = new () {
					Mode = CustomThemeMode.System,
					Color = new () {
						State = false,
						LightRed = 0x00,
						LightGreen = 0x78,
						LightBlue = 0xD4,
						DarkRed = 0x4C,
						DarkGreen = 0xC2,
						DarkBlue = 0xFF,
					},
					Backdrop = CustomThemeBackdrop.MicaBase,
				},
				Window = new () {
					Position = new () {
						State = false,
						X = 0,
						Y = 0,
					},
					Size = new () {
						State = false,
						Width = 1680,
						Height = 960,
					},
				},
				ModuleLauncher = new () {
					Module = [
						..ModuleHelper.Information.Select((value) => (new ModuleLauncherConfiguration() {
							Title = value.Name,
							Type = value.Type,
							Option = [],
							Command = [],
						})),
					],
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
				ThemeMode = null,
				ThemeColor = null,
				ThemeBackdrop = null,
			};
		}

		#endregion

	}

}
