#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using FluentIconGlyph = Twinning.AssistantPlus.Control.FluentIconGlyph;

namespace Twinning.AssistantPlus.View.CoreResourceShipper {

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

		public Setting Setting => App.Setting.Data.CoreResourceShipper;

		public Configuration Configuration { get; set; } = default!;

		// ----------------

		public Boolean ParallelForward { get; set; } = default!;

		public Boolean EnableFilter { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		public List<Tuple<String, Boolean?>> Resource { get; set; } = [];

		#endregion

		#region life

		public void InitializeView (
		) {
			this.Configuration = new () {
				Option = [],
			};
			this.ParallelForward = this.Setting.ParallelForward;
			this.EnableFilter = this.Setting.EnableFilter;
			this.EnableBatch = this.Setting.EnableBatch;
			this.uOptionList_ItemsSource = [];
			return;
		}

		public async Task OpenView (
		) {
			this.Configuration = await JsonHelper.DeserializeFile<Configuration>($"{App.Setting.Data.ModuleConfigurationDirectory}/{ModuleHelper.Query(ModuleType.CoreResourceShipper).Identifier}.json");
			this.uOptionList_ItemsSource = this.Configuration.Option.Select((group) => (new MainPageOptionGroupItemController() {
				Host = this,
				Configuration = group,
				Children = group.Item.Select((item) => (new MainPageOptionItemItemController() {
					Host = this,
					Configuration = item,
					SingleEnabled = false,
					SingleFiltered = false,
					BatchEnabled = false,
					BatchFiltered = false,
				})).ToList(),
			})).ToList();
			this.NotifyPropertyChanged([
				nameof(this.uOptionList_ItemsSource),
			]);
			await this.RefreshMatch();
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
			var optionEnableFilter = default(Boolean?);
			var optionEnableBatch = default(Boolean?);
			var optionResource = default(List<Tuple<String>>?);
			var option = new CommandLineReader(optionView);
			if (option.Check("-parallel_forward")) {
				optionParallelForward = option.NextBoolean();
			}
			if (option.Check("-enable_filter")) {
				optionEnableFilter = option.NextBoolean();
			}
			if (option.Check("-enable_batch")) {
				optionEnableBatch = option.NextBoolean();
			}
			if (option.Check("-resource")) {
				optionResource = [];
				while (!option.Done()) {
					optionResource.Add(new (
						option.NextString()
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
			if (optionEnableFilter != null) {
				this.EnableFilter = optionEnableFilter.AsNotNull();
				this.NotifyPropertyChanged([
					nameof(this.uEnableFilter_IsChecked),
				]);
			}
			if (optionEnableBatch != null) {
				this.EnableBatch = optionEnableBatch.AsNotNull();
				this.NotifyPropertyChanged([
					nameof(this.uEnableBatch_IsChecked),
				]);
			}
			if (optionResource != null) {
				await this.AppendResource(optionResource.Select((item) => (StorageHelper.Regularize(item.Item1))).ToList());
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-parallel_forward")) {
				option.NextBoolean(this.ParallelForward);
			}
			if (option.Check("-enable_filter")) {
				option.NextBoolean(this.EnableFilter);
			}
			if (option.Check("-enable_batch")) {
				option.NextBoolean(this.EnableBatch);
			}
			if (option.Check("-resource")) {
				foreach (var item in this.Resource) {
					option.NextString(item.Item1);
				}
			}
			return option.Done();
		}

		#endregion

		#region action

		public async Task RefreshMatch (
		) {
			foreach (var group in this.uOptionList_ItemsSource) {
				foreach (var item in group.Children) {
					item.SingleEnabled = true;
					item.BatchEnabled = item.Configuration.Batch;
					if (item.Configuration.Filter != null) {
						foreach (var resource in this.Resource) {
							item.SingleEnabled &= item.Configuration.Filter.Type switch {
								FilterType.Any       => resource.Item2 != null,
								FilterType.File      => resource.Item2 == false,
								FilterType.Directory => resource.Item2 == true,
								_                    => throw new UnreachableException(),
							};
							item.BatchEnabled &= resource.Item2 == true;
						}
					}
					item.SingleFiltered = false;
					item.BatchFiltered = false;
					if (item.Configuration.Filter == null && this.Resource.Count == 0) {
						item.SingleFiltered |= true;
						item.BatchFiltered |= true;
					}
					if (item.Configuration.Filter != null && this.Resource.Count != 0) {
						var nameRule = new Regex(item.Configuration.Filter.Name, RegexOptions.IgnoreCase);
						item.SingleFiltered |= this.Resource.All((resource) => nameRule.IsMatch(resource.Item1));
						item.BatchFiltered |= true;
					}
				}
			}
			return;
		}

		public async Task RefreshFilter (
		) {
			foreach (var group in this.uOptionList_ItemsSource) {
				foreach (var item in group.Children) {
					item.NotifyPropertyChanged([
						nameof(item.uRoot_Visibility),
						nameof(item.uRoot_IsEnabled),
					]);
				}
				group.NotifyPropertyChanged([
					nameof(group.uRoot_Visibility),
					nameof(group.uCount_Value),
				]);
			}
			return;
		}

		// ----------------

		public async Task AppendResource (
			List<String> list
		) {
			foreach (var item in list) {
				if (this.Resource.Any((value) => (value.Item1 == item))) {
					continue;
				}
				var itemType = null as Boolean?;
				if (StorageHelper.ExistFile(item)) {
					itemType = false;
				}
				if (StorageHelper.ExistDirectory(item)) {
					itemType = true;
				}
				this.Resource.Add(new (item, itemType));
				this.uResourceList_ItemsSource.Add(new () { Host = this, Path = item, Type = itemType });
			}
			this.NotifyPropertyChanged([
				nameof(this.uResourceCount_Text),
			]);
			await this.RefreshMatch();
			await this.RefreshFilter();
			return;
		}

		public async Task RemoveResource (
			List<String> list
		) {
			foreach (var item in list) {
				this.Resource.RemoveAll((value) => (value.Item1 == item));
				this.uResourceList_ItemsSource.Remove(this.uResourceList_ItemsSource.First((value) => (value.Path == item)));
			}
			this.NotifyPropertyChanged([
				nameof(this.uResourceCount_Text),
			]);
			await this.RefreshMatch();
			await this.RefreshFilter();
			return;
		}

		public async Task ForwardResource (
			String?                     method,
			Dictionary<String, Object>? argument
		) {
			var actualInput = this.Resource.Count != 0 ? this.Resource.Select((value) => value.Item1).Cast<String?>() : [null];
			var actualMethod = method == null ? null : CoreTaskWorker.ForwardHelper.MakeMethodMaybeBatch(method, this.EnableBatch);
			var actualCommand = actualInput.Select((value) => (CoreTaskWorker.ForwardHelper.MakeArgumentForCommand(value, actualMethod, argument))).ToList();
			await CoreTaskWorker.ForwardHelper.ForwardMany(actualCommand, this.ParallelForward);
			return;
		}

		#endregion

		#region page

		public async void uPage_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = DataPackageOperation.Link;
			}
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				var resource = item.Select((value) => (StorageHelper.GetLongPath(value.Path))).ToList();
				await this.AppendResource(resource);
			}
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

		// ----------------

		public Boolean uEnableFilter_IsChecked {
			get {
				return this.EnableFilter;
			}
		}

		public async void uEnableFilter_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.EnableFilter = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uEnableFilter_IsChecked),
			]);
			await this.RefreshFilter();
			return;
		}

		// ----------------

		public Boolean uEnableBatch_IsChecked {
			get {
				return this.EnableBatch;
			}
		}

		public async void uEnableBatch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.EnableBatch = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uEnableBatch_IsChecked),
			]);
			await this.RefreshFilter();
			return;
		}

		#endregion

		#region resource

		public String uResourceCount_Text {
			get {
				return this.Resource.Count.ToString();
			}
		}

		// ----------------

		public async void uResourceAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			switch (senders.Tag.As<String>()) {
				case "RemoveAll": {
					if (await ControlHelper.ShowDialogForConfirm(this.View, null, null)) {
						await this.RemoveResource(this.Resource.Select((value) => (value.Item1)).ToList());
					}
					break;
				}
				case "AppendNew": {
					var item = new List<String>();
					var dialogResult = await ControlHelper.ShowDialogAsAutomatic(this.View, "Append New", new TextBox() {
						HorizontalAlignment = HorizontalAlignment.Stretch,
						VerticalAlignment = VerticalAlignment.Stretch,
						TextWrapping = TextWrapping.Wrap,
						AcceptsReturn = true,
						Text = "",
					}.SelfAlso((it) => {
						it.LostFocus += async (_, _) => {
							item = ConvertHelper.ParseStringListFromStringWithLine(it.Text).Select(StorageHelper.Regularize).ToList();
							it.Text = ConvertHelper.MakeStringListToStringWithLine(item);
							return;
						};
					}), new ("Cancel", "Continue", null));
					if (dialogResult == ContentDialogResult.Primary) {
						await this.AppendResource(item);
					}
					break;
				}
				case "PickFile": {
					var item = await StorageHelper.PickLoadFile(App.MainWindow, $"@{nameof(CoreResourceShipper)}.Resource");
					if (item != null) {
						await this.AppendResource([item]);
					}
					break;
				}
				case "PickDirectory": {
					var item = await StorageHelper.PickLoadDirectory(App.MainWindow, $"@{nameof(CoreResourceShipper)}.Resource");
					if (item != null) {
						await this.AppendResource([item]);
					}
					break;
				}
				default: throw new UnreachableException();
			}
			return;
		}

		// ----------------

		public ObservableCollection<MainPageResourceItemController> uResourceList_ItemsSource { get; } = [];

		public async void uResourceList_ItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			var senders = sender.As<ListView>();
			var item = args.ClickedItem.As<MainPageResourceItemController>();
			await this.RemoveResource([item.Path]);
			return;
		}

		#endregion

		#region option

		public List<MainPageOptionGroupItemController> uOptionList_ItemsSource { get; set; } = default!;

		public async void uOptionList_ItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			var senders = sender.As<TreeView>();
			if (args.InvokedItem is MainPageOptionGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (Object.ReferenceEquals(value.Content, groupItem))).AsNotNull();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is MainPageOptionItemItemController itemItem) {
				await this.ForwardResource(itemItem.Configuration.Method, null);
			}
			return;
		}

		#endregion

	}

	public partial class MainPageResourceItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String Path { get; set; } = default!;

		public Boolean? Type { get; set; } = default!;

		#endregion

		#region view

		public String uIcon_Glyph {
			get {
				return this.Type switch {
					null  => FluentIconGlyph.StatusCircleBlock,
					false => FluentIconGlyph.Document,
					true  => FluentIconGlyph.Folder,
				};
			}
		}

		public String uName_ToolTip {
			get {
				return this.Path;
			}
		}

		public String uName_Text {
			get {
				return StorageHelper.Name(this.Path);
			}
		}

		#endregion

	}

	public partial class MainPageOptionItemTemplateSelector : DataTemplateSelector {

		public DataTemplate? GroupTemplate { get; set; }

		public DataTemplate? ItemTemplate { get; set; }

		// ----------------

		protected override DataTemplate? SelectTemplateCore (
			Object item
		) {
			return item switch {
				MainPageOptionGroupItemController => this.GroupTemplate,
				MainPageOptionItemItemController  => this.ItemTemplate,
				_                                 => null,
			};
		}

	}

	public partial class MainPageOptionGroupItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public OptionGroupConfiguration Configuration { get; set; } = default!;

		// ----------------

		public List<MainPageOptionItemItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return this.Children.Any((item) => !item.Host.EnableFilter || (!item.Host.EnableBatch ? item.SingleEnabled && item.SingleFiltered : item.BatchEnabled && item.BatchFiltered));
			}
		}

		public List<MainPageOptionItemItemController> uRoot_ItemsSource {
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
				return this.Children.Count((item) => !item.Host.EnableFilter || (!item.Host.EnableBatch ? item.SingleEnabled && item.SingleFiltered : item.BatchEnabled && item.BatchFiltered));
			}
		}

		#endregion

	}

	public partial class MainPageOptionItemItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public OptionConfiguration Configuration { get; set; } = default!;

		// ----------------

		public Boolean SingleEnabled { get; set; } = default!;

		public Boolean SingleFiltered { get; set; } = default!;

		public Boolean BatchEnabled { get; set; } = default!;

		public Boolean BatchFiltered { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return !this.Host.EnableFilter || (!this.Host.EnableBatch ? this.SingleEnabled && this.SingleFiltered : this.BatchEnabled && this.BatchFiltered);
			}
		}

		public Boolean uRoot_IsEnabled {
			get {
				return !this.Host.EnableBatch ? this.SingleEnabled : this.BatchEnabled;
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

		public MenuFlyout uPreset_Flyout {
			get {
				var menu = new MenuFlyout() {
					Placement = FlyoutPlacementMode.BottomEdgeAlignedRight,
				};
				foreach (var preset in this.Configuration.Preset) {
					menu.Items.Add(
						preset == null
							? new MenuFlyoutSeparator() {
							}
							: new MenuFlyoutItem() {
								Text = preset.Name,
							}.SelfAlso((it) => {
								it.Click += async (_, _) => {
									await this.Host.ForwardResource(this.Configuration.Method, preset.Argument);
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
				return this.Configuration.Preset.Count((value) => (value != null)).ToString();
			}
		}

		#endregion

	}

}
