#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.HomeSetting {

	public sealed partial class MainPage : Page {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new MainPageController() { View = this };
			this.Controller.Update();
		}

		// ----------------

		public MainPageController Controller { get; }

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public MainPage View { get; init; } = default!;

		#endregion

		#region update

		public async void Update (
		) {
			return;
		}

		#endregion

		#region appearance

		public String uAppearanceThemeModeBar_Description {
			get {
				return ConvertHelper.ThemeToString(Setting.AppearanceThemeMode);
			}
		}

		public List<String> uAppearanceThemeModeRadio_ItemsSource {
			get {
				return new[] { ElementTheme.Default, ElementTheme.Light, ElementTheme.Dark }.Select((value) => (ConvertHelper.ThemeToString(value))).ToList();
			}
		}

		public Size uAppearanceThemeModeRadio_SelectedIndex {
			get {
				return (Size)Setting.AppearanceThemeMode;
			}
		}

		public async void uAppearanceThemeModeRadio_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not RadioButtons senders) { return; }
			if (senders.SelectedIndex == -1) {
				return;
			}
			var newValue = (ElementTheme)senders.SelectedIndex;
			Setting.AppearanceThemeMode = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAppearanceThemeModeBar_Description),
				nameof(this.uAppearanceThemeModeRadio_SelectedIndex)
			);
			return;
		}

		#endregion

		#region command forwarder

		public async void uCommandForwarderOpen_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			MainWindow.Instance.Controller.PushTabItem(ModuleType.CommandForwarder, true);
			return;
		}

		// ----------------

		public String uCommandForwarderLaunchScriptBar_Description {
			get {
				return Setting.CommandForwarderLaunchScript;
			}
		}

		public String uCommandForwarderLaunchScriptText_Text {
			get {
				return Setting.CommandForwarderLaunchScript;
			}
		}

		public async void uCommandForwarderLaunchScriptText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.CommandForwarderLaunchScript = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uCommandForwarderLaunchScriptBar_Description),
				nameof(this.uCommandForwarderLaunchScriptText_Text)
			);
			return;
		}

		public async void uCommandForwarderLaunchScriptSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetWindowForElement(this.View) ?? throw new Exception());
			if (newValue is not null) {
				Setting.CommandForwarderLaunchScript = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uCommandForwarderLaunchScriptBar_Description),
					nameof(this.uCommandForwarderLaunchScriptText_Text)
				);
			}
			return;
		}

		// ----------------

		public String uCommandForwarderMethodConfigurationBar_Description {
			get {
				return Setting.CommandForwarderMethodConfiguration;
			}
		}

		public String uCommandForwarderMethodConfigurationText_Text {
			get {
				return Setting.CommandForwarderMethodConfiguration;
			}
		}

		public async void uCommandForwarderMethodConfigurationText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.CommandForwarderMethodConfiguration = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uCommandForwarderMethodConfigurationBar_Description),
				nameof(this.uCommandForwarderMethodConfigurationText_Text)
			);
			return;
		}

		public async void uCommandForwarderMethodConfigurationSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetWindowForElement(this.View) ?? throw new Exception());
			if (newValue is not null) {
				Setting.CommandForwarderMethodConfiguration = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uCommandForwarderMethodConfigurationBar_Description),
					nameof(this.uCommandForwarderMethodConfigurationText_Text)
				);
			}
			return;
		}

		#endregion

		#region animation viewer

		public async void uAnimationViewerOpen_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			MainWindow.Instance.Controller.PushTabItem(ModuleType.AnimationViewer, true);
			return;
		}

		// ----------------

		public String uAnimationViewerAutomaticPlayBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.AnimationViewerAutomaticPlay);
			}
		}

		public Boolean uAnimationViewerAutomaticPlaySwitch_IsOn {
			get {
				return Setting.AnimationViewerAutomaticPlay;
			}
		}

		public async void uAnimationViewerAutomaticPlaySwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.AnimationViewerAutomaticPlay = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationViewerAutomaticPlayBar_Description),
				nameof(this.uAnimationViewerAutomaticPlaySwitch_IsOn)
			);
			return;
		}

		// ----------------

		public String uAnimationViewerRepeatPlayBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.AnimationViewerRepeatPlay);
			}
		}

		public Boolean uAnimationViewerRepeatPlaySwitch_IsOn {
			get {
				return Setting.AnimationViewerRepeatPlay;
			}
		}

		public async void uAnimationViewerRepeatPlaySwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.AnimationViewerRepeatPlay = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationViewerRepeatPlayBar_Description),
				nameof(this.uAnimationViewerRepeatPlaySwitch_IsOn)
			);
			return;
		}

		// ----------------

		public String uAnimationViewerRemainFrameRateBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.AnimationViewerRemainFrameRate);
			}
		}

		public Boolean uAnimationViewerRemainFrameRateSwitch_IsOn {
			get {
				return Setting.AnimationViewerRemainFrameRate;
			}
		}

		public async void uAnimationViewerRemainFrameRateSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.AnimationViewerRemainFrameRate = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationViewerRemainFrameRateBar_Description),
				nameof(this.uAnimationViewerRemainFrameRateSwitch_IsOn)
			);
			return;
		}

		#endregion

	}

}
