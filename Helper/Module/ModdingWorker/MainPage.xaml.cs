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
			this.Controller.ApplyOption(args.Parameter.AsClass<List<String>>());
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		// ----------------

		public async Task<String?> ExecuteCommand (
			List<String> additionalArgument
		) {
			this.Controller.AdditionalArgument = additionalArgument;
			this.Controller.NotifyPropertyChanged(
				nameof(this.Controller.uAdditionalArgumentCount_Text),
				nameof(this.Controller.uAdditionalArgument_Text)
			);
			return await this.Controller.LaunchSession();
		}

		#endregion

		#region tab item page

		public async Task<Boolean> TabItemPageRequestClose (
		) {
			return await this.Controller.RequestClose();
		}

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

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
			this.AutomaticScroll = Setting.Data.ModdingWorker.AutomaticScroll;
			this.ImmediateLaunch = Setting.Data.ModdingWorker.ImmediateLaunch;
			this.SessionHost = new HelperHost(this);
			return;
		}

		public async void ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionAutomaticScroll = default(Boolean?);
			var optionImmediateLaunch = default(Boolean?);
			var optionAdditionalArgument = default(List<String>?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Check("-AutomaticScroll")) {
					optionAutomaticScroll = option.NextBoolean();
				}
				if (option.Check("-ImmediateLaunch")) {
					optionImmediateLaunch = option.NextBoolean();
				}
				if (option.Check("-AdditionalArgument")) {
					optionAdditionalArgument = option.NextStringList();
				}
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
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
				await this.LaunchSession();
			}
			return;
		}

		public async Task<Boolean> RequestClose (
		) {
			if (this.SessionTask is not null) {
				await ControlHelper.ShowDialogSimple(this.View, "Session In Progress", null);
				return false;
			}
			return true;
		}

		// ----------------

		public async Task<String?> LaunchSession (
		) {
			GF.AssertTest(this.SessionTask is null);
			var result = default(String?);
			var exception = default(Exception?);
			var kernel = new String(Setting.Data.ModdingWorker.Kernel);
			var script = new String(Setting.Data.ModdingWorker.Script);
			var argument = new List<String>(new[] { Setting.Data.ModdingWorker.Argument, this.AdditionalArgument }.SelectMany(GF.ReturnSelf));
			var temporaryKernel = StorageHelper.Temporary();
			var library = new Bridge.Library();
			this.SessionTask = new Task<String>(() => (Bridge.Launcher.Launch(this.SessionHost, library, script, argument)));
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uInput_Visibility),
				nameof(this.uProgress_Visibility)
			);
			try {
				this.uOutputList_ItemsSource.Clear();
				StorageHelper.CopyFile(kernel, temporaryKernel);
				library.Open(temporaryKernel);
				this.SessionTask.Start();
				result = await this.SessionTask;
			}
			catch (Exception e) {
				exception = e;
			}
			if (library.State()) {
				library.Close();
			}
			if (StorageHelper.ExistFile(temporaryKernel)) {
				StorageHelper.RemoveFile(temporaryKernel);
			}
			if (exception is null) {
				await this.PublishOutput(MessageType.Success, "SUCCEEDED", new List<String>() { result.AsNotNull() });
			}
			else {
				await this.PublishOutput(MessageType.Error, "FAILED", new List<String>() { exception.ToString() });
			}
			this.SessionTask = null;
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uInput_Visibility),
				nameof(this.uProgress_Visibility)
			);
			return result;
		}

		// ----------------

		public async Task PublishOutput (
			MessageType  type,
			String       title,
			List<String> description
		) {
			this.uOutputList_ItemsSource.Add(new MainPageOutputItemController() {
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
			this.uInput_Value.Data = null;
			this.NotifyPropertyChanged(
				nameof(this.uInput_Type),
				nameof(this.uInput_Option),
				nameof(this.uInput_Stamp)
			);
			while (this.InputBusy) {
				await Task.Delay(40);
			}
			var input = this.uInput_Value.Data;
			this.uInput_Type = null;
			this.uInput_Option = null;
			this.uInput_Value.Data = null;
			this.NotifyPropertyChanged(
				nameof(this.uInput_Type),
				nameof(this.uInput_Option),
				nameof(this.uInput_Stamp)
			);
			return input;
		}

		#endregion

		#region page

		public async void uPage_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var data = await args.DataView.GetStorageItemsAsync();
				this.AdditionalArgument.AddRange(data.Select((value) => (StorageHelper.Regularize(value.Path))).ToList());
				this.NotifyPropertyChanged(
					nameof(this.uAdditionalArgumentCount_Text),
					nameof(this.uAdditionalArgument_Text)
				);
			}
			return;
		}

		#endregion

		#region setting

		public Boolean uAutomaticScroll_IsChecked {
			get {
				return this.AutomaticScroll;
			}
		}

		public async void uAutomaticScroll_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.AutomaticScroll = senders.IsChecked.AsNotNull();
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
				return ConvertHelper.StringListToTextWithCr(this.AdditionalArgument);
			}
		}

		public async void uAdditionalArgument_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.AdditionalArgument = ConvertHelper.StringListFromTextWithCr(senders.Text);
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

		public async void uLaunch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (this.SessionTask is null) {
				await this.LaunchSession();
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

		public ObservableCollection<MainPageOutputItemController> uOutputList_ItemsSource { get; } = new ObservableCollection<MainPageOutputItemController>();

		#endregion

		#region input

		public Boolean uInput_Visibility {
			get {
				return this.SessionTask is not null;
			}
		}

		public InputType? uInput_Type { get; set; } = null;

		public Object? uInput_Option { get; set; } = null;

		public InputValue uInput_Value { get; } = new InputValue() { Data = null };

		public async void uInput_ValueSubmitted (
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

	public class MainPageOutputItemController : CustomController {

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
			GF.AssertTest(!this.mRunning);
			this.mRunning = true;
			return;
		}

		public override void Finish (
		) {
			GF.AssertTest(this.mRunning);
			this.mRunning = false;
			return;
		}

		// ----------------

		public override List<String> Execute (
			List<String> argument
		) {
			GF.AssertTest(this.mRunning);
			var result = new List<String>();
			GF.AssertTest(argument.Count >= 1);
			var method = argument[0];
			Task.WaitAll(this.mController.View.DispatcherQueue.EnqueueAsync(async () => {
				switch (method) {
					case "version": {
						GF.AssertTest(argument.Count == 1);
						result.Add(Package.Current.Id.Version.Major.ToString());
						break;
					}
					case "host": {
						GF.AssertTest(argument.Count == 1);
						result.Add("helper");
						break;
					}
					case "system": {
						GF.AssertTest(argument.Count == 1);
						result.Add("windows");
						break;
					}
					case "output_message": {
						GF.AssertTest(argument.Count >= 3);
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
						GF.AssertTest(argument.Count == 1);
						await this.mController.RequestInput(InputType.Pause, null);
						break;
					}
					case "input_boolean": {
						GF.AssertTest(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Boolean, null) as Boolean?;
						result.Add(input is null ? "" : !input.Value ? "n" : "y");
						break;
					}
					case "input_integer": {
						GF.AssertTest(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Integer, null) as Integer?;
						result.Add(input is null ? "" : input.Value == 0 ? "0" : (input.Value < 0 ? $"{input}" : $"+{input}"));
						break;
					}
					case "input_floater": {
						GF.AssertTest(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Floater, null) as Floater?;
						result.Add(input is null ? "" : input.Value == 0.0 ? "0.0" : (input.Value < 0.0 ? $"{input}{(!Floater.IsInteger(input.Value) ? "" : ".0")}" : $"+{input}{(!Floater.IsInteger(input.Value) ? "" : ".0")}"));
						break;
					}
					case "input_size": {
						GF.AssertTest(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Size, null) as SizeExpression;
						result.Add(input is null ? "" : input.ToString());
						break;
					}
					case "input_string": {
						GF.AssertTest(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.String, null) as String;
						result.Add(input is null ? "" : input);
						break;
					}
					case "input_path": {
						GF.AssertTest(argument.Count == 1);
						var input = await this.mController.RequestInput(InputType.Path, null) as PathExpression;
						result.Add(input is null ? "" : input.ToString());
						break;
					}
					case "input_enumeration": {
						GF.AssertTest(argument.Count >= 1);
						var option = argument.GetRange(1, argument.Count - 1);
						var input = await this.mController.RequestInput(InputType.Enumeration, option) as EnumerationExpression;
						result.Add(input is null ? "" : input.ToString());
						break;
					}
					case "pick_path": {
						GF.AssertTest(argument.Count == 2);
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
						GF.AssertTest(argument.Count == 3);
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
