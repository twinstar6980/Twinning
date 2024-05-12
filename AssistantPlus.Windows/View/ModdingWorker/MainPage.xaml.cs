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
			this.Controller.ApplyOption(args.Parameter.AsClass<List<String>>());
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
				nameof(this.Controller.uAdditionalArgument_Text)
			);
			return await this.Controller.LaunchSession();
		}

		#endregion

		#region tab item page

		public async Task<Boolean> ModulePageRequestClose (
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

		public List<String> AdditionalArgument { get; set; } = [];

		// ----------------

		public MainPageBridgeClient SessionClient { get; set; } = default!;

		public Task<List<String>>? SessionTask { get; set; } = null;

		public Boolean SubmissionState { get; set; } = false;

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.AutomaticScroll = App.Setting.Data.ModdingWorker.AutomaticScroll;
			this.SessionClient = new (this);
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
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionAutomaticScroll is not null) {
				this.AutomaticScroll = optionAutomaticScroll.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticScroll_IsChecked)
				);
			}
			if (optionAdditionalArgument is not null) {
				this.AdditionalArgument = optionAdditionalArgument;
				this.NotifyPropertyChanged(
					nameof(this.uAdditionalArgumentCount_Text),
					nameof(this.uAdditionalArgument_Text)
				);
			}
			if (optionImmediateLaunch is not null) {
				if (optionImmediateLaunch.AsNotNull()) {
					await this.LaunchSession();
				}
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

		public async Task<List<String>?> LaunchSession (
		) {
			GF.AssertTest(this.SessionTask is null);
			var result = default(List<String>?);
			var exception = default(Exception?);
			var kernel = new String(App.Setting.Data.ModdingWorker.Kernel);
			var script = new String(App.Setting.Data.ModdingWorker.Script);
			var argument = new List<String>([..App.Setting.Data.ModdingWorker.Argument, ..this.AdditionalArgument]);
			var temporaryKernel = StorageHelper.Temporary();
			var library = new Bridge.Library();
			this.SessionTask = new (() => (Bridge.Launcher.Launch(this.SessionClient, library, script, argument)));
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uSubmissionBar_Visibility),
				nameof(this.uProgress_ProgressIndeterminate)
			);
			try {
				this.uMessageList_ItemsSource.Clear();
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
				await this.SendMessage(MessageType.Success, "SUCCEEDED", result.AsNotNull());
			}
			else {
				await this.SendMessage(MessageType.Error, "FAILED", [exception.ToString()]);
			}
			this.SessionTask = null;
			this.NotifyPropertyChanged(
				nameof(this.uLaunch_Visibility),
				nameof(this.uSubmissionBar_Visibility),
				nameof(this.uProgress_ProgressIndeterminate)
			);
			return result;
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

		public async Task<Object?> ReceiveSubmission (
			SubmissionType type,
			List<String>   option
		) {
			this.SubmissionState = true;
			this.NotifyPropertyChanged(
				nameof(this.uProgress_ProgressPaused)
			);
			this.uSubmissionBar_Type = type;
			this.uSubmissionBar_Option = option;
			this.uSubmissionBar_Value.Data = null;
			this.NotifyPropertyChanged(
				nameof(this.uSubmissionBar_Type),
				nameof(this.uSubmissionBar_Option),
				nameof(this.uSubmissionBar_Stamp)
			);
			await Task.Delay(40);
			this.View.uSubmissionBar.Focus(FocusState.Programmatic);
			while (this.SubmissionState) {
				await Task.Delay(40);
			}
			var value = this.uSubmissionBar_Value.Data;
			this.uSubmissionBar_Type = null;
			this.uSubmissionBar_Option = [];
			this.uSubmissionBar_Value.Data = null;
			this.NotifyPropertyChanged(
				nameof(this.uSubmissionBar_Type),
				nameof(this.uSubmissionBar_Option),
				nameof(this.uSubmissionBar_Stamp)
			);
			return value;
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

		public Boolean uProgress_ProgressIndeterminate {
			get {
				return this.SessionTask is not null;
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
				return this.SessionTask is not null;
			}
		}

		public SubmissionType? uSubmissionBar_Type { get; set; } = null;

		public List<String> uSubmissionBar_Option { get; set; } = [];

		public SubmissionValue uSubmissionBar_Value { get; } = new () { Data = null };

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

		private MainPageController Controller;

		private Boolean Running;

		// ----------------

		public MainPageBridgeClient (
			MainPageController controller
		) {
			this.Controller = controller;
			this.Running = false;
		}

		#endregion

		#region life

		public override void Start (
		) {
			GF.AssertTest(!this.Running);
			this.Running = true;
			return;
		}

		public override void Finish (
		) {
			GF.AssertTest(this.Running);
			this.Running = false;
			return;
		}

		// ----------------

		public override List<String> Callback (
			List<String> argument
		) {
			GF.AssertTest(this.Running);
			var result = new List<String>();
			GF.AssertTest(argument.Count >= 1);
			Task.WaitAll(this.Controller.View.DispatcherQueue.EnqueueAsync(async () => {
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
					case "pick_path": {
						GF.AssertTest(argument.Count == 2);
						var detailType = argument[1];
						var detail = await this.CallbackPickPath(detailType);
						var detailTarget = detail.Item1;
						result.Add(detailTarget);
						break;
					}
					case "push_notification": {
						GF.AssertTest(argument.Count == 3);
						var detailType = argument[1];
						var detailDescription = argument[2];
						var detail = await this.CallbackPushNotification(detailType, detailDescription);
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
			await this.Controller.SendMessage(typeValue, title, description);
			return new ();
		}

		private async Task<ValueTuple<String>> CallbackReceiveSubmission (
			String       type,
			List<String> option
		) {
			var value = default(String);
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
			var valueData = await this.Controller.ReceiveSubmission(typeValue, option);
			switch (typeValue) {
				case SubmissionType.Pause: {
					var valueSource = valueData.AsClassOrNull<Object>();
					value = "";
					break;
				}
				case SubmissionType.Boolean: {
					var valueSource = valueData.AsStructOrNull<Boolean>();
					value = valueSource is null ? "" : !valueSource.AsNotNull() ? "n" : "y";
					break;
				}
				case SubmissionType.Integer: {
					var valueSource = valueData.AsStructOrNull<Integer>();
					value = valueSource is null ? "" : valueSource.AsNotNull() == 0 ? "0" : (valueSource.AsNotNull() < 0 ? $"{valueSource}" : $"+{valueSource}");
					break;
				}
				case SubmissionType.Floater: {
					var valueSource = valueData.AsStructOrNull<Floater>();
					value = valueSource is null ? "" : valueSource.AsNotNull() == 0.0 ? "0.0" : (valueSource.AsNotNull() < 0.0 ? $"{valueSource}{(!Floater.IsInteger(valueSource.AsNotNull()) ? "" : ".0")}" : $"+{valueSource}{(!Floater.IsInteger(valueSource.Value) ? "" : ".0")}");
					break;
				}
				case SubmissionType.Size: {
					var valueSource = valueData.AsClassOrNull<SizeExpression>();
					value = valueSource is null ? "" : valueSource.ToString();
					break;
				}
				case SubmissionType.String: {
					var valueSource = valueData.AsClassOrNull<String>();
					value = valueSource is null ? "" : valueSource;
					break;
				}
				case SubmissionType.Path: {
					var valueSource = valueData.AsClassOrNull<PathExpression>();
					value = valueSource is null ? "" : valueSource.ToString();
					break;
				}
				case SubmissionType.Enumeration: {
					var valueSource = valueData.AsClassOrNull<EnumerationExpression>();
					value = valueSource is null ? "" : valueSource.ToString();
					break;
				}
				default: throw new ();
			}
			return new (value);
		}

		private async Task<ValueTuple<String>> CallbackPickPath (
			String type
		) {
			var target = default(String?);
			switch (type) {
				case "open_file": {
					target = await StorageHelper.PickOpenFile(WindowHelper.Find(this.Controller.View), $"{nameof(ModdingWorker)}.Generic");
					break;
				}
				case "open_directory": {
					target = await StorageHelper.PickOpenDirectory(WindowHelper.Find(this.Controller.View), $"{nameof(ModdingWorker)}.Generic");
					break;
				}
				case "save_file": {
					target = await StorageHelper.PickSaveFile(WindowHelper.Find(this.Controller.View), $"{nameof(ModdingWorker)}.Generic", null, null);
					break;
				}
				default: throw new ();
			}
			target ??= "";
			return new (target);
		}

		private async Task<ValueTuple> CallbackPushNotification (
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
