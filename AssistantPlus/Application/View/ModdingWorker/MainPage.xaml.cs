#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Navigation;
using Microsoft.Windows.AppNotifications.Builder;

namespace AssistantPlus.View.ModdingWorker {

	public sealed partial class MainPage : Page, Home.IModulePage {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		protected override void OnNavigatedTo (
			NavigationEventArgs args
		) {
			_ = this.ModulePageApplyOption(args.Parameter.As<List<String>>());
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		// ----------------

		public async Task<List<String>?> ExecuteCommand (
			List<String> additionalArgument
		) {
			this.Controller.AdditionalArgument = additionalArgument;
			this.Controller.NotifyPropertyChanged(
				nameof(this.Controller.uAdditionalArgumentCount_Text),
				nameof(this.Controller.uAdditionalArgumentContent_Text)
			);
			return await this.Controller.LaunchSession();
		}

		#endregion

		#region module page

		public Task ModulePageApplyOption (
			List<String> optionView
		) {
			return this.Controller.ApplyOption(optionView);
		}

		public Task<List<String>> ModulePageCollectOption (
		) {
			return this.Controller.CollectOption();
		}

		public Task<Boolean> ModulePageRequestClose (
		) {
			return this.Controller.RequestClose();
		}

		#endregion

	}

	public class MainPageController : CustomController, Home.IModulePageController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public Boolean AutomaticScroll { get; set; } = default!;

		public List<String> AdditionalArgument { get; set; } = [];

		// ----------------

		public MainPageBridgeClient SessionClient { get; set; } = default!;

		public Boolean SessionRunning { get; set; } = false;

		public Boolean SubmissionState { get; set; } = false;

		public List<List<ValueExpression>> SubmissionHistory { get; set; } = Enum.GetValues<SubmissionType>().Select((value) => (new List<ValueExpression>())).ToList();

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.AutomaticScroll = App.Setting.Data.ModdingWorker.AutomaticScroll;
			this.SessionClient = new (this);
			return;
		}

