#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Navigation;
using Windows.Globalization.NumberFormatting;

namespace Helper.Module.ApplicationSetting {

	public sealed partial class MainPage : Page {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new MainPageController() { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		protected override void OnNavigatedTo (
			NavigationEventArgs args
		) {
			if (args.Parameter is List<String> option) {
				this.Controller.ApplyCommandOption(option);
			}
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		public MainPageController Controller { get; }

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public MainPage View { get; init; } = default!;

		#endregion

		#region initialize

		public void Initialize (
		) {
			return;
		}

		public async void ApplyCommandOption (
			List<String> optionView
		) {
			while (!this.View.IsLoaded) {
				await Task.Delay(TimeSpan.FromMilliseconds(50));
			}
			try {
				var option = new CommandLineReader(optionView);
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
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
			MainWindow.Instance.Controller.PushTabItem(ModuleType.CommandForwarder, null, true);
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
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
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

		public String uCommandForwarderCommandConfigurationBar_Description {
			get {
				return Setting.CommandForwarderCommandConfiguration;
			}
		}

		public String uCommandForwarderCommandConfigurationText_Text {
			get {
				return Setting.CommandForwarderCommandConfiguration;
			}
		}

		public async void uCommandForwarderCommandConfigurationText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.CommandForwarderCommandConfiguration = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uCommandForwarderCommandConfigurationBar_Description),
				nameof(this.uCommandForwarderCommandConfigurationText_Text)
			);
			return;
		}

		public async void uCommandForwarderCommandConfigurationSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.CommandForwarderCommandConfiguration = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uCommandForwarderCommandConfigurationBar_Description),
					nameof(this.uCommandForwarderCommandConfigurationText_Text)
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
			MainWindow.Instance.Controller.PushTabItem(ModuleType.AnimationViewer, null, true);
			return;
		}

		// ----------------

		public String uAnimationViewerImmediateSelectBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.AnimationViewerImmediateSelect);
			}
		}

		public Boolean uAnimationViewerImmediateSelectSwitch_IsOn {
			get {
				return Setting.AnimationViewerImmediateSelect;
			}
		}

		public async void uAnimationViewerImmediateSelectSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.AnimationViewerImmediateSelect = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationViewerImmediateSelectBar_Description),
				nameof(this.uAnimationViewerImmediateSelectSwitch_IsOn)
			);
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

		// ----------------

		public String uAnimationViewerShowSpriteBoundaryBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.AnimationViewerShowSpriteBoundary);
			}
		}

		public Boolean uAnimationViewerShowSpriteBoundarySwitch_IsOn {
			get {
				return Setting.AnimationViewerShowSpriteBoundary;
			}
		}

		public async void uAnimationViewerShowSpriteBoundarySwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.AnimationViewerShowSpriteBoundary = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationViewerShowSpriteBoundaryBar_Description),
				nameof(this.uAnimationViewerShowSpriteBoundarySwitch_IsOn)
			);
			return;
		}

		// ----------------

		public String uAnimationViewerSpriteFilterRuleBar_Description {
			get {
				return Setting.AnimationViewerSpriteFilterRule;
			}
		}

		public String uAnimationViewerSpriteFilterRuleText_Text {
			get {
				return Setting.AnimationViewerSpriteFilterRule;
			}
		}

		public async void uAnimationViewerSpriteFilterRuleText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.AnimationViewerSpriteFilterRule = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationViewerSpriteFilterRuleBar_Description),
				nameof(this.uAnimationViewerSpriteFilterRuleText_Text)
			);
			return;
		}

		#endregion

	}

}
