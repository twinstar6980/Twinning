#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.ApplicationModel;
using Microsoft.UI.Xaml.Media;

namespace Twinning.AssistantPlus.View.ModdingWorker {

	public sealed partial class MainPage : Page, Home.IModulePage {

		#region life

		private MainPageController Controller { get; }

		// ----------------

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			this.Controller.InitializeView();
			ControlHelper.PostTask(this, async () => {
				await this.Controller.OpenView();
				await this.Controller.ApplyOption(this.Tag.As<List<String>>());
			}).SelfLet(ExceptionHelper.WrapTask);
			return;
		}

		#endregion

		#region module page

		public Home.IModulePageController ModulePageGetController (
		) {
			return this.Controller;
		}

		#endregion

		#region action

		public async Task<List<String>?> ExecuteCommand (
			List<String> additionalArgument
		) {
			this.Controller.AdditionalArgument = additionalArgument;
			this.Controller.NotifyPropertyChanged([
				nameof(this.Controller.uAdditionalArgumentCount_Text),
			]);
			return await this.Controller.LaunchSession();
		}

		#endregion

	}

	public partial class MainPageController : CustomController, Home.IModulePageController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public Boolean AutomaticScroll { get; set; } = default!;

		public List<String> AdditionalArgument { get; set; } = [];

		// ----------------

		public MainPageBridgeClient SessionClient { get; set; } = default!;

		public Boolean SessionRunning { get; set; } = false;

		public Boolean SubmissionState { get; set; } = false;

		#endregion

		#region life

		public void InitializeView (
		) {
			this.AutomaticScroll = App.Setting.Data.ModdingWorker.AutomaticScroll;
			this.SessionClient = new (this);
			return;
		}

		public async Task OpenView (
		) {
			return;
		}

		public async Task<Boolean> CloseView (
		) {
			if (this.SessionRunning) {
				await ControlHelper.ShowDialogAsAutomatic(this.View, "Session In Progress", null, null);
				return false;
			}
			return true;
		}

		public async Task EnterView (
		) {
			if (this.SubmissionState) {
				this.NotifyPropertyChanged([
					nameof(this.uSubmissionBar_Stamp),
				]);
			}
			return;
		}

		public async Task ExitView (
		) {
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			var optionAutomaticScroll = default(Boolean?);
			var optionImmediateLaunch = default(Boolean?);
			var optionAdditionalArgument = default(List<String>?);
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
				throw new ($"Too many option '{String.Join(' ', option.NextStringList())}'.");
			}
			if (optionAutomaticScroll != null) {
				this.AutomaticScroll = optionAutomaticScroll.AsNotNull();
				this.NotifyPropertyChanged([
					nameof(this.uAutomaticScroll_IsChecked),
				]);
			}
			if (optionAdditionalArgument != null) {
				this.AdditionalArgument = optionAdditionalArgument;
				this.NotifyPropertyChanged([
					nameof(this.uAdditionalArgumentCount_Text),
				]);
			}
			if (optionImmediateLaunch != null) {
				if (optionImmediateLaunch.AsNotNull()) {
					_ = this.LaunchSession().SelfLet(ExceptionHelper.WrapTask);
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

		#endregion

		#region action

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
				}).SelfLet(ExceptionHelper.WrapTask);
			}
			return;
		}

		public async Task<ValueExpression?> ReceiveSubmission (
			SubmissionType type,
			List<String>   option
		) {
			var history = App.Setting.State.ModdingWorkerSubmissionHistory[type.CastPrimitive<Size>()];
			this.SubmissionState = true;
			this.NotifyPropertyChanged([
				nameof(this.uProgress_ProgressPaused),
			]);
			this.uSubmissionBar_Type = type;
			this.uSubmissionBar_Option = option;
			this.uSubmissionBar_History = history;
			this.uSubmissionBar_Value.Value = null;
			this.NotifyPropertyChanged([
				nameof(this.uSubmissionBar_Type),
				nameof(this.uSubmissionBar_Option),
				nameof(this.uSubmissionBar_History),
				nameof(this.uSubmissionBar_Stamp),
			]);
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
			this.NotifyPropertyChanged([
				nameof(this.uSubmissionBar_Type),
				nameof(this.uSubmissionBar_Option),
				nameof(this.uSubmissionBar_History),
				nameof(this.uSubmissionBar_Stamp),
			]);
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
			AssertTest(!this.SessionRunning);
			var result = default(List<String>?);
			var exception = default(Exception?);
			this.SessionRunning = true;
			this.uMessageList_ItemsSource.Clear();
			this.NotifyPropertyChanged([
				nameof(this.uLaunch_Visibility),
				nameof(this.uSubmissionBar_Visibility),
				nameof(this.uProgress_ProgressIndeterminate),
			]);
			try {
				var kernel = StorageHelper.Temporary();
				var library = new Bridge.Library();
				try {
					StorageHelper.Copy(App.Setting.Data.ModdingWorker.Kernel, kernel);
					library.Open(kernel);
					result = await Task.Run(() => (Bridge.Launcher.Launch(this.SessionClient, library, App.Setting.Data.ModdingWorker.Script, [..App.Setting.Data.ModdingWorker.Argument, ..this.AdditionalArgument])));
				}
				catch (Exception e) {
					exception = e;
				}
				if (library.State()) {
					library.Close();
				}
				if (StorageHelper.Exist(kernel)) {
					StorageHelper.Remove(kernel);
				}
			}
			catch (Exception e) {
				exception = e;
			}
			if (exception == null) {
				await this.SendMessage(MessageType.Success, "SUCCEEDED", result.AsNotNull());
			}
			else {
				await this.SendMessage(MessageType.Error, "FAILED", [ExceptionHelper.GenerateMessage(exception)]);
			}
			this.SessionRunning = false;
			this.NotifyPropertyChanged([
				nameof(this.uLaunch_Visibility),
				nameof(this.uSubmissionBar_Visibility),
				nameof(this.uProgress_ProgressIndeterminate),
			]);
			return exception != null ? null : result.AsNotNull();
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
			this.NotifyPropertyChanged([
				nameof(this.uAutomaticScroll_IsChecked),
			]);
			return;
		}

		#endregion

		#region additional argument

		public async void uAdditionalArgument_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await ControlHelper.ShowDialogAsAutomatic(this.View, "Additional Argument", new TextBox() {
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				FontFamily = this.View.FindResource("ModdingWorker.MessageFont").As<FontFamily>(),
				TextWrapping = TextWrapping.Wrap,
				AcceptsReturn = true,
				Text = ConvertHelper.MakeStringListToStringWithLine(this.AdditionalArgument),
			}.SelfAlso((it) => {
				it.LostFocus += async (_, _) => {
					this.AdditionalArgument = ConvertHelper.ParseStringListFromStringWithLine(it.Text);
					this.NotifyPropertyChanged([
						nameof(this.uAdditionalArgumentCount_Text),
					]);
					it.Text = ConvertHelper.MakeStringListToStringWithLine(this.AdditionalArgument);
					return;
				};
			}), null);
			return;
		}

		// ----------------

		public String uAdditionalArgumentCount_Text {
			get {
				return $"{this.AdditionalArgument.Count}";
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
			_ = this.LaunchSession().SelfLet(ExceptionHelper.WrapTask);
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
			this.NotifyPropertyChanged([
				nameof(this.uProgress_ProgressPaused),
			]);
			return;
		}

		public UniqueStamp uSubmissionBar_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

	}

	public partial class MainPageMessageItemController : CustomController {

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

		#region constructor

		private MainPageController mController;

		private Boolean mRunning;

		// ----------------

		public MainPageBridgeClient (
			MainPageController controller
		) {
			this.mController = controller;
			this.mRunning = false;
			return;
		}

		#endregion

		#region life

		public override void Start (
		) {
			AssertTest(!this.mRunning);
			this.mRunning = true;
			return;
		}

		public override void Finish (
		) {
			AssertTest(this.mRunning);
			this.mRunning = false;
			return;
		}

		// ----------------

		public override List<String> Callback (
			List<String> argument
		) {
			AssertTest(this.mRunning);
			var result = new List<String>();
			AssertTest(argument.Count >= 1);
			try {
				this.mController.View.DispatcherQueue.EnqueueAsync(async () => {
					switch (argument[0]) {
						case "name": {
							AssertTest(argument.Count == 1);
							var detail = await this.CallbackName();
							var detailName = detail.Item1;
							result.Add(detailName);
							break;
						}
						case "version": {
							AssertTest(argument.Count == 1);
							var detail = await this.CallbackVersion();
							var detailVersion = detail.Item1;
							result.Add(detailVersion);
							break;
						}
						case "send_message": {
							AssertTest(argument.Count >= 3);
							var detailType = argument[1];
							var detailTitle = argument[2];
							var detailDescription = argument[3..];
							await this.CallbackSendMessage(detailType, detailTitle, detailDescription);
							break;
						}
						case "receive_submission": {
							AssertTest(argument.Count >= 2);
							var detailType = argument[1];
							var detailOption = argument[2..];
							var detail = await this.CallbackReceiveSubmission(detailType, detailOption);
							var detailValue = detail.Item1;
							result.Add(detailValue);
							break;
						}
						case "pick_storage_item": {
							AssertTest(argument.Count == 2);
							var detailType = argument[1];
							var detail = await this.CallbackPickStorageItem(detailType);
							var detailTarget = detail.Item1;
							result.Add(detailTarget);
							break;
						}
						case "push_system_notification": {
							AssertTest(argument.Count == 3);
							var detailType = argument[1];
							var detailDescription = argument[2];
							var detail = await this.CallbackPushSystemNotification(detailType, detailDescription);
							break;
						}
						default: throw new ("invalid method");
					}
				}).Wait();
			}
			catch (Exception e) {
				if (e is AggregateException && e.InnerException != null) {
					throw e.InnerException;
				}
				throw;
			}
			return result;
		}

		private async Task<ValueTuple<String>> CallbackName (
		) {
			var name = "assistant_plus";
			return new (name);
		}

		private async Task<ValueTuple<String>> CallbackVersion (
		) {
			var version = ApplicationInformation.Version;
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
			var typeValue = type switch {
				"load_file"      => "LoadFile",
				"load_directory" => "LoadDirectory",
				"save_file"      => "SaveFile",
				_                => throw new (),
			};
			target = await StorageHelper.Pick(typeValue, App.MainWindow, $"@{nameof(ModdingWorker)}.Generic", null) ?? "";
			return new (target);
		}

		private async Task<ValueTuple> CallbackPushSystemNotification (
			String title,
			String description
		) {
			NotificationHelper.Push(title, description);
			return new ();
		}

		#endregion

	}

}
