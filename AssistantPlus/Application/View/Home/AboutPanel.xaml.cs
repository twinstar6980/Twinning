#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.Globalization.NumberFormatting;
using Windows.UI;

namespace AssistantPlus.View.Home {

	public sealed partial class AboutPanel : CustomControl {

		#region life

		public AboutPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private AboutPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		#endregion

	}

	public class AboutPanelController : CustomController {

		#region data

		public AboutPanel View { get; init; } = default!;

		// ----------------

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
			);
			return;
		}

		#endregion

		#region description

		public String uDescriptionLabel_Text {
			get {
				return $"{Package.Current.DisplayName} - {Package.Current.Id.Version.Major}";
			}
		}

		#endregion

		#region setting

		public List<String> uSettingThemeMode_ItemsSource {
			get {
				return Enum.GetNames<CustomThemeMode>().Select(ConvertHelper.InsertSpaceBetweenStringWord).ToList();
			}
		}

		public Size uSettingThemeMode_SelectedIndex {
			get {
				return (Size)App.Setting.Data.Theme.Mode;
			}
		}

		public async void uSettingThemeMode_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Setting.Data.Theme.Mode != (CustomThemeMode)senders.SelectedIndex) {
				App.Setting.Data.Theme.Mode = (CustomThemeMode)senders.SelectedIndex;
				await App.Setting.Save();
			}
			return;
		}

		// ----------------

		public Boolean uSettingThemeColor_IsChecked {
			get {
				return App.Setting.Data.Theme.Color.State;
			}
		}

		public async void uSettingThemeColor_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Setting.Data.Theme.Color.State = senders.IsChecked;
			this.NotifyPropertyChanged(
				nameof(this.uSettingThemeColor_IsChecked),
				nameof(this.uSettingThemeColor_Content)
			);
			await App.Setting.Save();
			return;
		}

		public String uSettingThemeColor_Content {
			get {
				return !App.Setting.Data.Theme.Color.State ? "Default" : "Custom";
			}
		}

		public async void uSettingThemeColorLight_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ColorPicker>();
			App.Setting.Data.Theme.Color.LightRed = senders.Color.R;
			App.Setting.Data.Theme.Color.LightGreen = senders.Color.G;
			App.Setting.Data.Theme.Color.LightBlue = senders.Color.B;
			this.NotifyPropertyChanged(
				nameof(this.uSettingThemeColorLight_Color)
			);
			await App.Setting.Save();
			return;
		}

		public Color uSettingThemeColorLight_Color {
			get {
				return Color.FromArgb(
					0xFF,
					(Byte)App.Setting.Data.Theme.Color.LightRed,
					(Byte)App.Setting.Data.Theme.Color.LightGreen,
					(Byte)App.Setting.Data.Theme.Color.LightBlue
				);
			}
		}

		public async void uSettingThemeColorDark_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ColorPicker>();
			App.Setting.Data.Theme.Color.DarkRed = senders.Color.R;
			App.Setting.Data.Theme.Color.DarkGreen = senders.Color.G;
			App.Setting.Data.Theme.Color.DarkBlue = senders.Color.B;
			this.NotifyPropertyChanged(
				nameof(this.uSettingThemeColorDark_Color)
			);
			await App.Setting.Save();
			return;
		}

		public Color uSettingThemeColorDark_Color {
			get {
				return Color.FromArgb(
					0xFF,
					(Byte)App.Setting.Data.Theme.Color.DarkRed,
					(Byte)App.Setting.Data.Theme.Color.DarkGreen,
					(Byte)App.Setting.Data.Theme.Color.DarkBlue
				);
			}
		}

		// ----------------

		public List<String> uSettingThemeBackdrop_ItemsSource {
			get {
				return Enum.GetNames<CustomThemeBackdrop>().Select(ConvertHelper.InsertSpaceBetweenStringWord).ToList();
			}
		}

		public Size uSettingThemeBackdrop_SelectedIndex {
			get {
				return (Size)App.Setting.Data.Theme.Backdrop;
			}
		}

		public async void uSettingThemeBackdrop_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (App.Setting.Data.Theme.Backdrop != (CustomThemeBackdrop)senders.SelectedIndex) {
				App.Setting.Data.Theme.Backdrop = (CustomThemeBackdrop)senders.SelectedIndex;
				await App.Setting.Save();
			}
			return;
		}

		// ----------------

		public Boolean uSettingWindowPosition_IsChecked {
			get {
				return App.Setting.Data.Window.Position.State;
			}
		}

		public async void uSettingWindowPosition_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Setting.Data.Window.Position.State = senders.IsChecked;
			this.NotifyPropertyChanged(
				nameof(this.uSettingWindowPosition_IsChecked),
				nameof(this.uSettingWindowPosition_Content)
			);
			await App.Setting.Save();
			return;
		}

		public String uSettingWindowPosition_Content {
			get {
				return !App.Setting.Data.Window.Position.State ? "Default" : "Custom";
			}
		}

		public async void uSettingWindowPositionX_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.Window.Position.X = (Integer)senders.Value;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSettingWindowPositionX_Value)
			);
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
				return App.Setting.Data.Window.Position.X;
			}
		}

		public async void uSettingWindowPositionY_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.Window.Position.Y = (Integer)senders.Value;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSettingWindowPositionY_Value)
			);
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
				return App.Setting.Data.Window.Position.Y;
			}
		}

		// ----------------

		public Boolean uSettingWindowSize_IsChecked {
			get {
				return App.Setting.Data.Window.Size.State;
			}
		}

		public async void uSettingWindowSize_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<ToggleSplitButton>();
			App.Setting.Data.Window.Size.State = senders.IsChecked;
			this.NotifyPropertyChanged(
				nameof(this.uSettingWindowSize_IsChecked),
				nameof(this.uSettingWindowSize_Content)
			);
			await App.Setting.Save();
			return;
		}

		public String uSettingWindowSize_Content {
			get {
				return !App.Setting.Data.Window.Size.State ? "Default" : "Custom";
			}
		}

		public async void uSettingWindowSizeWidth_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.Window.Size.Width = (Integer)senders.Value;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSettingWindowSizeWidth_Value)
			);
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
				return App.Setting.Data.Window.Size.Width;
			}
		}

		public async void uSettingWindowSizeHeight_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (Floater.IsFinite(senders.Value)) {
				App.Setting.Data.Window.Size.Height = (Integer)senders.Value;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSettingWindowSizeHeight_Value)
			);
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
				return App.Setting.Data.Window.Size.Height;
			}
		}

		#endregion

		#region storage

		public async void uStoragePackageDirectory_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.RevealDirectory(App.PackageDirectory);
			return;
		}

		// ----------------

		public async void uStorageSettingFile_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.RevealFile(App.Setting.File);
			return;
		}

		public async void uStorageSettingFileMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await App.Setting.Load();
					await App.Setting.Save();
					break;
				}
				case "Reset": {
					if (await ControlHelper.ShowDialogForConfirm(this.View, null, null)) {
						await App.Setting.Reset();
						await App.Setting.Save();
					}
					break;
				}
				case "Import": {
					var file = await StorageHelper.PickLoadFile(WindowHelper.Find(this.View), $"Application.SettingFile");
					if (file != null) {
						await App.Setting.Load(file);
						await App.Setting.Save();
					}
					break;
				}
				case "Export": {
					var file = await StorageHelper.PickSaveFile(WindowHelper.Find(this.View), $"Application.SettingFile", "json", $"{Package.Current.Id.Name}@Setting");
					if (file != null) {
						await App.Setting.Save(file, false);
					}
					break;
				}
				default: throw new ();
			}
			this.NotifyPropertyChanged(
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
				nameof(this.uSettingWindowSizeHeight_Value)
			);
			return;
		}

		// ----------------

		public async void uStorageSharedDirectory_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			await StorageHelper.RevealDirectory(App.SharedDirectory);
			return;
		}

		// ----------------

		public async void uStorageCacheDirectory_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			StorageHelper.RemoveDirectory(App.CacheDirectory);
			return;
		}

		#endregion

	}

}
