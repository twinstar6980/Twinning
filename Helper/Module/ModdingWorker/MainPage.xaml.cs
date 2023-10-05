#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.ApplicationModel;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Navigation;
using Microsoft.Windows.AppNotifications.Builder;

namespace Helper.Module.ModdingWorker {

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

		// ----------------

		public Boolean AutomaticClose { get; set; } = default!;

		public Boolean AutomaticScroll { get; set; } = default!;

		// ----------------

		public Boolean ImmediateLaunch { get; set; } = default!;

		public List<String> AdditionalArgument { get; set; } = new List<String>();

		// ----------------

		public HelperHost SessionHost { get; set; } = default!;

		public Task<String>? SessionTask { get; set; } = null;

		public Boolean InputBusy { get; set; } = false;

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.AutomaticClose = Setting.ModdingWorkerAutomaticClose;
			this.AutomaticScroll = Setting.ModdingWorkerAutomaticScroll;
			this.ImmediateLaunch = Setting.ModdingWorkerImmediateLaunch;
			this.SessionHost = new HelperHost(this);
			return;
		}

		public async void ApplyOption (
			List<String>? optionView
		) {
			while (!this.View.IsLoaded) {
				await Task.Delay(40);
			}
			var optionAutomaticClose = default(Boolean?);
			var optionAutomaticScroll = default(Boolean?);
			var optionImmediateLaunch = default(Boolean?);
			var optionAdditionalArgument = default(List<String>?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Ensure("-AutomaticClose")) {
					optionAutomaticClose = option.NextBoolean();
				}
				if (option.Ensure("-AutomaticScroll")) {
					optionAutomaticScroll = option.NextBoolean();
				}
				if (option.Ensure("-ImmediateLaunch")) {
					optionImmediateLaunch = option.NextBoolean();
				}
				if (option.Ensure("-AdditionalArgument")) {
					optionAdditionalArgument = option.NextStringList();
				}
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			} catch (Exception e) {
				App.MainWindow.Controller.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionAutomaticClose is not null) {
				this.AutomaticClose = optionAutomaticClose.Value;
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticClose_Visibility)
				);
			}
			if (optionAutomaticScroll is not null) {
				this.AutomaticScroll = optionAutomaticScroll.Value;
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticScroll_IsChecked)
				);
			}
			if (optionImmediateLaunch is not null) {
				this.ImmediateLaunch = optionImmediateLaunch.Value;
			}
			if (optionAdditionalArgument is not null) {
				this.AdditionalArgument = optionAdditionalArgument;
				this.NotifyPropertyChanged(
					nameof(this.uAdditionalArgumentCount_Text),
					nameof(this.uAdditionalArgument_Text)
				);
			}
			if (this.ImmediateLaunch) {
				this.LaunchSession().Wait(0);
			}
			return;
		}

		public async Task<Boolean> RequestClose (
		) {
			if (this.SessionTask is not null) {
				await new ContentDialog() {
					XamlRoot = this.View.XamlRoot,
					Title = "Session In Progress",
					CloseButtonText = "Cancel",
					DefaultButton = ContentDialogButton.Close,
				}.ShowAsync();
				return false;
			}
			return true;
		}

		// ----------------

		public async Task LaunchSession (
		) {
			var result = default(String?);
			var exception = default(Exception?);
			var kernel = new String(Setting.ModdingWorkerKernel);
			var script = new String(Setting.ModdingWorkerScript);
			var argument = new List<String>(new[] { Setting.ModdingWorkerArgument, this.AdditionalArgument }.SelectMany((value) => (value)));
			var temporaryKernel = StorageHelper.GetTemporaryPath();
			var library = new Bridge.Library();
			this.SessionTask = new Task<String>(() => (Bridge.Launcher.Launch(this.SessionHost, library, script, argument)));
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uInput_Visibility),
				nameof(this.uProgress_Visibility)
			);
			try {
				this.uOutput_ItemsSource.Clear();
				StorageHelper.CopyFile(kernel, temporaryKernel);
				library.Open(temporaryKernel);
				this.SessionTask.Start();
				result = await this.SessionTask;
			} catch (Exception e) {
				exception = e;
			}
			if (library.State()) {
				library.Close();
			}
			if (StorageHelper.ExistFile(temporaryKernel)) {
				StorageHelper.RemoveFile(temporaryKernel);
			}
			if (exception is null) {
				await this.PublishOutput(MessageType.Success, "SUCCEEDED", new List<String>() { result! });
			} else {
				await this.PublishOutput(MessageType.Error, "FAILED", new List<String>() { exception.ToString() });
			}
			this.SessionTask = null;
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uInput_Visibility),
				nameof(this.uProgress_Visibility)
			);
			if (exception is null && this.AutomaticClose) {
				await App.MainWindow.Controller.RemoveTabItem(this.View);
				return;
			}
			return;
		}

		// ----------------

		public async Task PublishOutput (
			MessageType  type,
			String       title,
			List<String> description
		) {
			this.uOutput_ItemsSource.Add(new MainPageOutputBarController() {
				Host = this,
				Type = type,
				Title = title,
				Description = description,
			});
			if (this.AutomaticScroll) {
				this.View.DispatcherQueue.EnqueueAsync(async () => {
					await Task.Delay(40);
					this.View.uOutputScrollViewer.ChangeView(null, this.View.uOutputScrollViewer.ScrollableHeight, null, true);
				}).Wait(0);
			}
			return;
		}

		public async Task<Object?> RequestInput (
			InputType type,
			Object?   option
		) {
			this.InputBusy = true;
			this.NotifyPropertyChanged(
				nameof(this.uProgress_ShowPaused)
			);
			this.uInput_Type = type;
			this.uInput_Option = option;
			this.uInput_Value = new InputValue() { Data = null };
			this.NotifyPropertyChanged(
				nameof(this.uInput_Type),
				nameof(this.uInput_Value),
				nameof(this.uInput_Option),
				nameof(this.uInput_Stamp)
			);
			while (this.InputBusy) {
				await Task.Delay(40);
			}
			var input = this.uInput_Value.Data;
			this.uInput_Type = null;
			this.uInput_Option = null;
			this.uInput_Value = null;
			this.NotifyPropertyChanged(
				nameof(this.uInput_Type),
				nameof(this.uInput_Value),
				nameof(this.uInput_Option),
				nameof(this.uInput_Stamp)
			);
			return input;
		}

		#endregion

		#region page

		public async void uPage_OnDragOver (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPage_OnDrop (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				var item = await args.DataView.GetStorageItemsAsync();
				this.AdditionalArgument.AddRange(item.Select((value) => (StorageHelper.Normalize(value.Path))).ToList());
				this.NotifyPropertyChanged(
					nameof(this.uAdditionalArgumentCount_Text),
					nameof(this.uAdditionalArgument_Text)
				);
			}
			return;
		}

		#endregion

		#region setting

		public Boolean uAutomaticClose_Visibility {
			get {
				return this.AutomaticClose;
			}
		}

		public async void uAutomaticClose_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			this.AutomaticClose = false;
			this.NotifyPropertyChanged(
				nameof(this.uAutomaticClose_Visibility)
			);
			return;
		}

		// ----------------

		public Boolean uAutomaticScroll_IsChecked {
			get {
				return this.AutomaticScroll;
			}
		}

		public async void uAutomaticScroll_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.AutomaticScroll = senders.IsChecked!.Value;
			this.NotifyPropertyChanged(
				nameof(this.uAutomaticScroll_IsChecked)
			);
			return;
		}

		#endregion

		#region additional argument

		public String uAdditionalArgumentCount_Text {
			get {
				return $"{this.AdditionalArgument.Count}";
			}
		}

		// ----------------

		public String uAdditionalArgument_Text {
			get {
				return String.Join('\r', this.AdditionalArgument);
			}
		}

		public async void uAdditionalArgument_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			var parsedValue = senders.Text.Split('\r').ToList();
			if (parsedValue.Count != 0 && parsedValue.Last().Length == 0) {
				parsedValue.RemoveAt(parsedValue.Count - 1);
			}
			this.AdditionalArgument = parsedValue;
			this.NotifyPropertyChanged(
				nameof(this.uAdditionalArgumentCount_Text)
			);
			return;
		}

		#endregion

		#region launch

		public Boolean uLaunch_Visibility {
			get {
				return this.SessionTask is null;
			}
		}

		public async void uLaunch_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (this.SessionTask is null) {
				this.LaunchSession().Wait(0);
			}
			return;
		}

		#endregion

		#region progress

		public Boolean uProgress_Visibility {
			get {
				return this.SessionTask is not null;
			}
		}

		public Boolean uProgress_ShowPaused {
			get {
				return this.InputBusy;
			}
		}

		#endregion

		#region output

		public ObservableCollection<MainPageOutputBarController> uOutput_ItemsSource { get; } = new ObservableCollection<MainPageOutputBarController>();

		#endregion

		#region input

		public Boolean uInput_Visibility {
			get {
				return this.SessionTask is not null;
			}
		}

		public InputType? uInput_Type { get; set; } = null;

		public Object? uInput_Option { get; set; } = null;

		public InputValue? uInput_Value { get; set; } = null;

		public async void uInput_OnValueSubmitted (
			Object? value
		) {
			this.InputBusy = false;
			this.NotifyPropertyChanged(
				nameof(this.uProgress_ShowPaused)
			);
			return;
		}

		public UniqueStamp uInput_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

	}

	public class MainPageOutputBarController : CustomController {

		#region data

		public MainPageController Host = default!;

		// ----------------

		public MessageType Type = default!;

		public String Title = default!;

		public List<String> Description = default!;

		#endregion

		#region view

		public MessageType uRoot_Type {
			get {
				return this.Type;
			}
		}

		public String uRoot_Title {
			get {
				return this.Title;
			}
		}

		public List<String> uRoot_Message {
			get {
				return this.Description;
			}
		}

		public UniqueStamp uRoot_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

	}

	public class HelperHost : Bridge.Host {

		public HelperHost (
			MainPageController controller
		) {
			this.mController = controller;
			this.mRunning = false;
		}

		// ----------------

		private MainPageController mController;

		private Boolean mRunning;

		// ----------------

		public override void Start (
		) {
			Debug.Assert(!this.mRunning);
			this.mRunning = true;
			return;
		}

		public override void Finish (
		) {
			Debug.Assert(this.mRunning);
			this.mRunning = false;
			return;
		}

		// ----------------

		public override List<String> Execute (
			List<String> argument
		) {
			Debug.Assert(this.mRunning);
			var result = new List<String>();
			Debug.Assert(argument.Count >= 1);
			var method = argument[0];
			Task.WaitAll(this.mController.View.DispatcherQueue.EnqueueAsync(async () => {
				switch (method) {
					case "version": {
						Debug.Assert(argument.Count == 1);
						result.Add(Package.Current.Id.Version.Major.ToString());
						break;
					}
					case "host": {
						Debug.Assert(argument.Count == 1);
						result.Add("helper");
						break;
					}
					case "system": {
						Debug.Assert(argument.Count == 1);
						result.Add("windows");
						break;
					}
					case "output_message": {
						Debug.Assert(argument.Count >= 3);
						var type = argument[1];
						var title = argument[2];
						var description = argument.GetRange(3, argument.Count - 3);
						await this.mController.PublishOutput(
							type switch {
								"v" => MessageType.Verbosity,
								"i" => MessageType.Information,
								"w" => MessageType.Warning,
								"e" => MessageType.Error,
								"s" => MessageType.Success,
								"t" => MessageType.Input,
								_   => throw new ArgumentOutOfRangeException(),
							},
							title,
							description
						);
						break;
					}
					case "input_pause": {
						Debug.Assert(argument.Count == 1);
						await this.mController.RequestInput(InputType.Pause, null);
						break;
					}
					case "input_boolean": {
						Debug.Assert(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Boolean, null) as Boolean?;
						result.Add(input is null ? "" : !input.Value ? "n" : "y");
						break;
					}
					case "input_integer": {
						Debug.Assert(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Integer, null) as Integer?;
						result.Add(input is null ? "" : input.Value == 0 ? "0" : (input.Value < 0 ? $"{input}" : $"+{input}"));
						break;
					}
					case "input_floater": {
						Debug.Assert(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Floater, null) as Floater?;
						result.Add(input is null ? "" : input.Value == 0.0 ? "0.0" : (input.Value < 0.0 ? $"{input}{(!Floater.IsInteger(input.Value) ? "" : ".0")}" : $"+{input}{(!Floater.IsInteger(input.Value) ? "" : ".0")}"));
						break;
					}
					case "input_size": {
						Debug.Assert(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Size, null) as SizeExpression;
						result.Add(input is null ? "" : input.ToString());
						break;
					}
					case "input_string": {
						Debug.Assert(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.String, null) as String;
						result.Add(input is null ? "" : input);
						break;
					}
					case "input_path": {
						Debug.Assert(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Path, null) as PathExpression;
						result.Add(input is null ? "" : input.ToString());
						break;
					}
					case "input_enumeration": {
						Debug.Assert(argument.Count >= 1);
						var option = argument.GetRange(1, argument.Count - 1);
						var input = await this.mController.RequestInput(InputType.Enumeration, option) as EnumerationExpression;
						result.Add(input is null ? "" : input.ToString());
						break;
					}
					case "pick_path": {
						Debug.Assert(argument.Count == 2);
						var type = argument[1];
						var selection = type switch {
							"file"      => await StorageHelper.PickFile(App.MainWindow),
							"directory" => await StorageHelper.PickDirectory(App.MainWindow),
							_           => throw new ArgumentOutOfRangeException(),
						};
						result.Add(selection is null ? "" : selection);
						break;
					}
					case "push_notification": {
						Debug.Assert(argument.Count == 3);
						var title = argument[1];
						var description = argument[2];
						App.Instance.PushNotification(
							new AppNotificationBuilder()
								.AddText(title)
								.AddText(description)
								.BuildNotification()
						);
						break;
					}
					default: {
						throw new Exception("invalid method");
					}
				}
			}));
			return result;
		}

	}

}
