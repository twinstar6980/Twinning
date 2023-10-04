#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Navigation;

namespace Helper.Module.ApplicationSetting {

	public sealed partial class MainPage : Page, ITabItemPage {

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
			this.Controller.ApplyOption(args.Parameter as List<String>);
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		public MainPageController Controller { get; }

		#endregion

		#region tab item page

		public async Task<Boolean> OnTabItemCloseRequested (
		) {
			return await this.Controller.RequestClose();
		}

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

		public async void ApplyOption (
			List<String>? optionView
		) {
			while (!this.View.IsLoaded) {
				await Task.Delay(40);
			}
			try {
				var option = new CommandLineReader(optionView);
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			} catch (Exception e) {
				App.MainWindow.Controller.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			return;
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		#endregion

		#region page

		public async void uPage_OnDragOver (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			return;
		}

		public async void uPage_OnDrop (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
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
				nameof(this.uAppearanceThemeModeBar_Description)
			);
			return;
		}

		#endregion

		#region modding worker

		public async void uModdingWorkerOpen_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await App.MainWindow.Controller.InsertTabItem(ModuleType.ModdingWorker, null);
			return;
		}

		// ----------------

		public String uModdingWorkerKernelBar_Description {
			get {
				return Setting.ModdingWorkerKernel;
			}
		}

		public String uModdingWorkerKernelText_Text {
			get {
				return Setting.ModdingWorkerKernel;
			}
		}

