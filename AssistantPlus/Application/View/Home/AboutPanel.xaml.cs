#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.Globalization.NumberFormatting;
using Windows.UI;

namespace Twinning.AssistantPlus.View.Home {

	public sealed partial class AboutPanel : CustomControl {

		#region life

		private AboutPanelController Controller { get; }

		// ----------------

		public AboutPanel(
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate(
		) {
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		#endregion

	}

	public partial class AboutPanelController : CustomController {

		#region data

		public AboutPanel View { get; init; } = default!;

		// ----------------

		#endregion

		#region life

		public async Task UpdateView(
		) {
			this.NotifyPropertyChanged([
			]);
			return;
		}

		#endregion

		#region description

		public String uVersionLabel_Text {
			get {
				return $"{ApplicationInformation.Name} - v{ApplicationInformation.Version}";
			}
		}

		public String uCopyrightLabel_Text {
			get {
				return $"\u00A9 {ApplicationInformation.Year} {ApplicationInformation.Developer}. All rights reserved.";
			}
		}

		#endregion

		#region setting

		public List<String> uSettingThemeBackdrop_ItemsSource {
			get {
				return Enum.GetNames<CustomThemeBackdrop>().Select(ConvertHelper.ChangeStringFromCamelCaseThenInsertSpace).ToList();
			}
		}

		public Size uSettingThemeBackdrop_SelectedIndex {
			get {
				return App.Instance.Setting.Data.ThemeBackdrop.CastPrimitive<Size>();
			}
		}

		public async void uSettingThemeBackdrop_SelectionChanged(
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Instance.Setting.Data.ThemeBackdrop != senders.SelectedIndex.CastPrimitive<CustomThemeBackdrop>()) {
				App.Instance.Setting.Data.ThemeBackdrop = senders.SelectedIndex.CastPrimitive<CustomThemeBackdrop>();
				await App.Instance.Setting.Save();
			}
			return;
		}

		// ----------------

		public List<String> uSettingThemeMode_ItemsSource {
			get {
				return Enum.GetNames<CustomThemeMode>().Select(ConvertHelper.ChangeStringFromCamelCaseThenInsertSpace).ToList();
			}
		}

		public Size uSettingThemeMode_SelectedIndex {
			get {
				return App.Instance.Setting.Data.ThemeMode.CastPrimitive<Size>();
			}
		}

		public async void uSettingThemeMode_SelectionChanged(
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Instance.Setting.Data.ThemeMode != senders.SelectedIndex.CastPrimitive<CustomThemeMode>()) {
				App.Instance.Setting.Data.ThemeMode = senders.SelectedIndex.CastPrimitive<CustomThemeMode>();
				await App.Instance.Setting.Save();
			}
			return;
		}

		// ----------------

		public Boolean uSettingThemeColor_IsChecked {
			get {
				return App.Instance.Setting.Data.ThemeColorState;
			}
		}

