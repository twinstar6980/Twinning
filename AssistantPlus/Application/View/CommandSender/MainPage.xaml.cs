#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Microsoft.UI.Xaml.Navigation;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus.View.CommandSender {

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

		public List<MethodGroupConfiguration> MethodConfiguration { get; set; } = default!;

		// ----------------

		public Boolean ParallelForward { get; set; } = default!;

		public List<Tuple<MethodGroupConfiguration, MethodConfiguration, Wrapper<Boolean>, List<Wrapper<ValueExpression>>>> Command { get; set; } = [];

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.MethodConfiguration = [];
			this.ParallelForward = App.Setting.Data.CommandSender.ParallelForward;
			try {
				this.MethodConfiguration = JsonHelper.DeserializeText<List<MethodGroupConfiguration>>(StorageHelper.ReadFileTextSync(App.Setting.Data.CommandSender.MethodConfiguration));
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to load method configuration.", e.ToString());
			}
			this.uMethodList_ItemsSource = this.MethodConfiguration.Select((group) => (new MainPageMethodGroupItemController() {
				Host = this,
				Configuration = group,
				Children = group.Item.Select((item) => (new MainPageMethodItemItemController() {
					Host = this,
					Configuration = item,
				})).ToList(),
			})).ToList();
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionParallelForward = default(Boolean?);
			var optionCommand = default(List<Tuple<String, Boolean, Dictionary<String, Object>>>?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Check("-ParallelForward")) {
					optionParallelForward = option.NextBoolean();
				}
				if (option.Check("-Command")) {
					optionCommand = [];
					while (!option.Done()) {
						optionCommand.Add(new (
							option.NextString(),
							option.NextBoolean(),
							option.NextString().SelfLet((it) => (JsonHelper.DeserializeText<Dictionary<String, Object>>(it)))
						));
					}
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionParallelForward != null) {
				this.ParallelForward = optionParallelForward.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uParallelForward_IsChecked)
				);
			}
			if (optionCommand != null) {
				foreach (var item in optionCommand) {
					await this.AppendCommand(item.Item1, item.Item2, item.Item3);
				}
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-ParallelForward")) {
				option.NextBoolean(this.ParallelForward);
			}
			if (option.Check("-Command")) {
				foreach (var item in this.Command) {
					option.NextString(item.Item2.Id);
					option.NextBoolean(item.Item3.Value);
					option.NextString(item.Item4.SelfLet((it) => (JsonHelper.SerializeText(ConfigurationHelper.MakeArgumentValueListJson(item.Item2.Argument, it), false))));
				}
			}
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		// ----------------

		public async Task AppendCommand (
			String                     methodId,
			Boolean                    enableBatch,
			Dictionary<String, Object> argumentValue
		) {
			var groupConfiguration = this.MethodConfiguration.First((value) => (methodId.StartsWith($"{value.Id}.")));
			var itemConfiguration = groupConfiguration.Item.First((value) => (value.Id == methodId));
			this.Command.Add(new (groupConfiguration, itemConfiguration, new (enableBatch), ConfigurationHelper.ParseArgumentValueListJson(itemConfiguration.Argument, argumentValue)));
			this.uCommandList_ItemsSource.Add(new () {
				Host = this,
				GroupConfiguration = this.Command.Last().Item1,
				ItemConfiguration = this.Command.Last().Item2,
				EnableBatch = this.Command.Last().Item3,
				ArgumentValue = this.Command.Last().Item4,
			});
			await Task.Delay(40);
			this.View.uCommandScrollViewer.ChangeView(null, this.View.uCommandScrollViewer.ExtentHeight, null, false);
			return;
		}

		public async Task RemoveCommand (
			Size index
		) {
			this.Command.RemoveAt(index);
			this.uCommandList_ItemsSource.RemoveAt(index);
			return;
		}

		public async Task ForwardCommand (
			List<Size> index
		) {
			var actualCommand = new List<List<String>>();
			foreach (var itemIndex in index) {
				var item = this.Command[itemIndex];
				var method = ModdingWorker.ForwardHelper.MakeMethodForBatchable(item.Item2.Id, item.Item3.Value);
				var argument = ConfigurationHelper.MakeArgumentValueListJson(item.Item2.Argument, item.Item4);
				actualCommand.Add(ModdingWorker.ForwardHelper.MakeArgumentForCommand(null, method, argument));
			}
			await ModdingWorker.ForwardHelper.ForwardMany(actualCommand, this.ParallelForward);
			return;
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

		public Boolean uParallelForward_IsChecked {
			get {
				return this.ParallelForward;
			}
		}

		public async void uParallelForward_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.ParallelForward = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region method

		public List<MainPageMethodGroupItemController> uMethodList_ItemsSource { get; set; } = default!;

		public async void uMethodList_ItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			var senders = sender.As<TreeView>();
			if (args.InvokedItem is MainPageMethodGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (Object.ReferenceEquals(value.Content, groupItem))).AsNotNull();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is MainPageMethodItemItemController itemItem) {
				await this.AppendCommand(itemItem.Configuration.Id, false, []);
			}
			return;
		}

		#endregion

		#region command

		public async void uCommandForward_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await this.ForwardCommand(this.Command.Select((value, index) => (index)).ToList());
			return;
		}

		// ----------------

		public ObservableCollection<MainPageCommandItemController> uCommandList_ItemsSource { get; } = [];

		#endregion

	}

	public partial class MainPageMethodItemTemplateSelector : DataTemplateSelector {

		public DataTemplate? GroupTemplate { get; set; }

		public DataTemplate? ItemTemplate { get; set; }

		// ----------------

		protected override DataTemplate? SelectTemplateCore (
			Object item
		) {
			return item switch {
				MainPageMethodGroupItemController => this.GroupTemplate,
				MainPageMethodItemItemController  => this.ItemTemplate,
				_                                 => null,
			};
		}

	}

	public class MainPageMethodGroupItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodGroupConfiguration Configuration { get; set; } = default!;

		// ----------------

		public List<MainPageMethodItemItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public List<MainPageMethodItemItemController> uRoot_ItemsSource {
			get {
				return this.Children;
			}
		}

		public String uIcon_Glyph {
			get {
				return FluentIconGlyph.FindGlyph(this.Configuration.Icon);
			}
		}

		public String uName_Text {
			get {
				return this.Configuration.Name;
			}
		}

		#endregion

	}

	public class MainPageMethodItemItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodConfiguration Configuration { get; set; } = default!;

		#endregion

		#region view

		public String uIcon_Glyph {
			get {
				return FluentIconGlyph.FindGlyph(this.Configuration.Icon);
			}
		}

		public String uName_Text {
			get {
				return this.Configuration.Name;
			}
		}

		public String? uBatchable_ToolTip {
			get {
				return this.Configuration.Batchable == null ? null : "Batchable";
			}
		}

		public Floater uBatchable_Opacity {
			get {
				return ConvertHelper.MakeBooleanToFloaterOfOpacityVisibility(this.Configuration.Batchable != null);
			}
		}

		#endregion

	}

	public class MainPageCommandItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodGroupConfiguration GroupConfiguration { get; set; } = default!;

		public MethodConfiguration ItemConfiguration { get; set; } = default!;

		public Wrapper<Boolean> EnableBatch { get; set; } = default!;

		public List<Wrapper<ValueExpression>> ArgumentValue { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				return $"{this.GroupConfiguration.Name} - {this.ItemConfiguration.Name}";
			}
		}

		// ----------------

		public async void uRemove_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await this.Host.RemoveCommand(this.Host.uCommandList_ItemsSource.IndexOf(this));
			return;
		}

		public async void uForward_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await this.Host.ForwardCommand([this.Host.uCommandList_ItemsSource.IndexOf(this)]);
			return;
		}

		public Boolean uEnableBatch_IsEnabled {
			get {
				return this.ItemConfiguration.Batchable != null;
			}
		}

		public Boolean uEnableBatch_IsChecked {
			get {
				return this.EnableBatch.Value;
			}
		}

		public async void uEnableBatch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.EnableBatch.Value = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged(
				nameof(this.uEnableBatch_IsChecked)
			);
			return;
		}

		public MenuFlyout uApplyPreset_Flyout {
			get {
				var menu = new MenuFlyout() {
					Placement = FlyoutPlacementMode.BottomEdgeAlignedRight,
				};
				foreach (var preset in this.ItemConfiguration.Preset) {
					menu.Items.Add(
						preset == null
							? new MenuFlyoutSeparator() {
							}
							: new MenuFlyoutItem() {
								Text = preset.Name,
							}.SelfAlso((it) => {
								it.Click += async (_, _) => {
									foreach (var argument in preset.Argument) {
										var argumentIndex = this.ItemConfiguration.Argument.FindIndex((value) => (value.Id == argument.Key));
										GF.AssertTest(argumentIndex != -1);
										this.ArgumentValue[argumentIndex].Value = ConfigurationHelper.ParseArgumentValueJson(this.ItemConfiguration.Argument[argumentIndex].Type, argument.Value);
									}
									this.NotifyPropertyChanged(
										nameof(this.uArgumentPanel_Stamp)
									);
									return;
								};
							})
					);
				}
				return menu;
			}
		}

		public String uApplyPresetCount_Text {
			get {
				return this.ItemConfiguration.Preset.Count((value) => (value != null)).ToString();
			}
		}

		// ----------------

		public List<ArgumentConfiguration> uArgumentPanel_Configuration {
			get {
				return this.ItemConfiguration.Argument;
			}
		}

		public List<Wrapper<ValueExpression>> uArgumentPanel_Value {
			get {
				return this.ArgumentValue;
			}
		}

		public UniqueStamp uArgumentPanel_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

	}

}
