#pragma warning disable 0,
// ReSharper disable MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.UI;
using Microsoft.UI;
using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Xaml.Media;
using Newtonsoft.Json;

namespace AssistantPlus {

	public class Setting {

		#region data

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record SettingData {
			public Integer                           Version              = default!;
			public CustomThemeSetting                Theme                = default!;
			public CustomWindowSetting               Window               = default!;
			public ModuleLauncherSetting             ModuleLauncher       = default!;
			public View.ModdingWorker.Setting        ModdingWorker        = default!;
			public View.CommandSender.Setting        CommandSender        = default!;
			public View.ResourceForwarder.Setting    ResourceForwarder    = default!;
			public View.ReflectionDescriptor.Setting ReflectionDescriptor = default!;
			public View.AnimationViewer.Setting      AnimationViewer      = default!;
			public View.PackageBuilder.Setting       PackageBuilder       = default!;
		}

		public static SettingData CreateDefaultData (
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
					AlternativeLaunchScript = "",
				},
				CommandSender = new () {
					MethodConfiguration = "",
				},
				ResourceForwarder = new () {
					OptionConfiguration = "",
					ParallelExecute = false,
					EnableFilter = true,
					EnableBatch = false,
					RemainInput = false,
					AutomaticClose = false,
				},
				ReflectionDescriptor = new () {
				},
				AnimationViewer = new () {
					ImmediateSelect = true,
					AutomaticPlay = true,
					RepeatPlay = true,
					RemainFrameRate = true,
					ShowSpriteBoundary = true,
					ImageFilterRule = "",
					SpriteFilterRule = "",
				},
				PackageBuilder = new () {
				},
			};
		}

		#endregion

		#region access

		public SettingData Data { get; set; } = null!;

		// ----------------

		private CustomThemeMode? LegacyThemeMode { get; set; } = null;

		private CustomThemeColor? LegacyThemeColor { get; set; } = null;

		private CustomThemeBackdrop? LegacyThemeBackdrop { get; set; } = null;

		public async Task Apply (
		) {
			// Theme.Mode
			if (this.LegacyThemeMode != this.Data.Theme.Mode && App.MainWindowIsInitialized) {
				App.MainWindow.Content.AsClass<FrameworkElement>().RequestedTheme = this.Data.Theme.Mode switch {
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
				this.LegacyThemeMode = this.Data.Theme.Mode;
			}
			// Theme.Color
			if (this.LegacyThemeColor != this.Data.Theme.Color) {
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
				this.LegacyThemeColor = this.Data.Theme.Color;
			}
			// Theme.Backdrop
			if (this.LegacyThemeBackdrop != this.Data.Theme.Backdrop && App.MainWindowIsInitialized) {
				App.MainWindow.SystemBackdrop = this.Data.Theme.Backdrop switch {
					CustomThemeBackdrop.Solid          => null,
					CustomThemeBackdrop.MicaBase       => new MicaBackdrop() { Kind = MicaKind.Base },
					CustomThemeBackdrop.MicaAlt        => new MicaBackdrop() { Kind = MicaKind.BaseAlt },
					CustomThemeBackdrop.AcrylicDesktop => new DesktopAcrylicBackdrop() { },
					_                                  => throw new (),
				};
				App.MainWindow.uBackground.Visibility = this.Data.Theme.Backdrop == CustomThemeBackdrop.Solid ? Visibility.Visible : Visibility.Collapsed;
				this.LegacyThemeBackdrop = this.Data.Theme.Backdrop;
			}
			// ModuleLauncher
			await App.Instance.RegisterShellJumpList();
			// ModdingWorker.MessageFont
			App.Instance.Resources["ModdingWorker.MessageFont"] = this.Data.ModdingWorker.MessageFont.Length == 0 ? FontFamily.XamlAutoFontFamily : new (this.Data.ModdingWorker.MessageFont);
			return;
		}

		#endregion

		#region storage

		public String File { get; } = Path.Combine(Windows.Storage.ApplicationData.Current.LocalFolder.Path, "Setting.json");

		// ----------------

		public async Task Load (
			String? file = null
		) {
			this.Data = await JsonHelper.DeserializeFile<SettingData>(file ?? this.File);
			GF.AssertTest(this.Data.Version == Package.Current.Id.Version.Major);
			return;
		}

		public async Task Save (
			String? file  = null,
			Boolean apply = true
		) {
			if (apply) {
				await this.Apply();
			}
			await JsonHelper.SerializeFile<SettingData>(file ?? this.File, this.Data);
			return;
		}

		#endregion

	}

}