		public async void uSettingThemeColor_Click(
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Instance.Setting.Data.ThemeColorState = senders.IsChecked;
			this.NotifyPropertyChanged([
				nameof(this.uSettingThemeColor_IsChecked),
				nameof(this.uSettingThemeColor_Content),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public String uSettingThemeColor_Content {
			get {
				return !App.Instance.Setting.Data.ThemeColorState ? "Default" : "Custom";
			}
		}

		public async void uSettingThemeColorLight_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ColorPicker>();
			App.Instance.Setting.Data.ThemeColorLight = ConvertHelper.MakeColorToInteger(senders.Color);
			this.NotifyPropertyChanged([
				nameof(this.uSettingThemeColorLight_Color),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public Color uSettingThemeColorLight_Color {
			get {
				return ConvertHelper.ParseColorFromInteger(App.Instance.Setting.Data.ThemeColorLight);
			}
		}

		public async void uSettingThemeColorDark_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ColorPicker>();
			App.Instance.Setting.Data.ThemeColorDark = ConvertHelper.MakeColorToInteger(senders.Color);
			this.NotifyPropertyChanged([
				nameof(this.uSettingThemeColorDark_Color),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public Color uSettingThemeColorDark_Color {
			get {
				return ConvertHelper.ParseColorFromInteger(App.Instance.Setting.Data.ThemeColorDark);
			}
		}

		// ----------------

		public Boolean uSettingWindowPosition_IsChecked {
			get {
				return App.Instance.Setting.Data.WindowPositionState;
			}
		}

		public async void uSettingWindowPosition_Click(
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Instance.Setting.Data.WindowPositionState = senders.IsChecked;
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowPosition_IsChecked),
				nameof(this.uSettingWindowPosition_Content),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public String uSettingWindowPosition_Content {
			get {
				return !App.Instance.Setting.Data.WindowPositionState ? "Default" : "Custom";
			}
		}

		public async void uSettingWindowPositionX_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Instance.Setting.Data.WindowPositionX = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowPositionX_Value),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowPositionX_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowPositionX_Value {
			get {
				return App.Instance.Setting.Data.WindowPositionX;
			}
		}

		public async void uSettingWindowPositionY_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Instance.Setting.Data.WindowPositionY = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowPositionY_Value),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowPositionY_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowPositionY_Value {
			get {
				return App.Instance.Setting.Data.WindowPositionY;
			}
		}

		// ----------------

		public Boolean uSettingWindowSize_IsChecked {
			get {
				return App.Instance.Setting.Data.WindowSizeState;
			}
		}

		public async void uSettingWindowSize_Click(
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Instance.Setting.Data.WindowSizeState = senders.IsChecked;
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowSize_IsChecked),
				nameof(this.uSettingWindowSize_Content),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public String uSettingWindowSize_Content {
			get {
				return !App.Instance.Setting.Data.WindowSizeState ? "Default" : "Custom";
			}
		}

		public async void uSettingWindowSizeWidth_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Instance.Setting.Data.WindowSizeWidth = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowSizeWidth_Value),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowSizeWidth_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowSizeWidth_Value {
			get {
				return App.Instance.Setting.Data.WindowSizeWidth;
			}
		}

		public async void uSettingWindowSizeHeight_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Instance.Setting.Data.WindowSizeHeight = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowSizeHeight_Value),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowSizeHeight_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowSizeHeight_Value {
			get {
				return App.Instance.Setting.Data.WindowSizeHeight;
			}
		}

		// ----------------

		public Boolean uSettingForwarderExtension_IsChecked {
			get {
				return ApplicationExtensionManager.Instance.CheckForwarder().Result;
			}
		}

		public async void uSettingForwarderExtension_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			await ApplicationExtensionManager.Instance.ToggleForwarder(!await ApplicationExtensionManager.Instance.CheckForwarder());
			this.NotifyPropertyChanged([
				nameof(this.uSettingForwarderExtension_IsChecked),
				nameof(this.uSettingForwarderExtension_Content),
			]);
			return;
		}

		public String uSettingForwarderExtension_Content {
			get {
				return !ApplicationExtensionManager.Instance.CheckForwarder().Result ? "Disabled" : "Enabled";
			}
		}

		// ----------------

		public List<String> uSettingForwarderDefaultTarget_ItemsSource {
			get {
				return Enum.GetValues<ModuleType>().Select((value) => ModuleHelper.Query(value).Name).ToList();
			}
		}

		public Size uSettingForwarderDefaultTarget_SelectedIndex {
			get {
				return App.Instance.Setting.Data.ForwarderDefaultTarget.CastPrimitive<Size>();
			}
		}

		public async void uSettingForwarderDefaultTarget_SelectionChanged(
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Instance.Setting.Data.ForwarderDefaultTarget != senders.SelectedIndex.CastPrimitive<ModuleType>()) {
				App.Instance.Setting.Data.ForwarderDefaultTarget = senders.SelectedIndex.CastPrimitive<ModuleType>();
				await App.Instance.Setting.Save();
			}
			return;
		}

		// ----------------

		public Boolean uSettingForwarderImmediateJump_IsChecked {
			get {
				return App.Instance.Setting.Data.ForwarderImmediateJump;
			}
		}

		public async void uSettingForwarderImmediateJump_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			App.Instance.Setting.Data.ForwarderImmediateJump = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uSettingForwarderImmediateJump_IsChecked),
				nameof(this.uSettingForwarderImmediateJump_Content),
			]);
			await App.Instance.Setting.Save();
			return;
		}

		public String uSettingForwarderImmediateJump_Content {
			get {
				return !App.Instance.Setting.Data.ForwarderImmediateJump ? "Disabled" : "Enabled";
			}
		}

		// ----------------

		public async void uSettingModuleConfigurationDirectory_Click(
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			var target = await StorageHelper.PickLoadDirectory(App.Instance.MainWindow, "@application.module_configuration_directory");
			if (target != null) {
				App.Instance.Setting.Data.ModuleConfigurationDirectory = target;
				this.NotifyPropertyChanged([
					nameof(this.uSettingModuleConfigurationDirectoryText_Text),
				]);
				await App.Instance.Setting.Save();
			}
			return;
		}

		public String uSettingModuleConfigurationDirectoryText_Text {
			get {
				return App.Instance.Setting.Data.ModuleConfigurationDirectory.Emit();
			}
		}

		#endregion

		#region storage

		public async void uStoragePackageDirectory_Click(
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.Reveal(App.Instance.PackageDirectory);
			await App.Instance.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		// ----------------

		public async void uStorageSettingFile_Click(
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.Reveal(App.Instance.Setting.File);
			await App.Instance.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		public async void uStorageSettingFileMenu_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			var changed = false;
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await App.Instance.Setting.Load();
					await App.Instance.Setting.Save();
					changed = true;
					break;
				}
				case "Reset": {
					if (await ControlHelper.ShowDialogForConfirm(this.View, null, null)) {
						await App.Instance.Setting.Reset();
						await App.Instance.Setting.Save();
						changed = true;
					}
					break;
				}
				case "Import": {
					var target = await StorageHelper.PickLoadFile(App.Instance.MainWindow, $"@application.setting_file");
					if (target != null) {
						await App.Instance.Setting.Load(target);
						await App.Instance.Setting.Save();
						changed = true;
					}
					break;
				}
				case "Export": {
					var target = await StorageHelper.PickSaveFile(App.Instance.MainWindow, $"@application.setting_file", "setting.json");
					if (target != null) {
						await App.Instance.Setting.Save(target, false);
						changed = true;
					}
					break;
				}
				default: throw new UnreachableException();
			}
			if (changed) {
				this.NotifyPropertyChanged([
					nameof(this.uSettingThemeMode_SelectedIndex),
					nameof(this.uSettingThemeColor_IsChecked),
					nameof(this.uSettingThemeColor_Content),
					nameof(this.uSettingThemeColorLight_Color),
					nameof(this.uSettingThemeColorDark_Color),
					nameof(this.uSettingThemeBackdrop_SelectedIndex),
					nameof(this.uSettingWindowPosition_IsChecked),
					nameof(this.uSettingWindowPosition_Content),
					nameof(this.uSettingWindowPositionX_Value),
					nameof(this.uSettingWindowPositionY_Value),
					nameof(this.uSettingWindowSize_IsChecked),
					nameof(this.uSettingWindowSize_Content),
					nameof(this.uSettingWindowSizeWidth_Value),
					nameof(this.uSettingWindowSizeHeight_Value),
				]);
				await App.Instance.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			}
			return;
		}

		// ----------------

		public async void uStorageSharedDirectory_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await StorageHelper.Reveal(App.Instance.SharedDirectory);
			await App.Instance.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		// ----------------

		public async void uStorageCacheDirectory_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (await StorageHelper.Exist(App.Instance.CacheDirectory)) {
				await StorageHelper.Remove(App.Instance.CacheDirectory);
			}
			await App.Instance.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		// ----------------

		public async void uOnboarding_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await App.Instance.MainWindow.ShowOnboarding();
			return;
		}

		#endregion

	}

}