		public async Task ApplyOption (
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
				else {
					optionImmediateLaunch = App.Setting.Data.ModdingWorker.ImmediateLaunch;
				}
				if (option.Check("-AdditionalArgument")) {
					optionAdditionalArgument = option.NextStringList();
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionAutomaticScroll != null) {
				this.AutomaticScroll = optionAutomaticScroll.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticScroll_IsChecked)
				);
			}
			if (optionAdditionalArgument != null) {
				this.AdditionalArgument = optionAdditionalArgument;
				this.NotifyPropertyChanged(
					nameof(this.uAdditionalArgumentCount_Text),
					nameof(this.uAdditionalArgumentContent_Text)
				);
			}
			if (optionImmediateLaunch != null) {
				if (optionImmediateLaunch.AsNotNull()) {
					await this.LaunchSession();
				}
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-AutomaticScroll")) {
				option.NextBoolean(this.AutomaticScroll);
			}
			if (option.Check("-AdditionalArgument")) {
				option.NextStringList(this.AdditionalArgument);
			}
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			if (this.SessionRunning) {
				await ControlHelper.ShowDialogAsAutomatic(this.View, "Session In Progress", null, null);
				return false;
			}
			return true;
		}

		// ----------------

		public async Task SendMessage (
			MessageType  type,
			String       title,
			List<String> description
		) {
			this.uMessageList_ItemsSource.Add(new () {
				Host = this,
				Type = type,
				Title = title,
				Description = description,
			});
			if (this.AutomaticScroll) {
				_ = this.View.DispatcherQueue.EnqueueAsync(async () => {
					await Task.Delay(40);
					this.View.uMessageListScrollViewer.ChangeView(null, this.View.uMessageListScrollViewer.ScrollableHeight, null, true);
				});
			}
			return;
		}

		public async Task<ValueExpression?> ReceiveSubmission (
			SubmissionType type,
			List<String>   option
		) {
			var history = this.SubmissionHistory[(Size)type];
			this.SubmissionState = true;
			this.NotifyPropertyChanged(
				nameof(this.uProgress_ProgressPaused)
			);
			this.uSubmissionBar_Type = type;
			this.uSubmissionBar_Option = option;
			this.uSubmissionBar_History = history;
			this.uSubmissionBar_Value.Value = null;
			this.NotifyPropertyChanged(
				nameof(this.uSubmissionBar_Type),
				nameof(this.uSubmissionBar_Option),
				nameof(this.uSubmissionBar_History),
				nameof(this.uSubmissionBar_Stamp)
			);
			await Task.Delay(40);
			this.View.uSubmissionBar.Focus(FocusState.Programmatic);
			while (this.SubmissionState) {
				await Task.Delay(40);
			}
			var value = this.uSubmissionBar_Value.Value;
			this.uSubmissionBar_Type = null;
			this.uSubmissionBar_Option = [];
			this.uSubmissionBar_History = [];
			this.uSubmissionBar_Value.Value = null;
			this.NotifyPropertyChanged(
				nameof(this.uSubmissionBar_Type),
				nameof(this.uSubmissionBar_Option),
				nameof(this.uSubmissionBar_History),
				nameof(this.uSubmissionBar_Stamp)
			);
			if (value != null) {
				var valueString = ValueExpressionHelper.MakeString(value);
				history.RemoveAll((item) => (ValueExpressionHelper.MakeString(item) == valueString));
				history.Add(value);
			}
			return value;
		}

		// ----------------

		public async Task<List<String>?> LaunchSession (
		) {
			GF.AssertTest(!this.SessionRunning);
			var result = default(List<String>?);
			var exception = default(Exception?);
			this.SessionRunning = true;
			this.uMessageList_ItemsSource.Clear();
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uSubmissionBar_Visibility),
				nameof(this.uProgress_ProgressIndeterminate)
			);
			try {
				var kernel = StorageHelper.Temporary();
				var library = new Bridge.Library();
				try {
					StorageHelper.CopyFile(App.Setting.Data.ModdingWorker.Kernel, kernel);
					library.Open(kernel);
					result = await new Task<List<String>>(() => (Bridge.Launcher.Launch(this.SessionClient, library, App.Setting.Data.ModdingWorker.Script, [..App.Setting.Data.ModdingWorker.Argument, ..this.AdditionalArgument]))).SelfAlso((it) => { it.Start(); });
				}
				catch (Exception e) {
					exception = e;
				}
				if (library.State()) {
					library.Close();
				}
				if (StorageHelper.ExistFile(kernel)) {
					StorageHelper.RemoveFile(kernel);
				}
			}
			catch (Exception e) {
				exception = e;
			}
			if (exception == null) {
				await this.SendMessage(MessageType.Success, "SUCCEEDED", result.AsNotNull());
			}
			else {
				await this.SendMessage(MessageType.Error, "FAILED", [exception.ToString()]);
			}
			this.SessionRunning = false;
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uSubmissionBar_Visibility),
				nameof(this.uProgress_ProgressIndeterminate)
			);
			return exception == null ? result.AsNotNull() : null;
		}

		#endregion

		#region page

		public async void uPage_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Page>();
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Page>();
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
			var senders = sender.As<ToggleButton>();
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

		public async void uAdditionalArgumentContent_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.AdditionalArgument = ConvertHelper.ParseStringListFromStringWithLine(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uAdditionalArgumentCount_Text),
				nameof(this.uAdditionalArgumentContent_Text)
			);
			return;
		}

		public String uAdditionalArgumentContent_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.AdditionalArgument);
			}
		}

		#endregion

		#region launch

		public Boolean uLaunch_Visibility {
			get {
				return !this.SessionRunning;
			}
		}

		public async void uLaunch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			_ = this.LaunchSession();
			return;
		}

		#endregion

		#region progress

		public Boolean uProgress_ProgressIndeterminate {
			get {
				return this.SessionRunning;
			}
		}

		public Boolean uProgress_ProgressPaused {
			get {
				return this.SubmissionState;
			}
		}

		#endregion

		#region message

		public ObservableCollection<MainPageMessageItemController> uMessageList_ItemsSource { get; } = [];

		#endregion

		#region submission

		public Boolean uSubmissionBar_Visibility {
			get {
				return this.SessionRunning;
			}
		}

		public SubmissionType? uSubmissionBar_Type { get; set; } = null;

		public List<String> uSubmissionBar_Option { get; set; } = [];

		public List<ValueExpression> uSubmissionBar_History { get; set; } = [];

		public Wrapper<ValueExpression> uSubmissionBar_Value { get; } = new ();

		public async void uSubmissionBar_ValueSubmitted (
		) {
			this.SubmissionState = false;
			this.NotifyPropertyChanged(
				nameof(this.uProgress_ProgressPaused)
			);
			return;
		}

		public UniqueStamp uSubmissionBar_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

	}

	public class MainPageMessageItemController : CustomController {

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

	public class MainPageBridgeClient : Bridge.Client {

		#region structor

		private MainPageController mController;

		private Boolean mRunning;

		// ----------------

		public MainPageBridgeClient (
			MainPageController controller
		) {
			this.mController = controller;
			this.mRunning = false;
		}

		#endregion

		#region life

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

		public override List<String> Callback (
			List<String> argument
		) {
			GF.AssertTest(this.mRunning);
			var result = new List<String>();
			GF.AssertTest(argument.Count >= 1);
			Task.WaitAll(this.mController.View.DispatcherQueue.EnqueueAsync(async () => {
				switch (argument[0]) {
					case "name": {
						GF.AssertTest(argument.Count == 1);
						var detail = await this.CallbackName();
						var detailName = detail.Item1;
						result.Add(detailName);
						break;
					}
					case "version": {
						GF.AssertTest(argument.Count == 1);
						var detail = await this.CallbackVersion();
						var detailVersion = detail.Item1;
						result.Add(detailVersion);
						break;
					}
					case "send_message": {
						GF.AssertTest(argument.Count >= 3);
						var detailType = argument[1];
						var detailTitle = argument[2];
						var detailDescription = argument[3..];
						await this.CallbackSendMessage(detailType, detailTitle, detailDescription);
						break;
					}
					case "receive_submission": {
						GF.AssertTest(argument.Count >= 2);
						var detailType = argument[1];
						var detailOption = argument[2..];
						var detail = await this.CallbackReceiveSubmission(detailType, detailOption);
						var detailValue = detail.Item1;
						result.Add(detailValue);
						break;
					}
					case "pick_storage_item": {
						GF.AssertTest(argument.Count == 2);
						var detailType = argument[1];
						var detail = await this.CallbackPickStorageItem(detailType);
						var detailTarget = detail.Item1;
						result.Add(detailTarget);
						break;
					}
					case "push_system_notification": {
						GF.AssertTest(argument.Count == 3);
						var detailType = argument[1];
						var detailDescription = argument[2];
						var detail = await this.CallbackPushSystemNotification(detailType, detailDescription);
						break;
					}
					default: {
						throw new ("invalid method");
					}
				}
			}));
			return result;
		}

		private async Task<ValueTuple<String>> CallbackName (
		) {
			var name = "assistant.plus";
			return new (name);
		}

		private async Task<ValueTuple<String>> CallbackVersion (
		) {
			var version = Package.Current.Id.Version.Major.ToString();
			return new (version);
		}

		private async Task<ValueTuple> CallbackSendMessage (
			String       type,
			String       title,
			List<String> description
		) {
			var typeValue = type switch {
				"verbosity"   => MessageType.Verbosity,
				"information" => MessageType.Information,
				"warning"     => MessageType.Warning,
				"error"       => MessageType.Error,
				"success"     => MessageType.Success,
				"input"       => MessageType.Input,
				_             => throw new (),
			};
			await this.mController.SendMessage(typeValue, title, description);
			return new ();
		}

		private async Task<ValueTuple<String>> CallbackReceiveSubmission (
			String       type,
			List<String> option
		) {
			var value = "";
			var typeValue = type switch {
				"pause"       => SubmissionType.Pause,
				"boolean"     => SubmissionType.Boolean,
				"integer"     => SubmissionType.Integer,
				"floater"     => SubmissionType.Floater,
				"size"        => SubmissionType.Size,
				"string"      => SubmissionType.String,
				"path"        => SubmissionType.Path,
				"enumeration" => SubmissionType.Enumeration,
				_             => throw new (),
			};
			var valueData = await this.mController.ReceiveSubmission(typeValue, option);
			if (valueData != null) {
				value = ValueExpressionHelper.MakeString(valueData);
			}
			return new (value);
		}

		private async Task<ValueTuple<String>> CallbackPickStorageItem (
			String type
		) {
			var target = "";
			switch (type) {
				case "load_file": {
					target = await StorageHelper.PickLoadFile(WindowHelper.Find(this.mController.View), $"{nameof(ModdingWorker)}.Generic") ?? "";
					break;
				}
				case "load_directory": {
					target = await StorageHelper.PickLoadDirectory(WindowHelper.Find(this.mController.View), $"{nameof(ModdingWorker)}.Generic") ?? "";
					break;
				}
				case "save_file": {
					target = await StorageHelper.PickSaveFile(WindowHelper.Find(this.mController.View), $"{nameof(ModdingWorker)}.Generic", null, null) ?? "";
					break;
				}
				default: throw new ();
			}
			return new (target);
		}

		private async Task<ValueTuple> CallbackPushSystemNotification (
			String title,
			String description
		) {
			App.Instance.PushNotification(
				new AppNotificationBuilder()
					.AddText(title)
					.AddText(description)
					.BuildNotification()
			);
			return new ();
		}

		#endregion

	}

}