		public async void uModdingWorkerKernelText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.ModdingWorkerKernel = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerKernelBar_Description)
			);
			return;
		}

		// ----------------

		public String uModdingWorkerScriptBar_Description {
			get {
				return Setting.ModdingWorkerScript;
			}
		}

		public String uModdingWorkerScriptText_Text {
			get {
				return Setting.ModdingWorkerScript;
			}
		}

		public async void uModdingWorkerScriptText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.ModdingWorkerScript = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerScriptBar_Description)
			);
			return;
		}

		// ----------------

		public String uModdingWorkerArgumentBar_Description {
			get {
				return String.Join(' ', Setting.ModdingWorkerArgument);
			}
		}

		public String uModdingWorkerArgumentText_Text {
			get {
				return String.Join('\r', Setting.ModdingWorkerArgument);
			}
		}

		public async void uModdingWorkerArgumentText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			var parsedValue = senders.Text.Split('\r').ToList();
			if (parsedValue.Count != 0 && parsedValue.Last().Length == 0) {
				parsedValue.RemoveAt(parsedValue.Count - 1);
			}
			Setting.ModdingWorkerArgument = parsedValue;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerArgumentBar_Description)
			);
			return;
		}

		// ----------------

		public String uModdingWorkerAutomaticCloseBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ModdingWorkerAutomaticClose);
			}
		}

		public Boolean uModdingWorkerAutomaticCloseSwitch_IsOn {
			get {
				return Setting.ModdingWorkerAutomaticClose;
			}
		}

		public async void uModdingWorkerAutomaticCloseSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ModdingWorkerAutomaticClose = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerAutomaticCloseBar_Description)
			);
			return;
		}

		// ----------------

		public String uModdingWorkerAutomaticScrollBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ModdingWorkerAutomaticScroll);
			}
		}

		public Boolean uModdingWorkerAutomaticScrollSwitch_IsOn {
			get {
				return Setting.ModdingWorkerAutomaticScroll;
			}
		}

		public async void uModdingWorkerAutomaticScrollSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ModdingWorkerAutomaticScroll = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerAutomaticScrollBar_Description)
			);
			return;
		}

		// ----------------

		public String uModdingWorkerImmediateLaunchBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ModdingWorkerImmediateLaunch);
			}
		}

		public Boolean uModdingWorkerImmediateLaunchSwitch_IsOn {
			get {
				return Setting.ModdingWorkerImmediateLaunch;
			}
		}

		public async void uModdingWorkerImmediateLaunchSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ModdingWorkerImmediateLaunch = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerImmediateLaunchBar_Description)
			);
			return;
		}

		// ----------------

		public String uModdingWorkerAlternativeLaunchScriptBar_Description {
			get {
				return Setting.ModdingWorkerAlternativeLaunchScript;
			}
		}

		public String uModdingWorkerAlternativeLaunchScriptText_Text {
			get {
				return Setting.ModdingWorkerAlternativeLaunchScript;
			}
		}

		public async void uModdingWorkerAlternativeLaunchScriptText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.ModdingWorkerAlternativeLaunchScript = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uModdingWorkerAlternativeLaunchScriptBar_Description)
			);
			return;
		}

		public async void uModdingWorkerAlternativeLaunchScriptSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.ModdingWorkerAlternativeLaunchScript = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uModdingWorkerAlternativeLaunchScriptBar_Description),
					nameof(this.uModdingWorkerAlternativeLaunchScriptText_Text)
				);
			}
			return;
		}

		#endregion

		#region resource forwarder

		public async void uResourceForwarderOpen_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await App.MainWindow.Controller.InsertTabItem(ModuleType.ResourceForwarder, null);
			return;
		}

		// ----------------

		public String uResourceForwarderOptionConfigurationBar_Description {
			get {
				return Setting.ResourceForwarderOptionConfiguration;
			}
		}

		public String uResourceForwarderOptionConfigurationText_Text {
			get {
				return Setting.ResourceForwarderOptionConfiguration;
			}
		}

		public async void uResourceForwarderOptionConfigurationText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.ResourceForwarderOptionConfiguration = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uResourceForwarderOptionConfigurationBar_Description)
			);
			return;
		}

		public async void uResourceForwarderOptionConfigurationSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.ResourceForwarderOptionConfiguration = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uResourceForwarderOptionConfigurationBar_Description),
					nameof(this.uResourceForwarderOptionConfigurationText_Text)
				);
			}
			return;
		}

		// ----------------

		public String uResourceForwarderAutomaticCloseBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ResourceForwarderAutomaticClose);
			}
		}

		public Boolean uResourceForwarderAutomaticCloseSwitch_IsOn {
			get {
				return Setting.ResourceForwarderAutomaticClose;
			}
		}

		public async void uResourceForwarderAutomaticCloseSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ResourceForwarderAutomaticClose = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uResourceForwarderAutomaticCloseBar_Description)
			);
			return;
		}

		// ----------------

		public String uResourceForwarderParallelExecuteBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ResourceForwarderParallelExecute);
			}
		}

		public Boolean uResourceForwarderParallelExecuteSwitch_IsOn {
			get {
				return Setting.ResourceForwarderParallelExecute;
			}
		}

		public async void uResourceForwarderParallelExecuteSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ResourceForwarderParallelExecute = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uResourceForwarderParallelExecuteBar_Description)
			);
			return;
		}

		// ----------------

		public String uResourceForwarderEnableFilterBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ResourceForwarderEnableFilter);
			}
		}

		public Boolean uResourceForwarderEnableFilterSwitch_IsOn {
			get {
				return Setting.ResourceForwarderEnableFilter;
			}
		}

		public async void uResourceForwarderEnableFilterSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ResourceForwarderEnableFilter = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uResourceForwarderEnableFilterBar_Description)
			);
			return;
		}

		// ----------------

		public String uResourceForwarderEnableBatchBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ResourceForwarderEnableBatch);
			}
		}

		public Boolean uResourceForwarderEnableBatchSwitch_IsOn {
			get {
				return Setting.ResourceForwarderEnableBatch;
			}
		}

		public async void uResourceForwarderEnableBatchSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ResourceForwarderEnableBatch = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uResourceForwarderEnableBatchBar_Description)
			);
			return;
		}

		// ----------------

		public String uResourceForwarderRemainInputBar_Description {
			get {
				return ConvertHelper.BooleanToSwitchString(Setting.ResourceForwarderRemainInput);
			}
		}

		public Boolean uResourceForwarderRemainInputSwitch_IsOn {
			get {
				return Setting.ResourceForwarderRemainInput;
			}
		}

		public async void uResourceForwarderRemainInputSwitch_OnToggled (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleSwitch senders) { return; }
			var newValue = senders.IsOn;
			Setting.ResourceForwarderRemainInput = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uResourceForwarderRemainInputBar_Description)
			);
			return;
		}

		#endregion

		#region command forwarder

		public async void uCommandSenderOpen_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await App.MainWindow.Controller.InsertTabItem(ModuleType.CommandSender, null);
			return;
		}

		// ----------------

		public String uCommandSenderMethodConfigurationBar_Description {
			get {
				return Setting.CommandSenderMethodConfiguration;
			}
		}

		public String uCommandSenderMethodConfigurationText_Text {
			get {
				return Setting.CommandSenderMethodConfiguration;
			}
		}

		public async void uCommandSenderMethodConfigurationText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.CommandSenderMethodConfiguration = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uCommandSenderMethodConfigurationBar_Description)
			);
			return;
		}

		public async void uCommandSenderMethodConfigurationSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				Setting.CommandSenderMethodConfiguration = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uCommandSenderMethodConfigurationBar_Description),
					nameof(this.uCommandSenderMethodConfigurationText_Text)
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
			await App.MainWindow.Controller.InsertTabItem(ModuleType.AnimationViewer, null);
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
				nameof(this.uAnimationViewerImmediateSelectBar_Description)
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
				nameof(this.uAnimationViewerAutomaticPlayBar_Description)
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
				nameof(this.uAnimationViewerRepeatPlayBar_Description)
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
				nameof(this.uAnimationViewerRemainFrameRateBar_Description)
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
				nameof(this.uAnimationViewerShowSpriteBoundaryBar_Description)
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
				nameof(this.uAnimationViewerSpriteFilterRuleBar_Description)
			);
			return;
		}

		#endregion

	}

}
