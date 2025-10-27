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

		public AboutPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
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

		public async Task UpdateView (
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
				return Enum.GetNames<CustomThemeBackdrop>().Select(ConvertHelper.InsertSpaceBetweenStringWord).ToList();
			}
		}

		public Size uSettingThemeBackdrop_SelectedIndex {
			get {
				return App.Setting.Data.ThemeBackdrop.CastPrimitive<Size>();
			}
		}

		public async void uSettingThemeBackdrop_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Setting.Data.ThemeBackdrop != senders.SelectedIndex.CastPrimitive<CustomThemeBackdrop>()) {
				App.Setting.Data.ThemeBackdrop = senders.SelectedIndex.CastPrimitive<CustomThemeBackdrop>();
				await App.Setting.Save();
			}
			return;
		}

		// ----------------

		public List<String> uSettingThemeMode_ItemsSource {
			get {
				return Enum.GetNames<CustomThemeMode>().Select(ConvertHelper.InsertSpaceBetweenStringWord).ToList();
			}
		}

		public Size uSettingThemeMode_SelectedIndex {
			get {
				return App.Setting.Data.ThemeMode.CastPrimitive<Size>();
			}
		}

		public async void uSettingThemeMode_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Setting.Data.ThemeMode != senders.SelectedIndex.CastPrimitive<CustomThemeMode>()) {
				App.Setting.Data.ThemeMode = senders.SelectedIndex.CastPrimitive<CustomThemeMode>();
				await App.Setting.Save();
			}
			return;
		}

		// ----------------

		public Boolean uSettingThemeColor_IsChecked {
			get {
				return App.Setting.Data.ThemeColorState;
			}
		}

		public async void uSettingThemeColor_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Setting.Data.ThemeColorState = senders.IsChecked;
			this.NotifyPropertyChanged([
				nameof(this.uSettingThemeColor_IsChecked),
				nameof(this.uSettingThemeColor_Content),
			]);
			await App.Setting.Save();
			return;
		}

		public String uSettingThemeColor_Content {
			get {
				return !App.Setting.Data.ThemeColorState ? "Default" : "Custom";
			}
		}

		public async void uSettingThemeColorLight_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ColorPicker>();
			App.Setting.Data.ThemeColorLight = ConvertHelper.MakeColorToInteger(senders.Color);
			this.NotifyPropertyChanged([
				nameof(this.uSettingThemeColorLight_Color),
			]);
			await App.Setting.Save();
			return;
		}

		public Color uSettingThemeColorLight_Color {
			get {
				return ConvertHelper.ParseColorFromInteger(App.Setting.Data.ThemeColorLight);
			}
		}

		public async void uSettingThemeColorDark_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ColorPicker>();
			App.Setting.Data.ThemeColorDark = ConvertHelper.MakeColorToInteger(senders.Color);
			this.NotifyPropertyChanged([
				nameof(this.uSettingThemeColorDark_Color),
			]);
			await App.Setting.Save();
			return;
		}

		public Color uSettingThemeColorDark_Color {
			get {
				return ConvertHelper.ParseColorFromInteger(App.Setting.Data.ThemeColorDark);
			}
		}

		// ----------------

		public Boolean uSettingWindowPosition_IsChecked {
			get {
				return App.Setting.Data.WindowPositionState;
			}
		}

		public async void uSettingWindowPosition_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Setting.Data.WindowPositionState = senders.IsChecked;
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowPosition_IsChecked),
				nameof(this.uSettingWindowPosition_Content),
			]);
			await App.Setting.Save();
			return;
		}

		public String uSettingWindowPosition_Content {
			get {
				return !App.Setting.Data.WindowPositionState ? "Default" : "Custom";
			}
		}

		public async void uSettingWindowPositionX_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.WindowPositionX = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowPositionX_Value),
			]);
			await App.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowPositionX_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowPositionX_Value {
			get {
				return App.Setting.Data.WindowPositionX;
			}
		}

		public async void uSettingWindowPositionY_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.WindowPositionY = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowPositionY_Value),
			]);
			await App.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowPositionY_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowPositionY_Value {
			get {
				return App.Setting.Data.WindowPositionY;
			}
		}

		// ----------------

		public Boolean uSettingWindowSize_IsChecked {
			get {
				return App.Setting.Data.WindowSizeState;
			}
		}

		public async void uSettingWindowSize_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Setting.Data.WindowSizeState = senders.IsChecked;
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowSize_IsChecked),
				nameof(this.uSettingWindowSize_Content),
			]);
			await App.Setting.Save();
			return;
		}

		public String uSettingWindowSize_Content {
			get {
				return !App.Setting.Data.WindowSizeState ? "Default" : "Custom";
			}
		}

		public async void uSettingWindowSizeWidth_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.WindowSizeWidth = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowSizeWidth_Value),
			]);
			await App.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowSizeWidth_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowSizeWidth_Value {
			get {
				return App.Setting.Data.WindowSizeWidth;
			}
		}

		public async void uSettingWindowSizeHeight_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.WindowSizeHeight = senders.Value.CastPrimitive<Integer>();
			}
			this.NotifyPropertyChanged([
				nameof(this.uSettingWindowSizeHeight_Value),
			]);
			await App.Setting.Save();
			return;
		}

		public DecimalFormatter uSettingWindowSizeHeight_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uSettingWindowSizeHeight_Value {
			get {
				return App.Setting.Data.WindowSizeHeight;
			}
		}

		// ----------------

		public Boolean uSettingForwarderExtension_IsChecked {
			get {
				return ForwarderExtensionHelper.Check();
			}
		}

		public async void uSettingForwarderExtension_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			ForwarderExtensionHelper.Toggle();
			this.NotifyPropertyChanged([
				nameof(this.uSettingForwarderExtension_IsChecked),
				nameof(this.uSettingForwarderExtension_Content),
			]);
			return;
		}

		public String uSettingForwarderExtension_Content {
			get {
				return !ForwarderExtensionHelper.Check() ? "Disabled" : "Enabled";
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
				return App.Setting.Data.ForwarderDefaultTarget.CastPrimitive<Size>();
			}
		}

		public async void uSettingForwarderDefaultTarget_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Setting.Data.ForwarderDefaultTarget != senders.SelectedIndex.CastPrimitive<ModuleType>()) {
				App.Setting.Data.ForwarderDefaultTarget = senders.SelectedIndex.CastPrimitive<ModuleType>();
				await App.Setting.Save();
			}
			return;
		}

		// ----------------

		public Boolean uSettingForwarderImmediateJump_IsChecked {
			get {
				return App.Setting.Data.ForwarderImmediateJump;
			}
		}

		public async void uSettingForwarderImmediateJump_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			App.Setting.Data.ForwarderImmediateJump = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uSettingForwarderImmediateJump_IsChecked),
				nameof(this.uSettingForwarderImmediateJump_Content),
			]);
			await App.Setting.Save();
			return;
		}

		public String uSettingForwarderImmediateJump_Content {
			get {
				return !App.Setting.Data.ForwarderImmediateJump ? "Disabled" : "Enabled";
			}
		}

		#endregion

		#region storage

		public async void uStoragePackageDirectory_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.Reveal(App.PackageDirectory);
			await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		// ----------------

		public async void uStorageSettingFile_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.Reveal(App.Setting.File);
			await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		public async void uStorageSettingFileMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			var changed = false;
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await App.Setting.Load();
					await App.Setting.Save();
					changed = true;
					break;
				}
				case "Reset": {
					if (await ControlHelper.ShowDialogForConfirm(this.View, null, null)) {
						await App.Setting.Reset();
						await App.Setting.Save();
						changed = true;
					}
					break;
				}
				case "Import": {
					var file = await StorageHelper.PickLoadFile(App.MainWindow, $"@Application.SettingFile");
					if (file != null) {
						await App.Setting.Load(file);
						await App.Setting.Save();
						changed = true;
					}
					break;
				}
				case "Export": {
					var file = await StorageHelper.PickSaveFile(App.MainWindow, $"@Application.SettingFile", "Setting.json");
					if (file != null) {
						await App.Setting.Save(file, false);
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
				await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			}
			return;
		}

		// ----------------

		public async void uStorageSharedDirectory_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await StorageHelper.Reveal(App.SharedDirectory);
			await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		// ----------------

		public async void uStorageCacheDirectory_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (StorageHelper.Exist(App.CacheDirectory)) {
				StorageHelper.Remove(App.CacheDirectory);
			}
			await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Done!", "");
			return;
		}

		#endregion

	}

}
