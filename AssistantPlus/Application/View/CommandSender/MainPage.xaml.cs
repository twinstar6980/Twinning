#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using FluentIconGlyph = Twinning.AssistantPlus.Control.FluentIconGlyph;

namespace Twinning.AssistantPlus.View.CommandSender {

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

	}

	public partial class MainPageController : CustomController, Home.IModulePageController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public List<MethodGroupConfiguration> MethodConfiguration { get; set; } = default!;

		// ----------------

		public Boolean ParallelForward { get; set; } = default!;

		public List<Tuple<MethodGroupConfiguration, MethodConfiguration, Wrapper<Boolean>, List<Wrapper<ValueExpression>>>> Command { get; set; } = [];

		#endregion

		#region life

		public void InitializeView (
		) {
			this.MethodConfiguration = [];
			this.ParallelForward = App.Setting.Data.CommandSender.ParallelForward;
			this.uMethodList_ItemsSource = [];
			return;
		}

		public async Task OpenView (
		) {
			this.MethodConfiguration = JsonHelper.DeserializeText<List<MethodGroupConfiguration>>(await StorageHelper.ReadFileText(App.Setting.Data.CommandSender.MethodConfiguration));
			this.uMethodList_ItemsSource = this.MethodConfiguration.Select((group) => (new MainPageMethodGroupItemController() {
				Host = this,
				Configuration = group,
				Children = group.Item.Select((item) => (new MainPageMethodItemItemController() {
					Host = this,
					Configuration = item,
				})).ToList(),
			})).ToList();
			this.NotifyPropertyChanged([
				nameof(this.uMethodList_ItemsSource),
			]);
			return;
		}

		public async Task<Boolean> CloseView (
		) {
			return true;
		}

		public async Task EnterView (
		) {
			return;
		}

		public async Task ExitView (
		) {
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			var optionParallelForward = default(Boolean?);
			var optionCommand = default(List<Tuple<String, Boolean, Dictionary<String, Object>>>?);
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
				throw new ($"Too many option '{String.Join(' ', option.NextStringList())}'.");
			}
			if (optionParallelForward != null) {
				this.ParallelForward = optionParallelForward.AsNotNull();
				this.NotifyPropertyChanged([
					nameof(this.uParallelForward_IsChecked),
				]);
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

		#endregion

		#region action

		public async Task AppendCommand (
			String                     method,
			Boolean                    batch,
			Dictionary<String, Object> argument
		) {
			var groupConfiguration = this.MethodConfiguration.First((value) => (method.StartsWith($"{value.Id}.")));
			var itemConfiguration = groupConfiguration.Item.First((value) => (value.Id == method));
			this.Command.Add(new (groupConfiguration, itemConfiguration, new (batch), ConfigurationHelper.ParseArgumentValueListJson(itemConfiguration.Argument, argument)));
			this.uCommandList_ItemsSource.Add(new () {
				Host = this,
				GroupConfiguration = this.Command.Last().Item1,
				ItemConfiguration = this.Command.Last().Item2,
				Batch = this.Command.Last().Item3,
				Argument = this.Command.Last().Item4,
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
				var method = ModdingWorker.ForwardHelper.MakeMethodMaybeBatch(item.Item2.Id, item.Item3.Value);
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
			this.NotifyPropertyChanged([
				nameof(this.uParallelForward_IsChecked),
			]);
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

	public partial class MainPageMethodGroupItemController : CustomController {

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

		public String uName_ToolTip {
			get {
				return this.Configuration.Name;
			}
		}

		public String uName_Text {
			get {
				return this.Configuration.Name;
			}
		}

		public Size uCount_Value {
			get {
				return this.Configuration.Item.Count;
			}
		}

		#endregion

	}

	public partial class MainPageMethodItemItemController : CustomController {

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

		public String uName_ToolTip {
			get {
				return this.Configuration.Name;
			}
		}

		public String uName_Text {
			get {
				return this.Configuration.Name;
			}
		}

		public String? uBatch_ToolTip {
			get {
				return this.Configuration.Batch == null ? null : "Batch";
			}
		}

		public Floater uBatch_Opacity {
			get {
				return ConvertHelper.MakeBooleanToFloaterOfOpacityVisibility(this.Configuration.Batch != null);
			}
		}

		#endregion

	}

	public partial class MainPageCommandItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodGroupConfiguration GroupConfiguration { get; set; } = default!;

		public MethodConfiguration ItemConfiguration { get; set; } = default!;

		public Wrapper<Boolean> Batch { get; set; } = default!;

		public List<Wrapper<ValueExpression>> Argument { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_ToolTip {
			get {
				return $"{this.GroupConfiguration.Name} - {this.ItemConfiguration.Name}";
			}
		}

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
			if (this.Argument.All((value) => value.Value == null) || await ControlHelper.ShowDialogForConfirm(this.Host.View, null, null)) {
				await this.Host.RemoveCommand(this.Host.uCommandList_ItemsSource.IndexOf(this));
			}
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

		public Boolean uBatch_IsEnabled {
			get {
				return this.ItemConfiguration.Batch != null;
			}
		}

		public Boolean uBatch_IsChecked {
			get {
				return this.Batch.Value;
			}
		}

		public async void uBatch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Batch.Value = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uBatch_IsChecked),
				nameof(this.uArgumentPanel_Batch),
				nameof(this.uArgumentPanel_Stamp),
			]);
			return;
		}

		public MenuFlyout uPreset_Flyout {
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
										this.Argument[argumentIndex].Value = ConfigurationHelper.ParseArgumentValueJson(this.ItemConfiguration.Argument[argumentIndex].Type, argument.Value);
									}
									this.NotifyPropertyChanged([
										nameof(this.uArgumentPanel_Stamp),
									]);
									return;
								};
							})
					);
				}
				return menu;
			}
		}

		public String uPresetCount_Text {
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
				return this.Argument;
			}
		}

		public List<Boolean> uArgumentPanel_Batch {
			get {
				return this.ItemConfiguration.Argument.Select((value) => (this.Batch.Value && (this.ItemConfiguration.Batch?.Contains(value.Id) ?? false))).ToList();
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
