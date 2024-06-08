#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Navigation;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus.View.ResourceForwarder {

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

		#endregion

		#region module page

		public async Task<List<String>> ModulePageCollectOption (
		) {
			return await this.Controller.CollectOption();
		}

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

		public Boolean ParallelExecute { get; set; } = default!;

		public Boolean EnableFilter { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		public Boolean RemainInput { get; set; } = default!;

		public List<Tuple<String, Boolean?>> Input { get; set; } = [];

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.ParallelExecute = App.Setting.Data.ResourceForwarder.ParallelExecute;
			this.EnableFilter = App.Setting.Data.ResourceForwarder.EnableFilter;
			this.EnableBatch = App.Setting.Data.ResourceForwarder.EnableBatch;
			this.RemainInput = App.Setting.Data.ResourceForwarder.RemainInput;
			var optionConfiguration = new List<OptionGroupConfiguration>();
			try {
				optionConfiguration = JsonHelper.DeserializeText<List<OptionGroupConfiguration>>(StorageHelper.ReadFileTextSync(App.Setting.Data.ResourceForwarder.OptionConfiguration));
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to load option configuration.", e.ToString());
			}
			this.uOptionList_ItemsSource = optionConfiguration.Select((group) => (new MainPageOptionGroupItemController() {
				Host = this,
				GroupModel = group,
				Children = group.Item.Select((item) => (new MainPageOptionItemItemController() {
					Host = this,
					ItemModel = item,
					SingleMatched = false,
					BatchMatched = false,
					NameMatched = false,
				})).ToList(),
			})).ToList();
			return;
		}

		public async void ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionParallelExecute = default(Boolean?);
			var optionEnableFilter = default(Boolean?);
			var optionEnableBatch = default(Boolean?);
			var optionRemainInput = default(Boolean?);
			var optionInput = default(List<String>?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Check("-ParallelExecute")) {
					optionParallelExecute = option.NextBoolean();
				}
				if (option.Check("-EnableFilter")) {
					optionEnableFilter = option.NextBoolean();
				}
				if (option.Check("-EnableBatch")) {
					optionEnableBatch = option.NextBoolean();
				}
				if (option.Check("-RemainInput")) {
					optionRemainInput = option.NextBoolean();
				}
				if (option.Check("-Input")) {
					optionInput = option.NextStringList();
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionParallelExecute is not null) {
				this.ParallelExecute = optionParallelExecute.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uParallelExecute_IsChecked)
				);
			}
			if (optionEnableFilter is not null) {
				this.EnableFilter = optionEnableFilter.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uEnableFilter_IsChecked)
				);
			}
			if (optionEnableBatch is not null) {
				this.EnableBatch = optionEnableBatch.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uEnableBatch_IsChecked)
				);
			}
			if (optionRemainInput is not null) {
				this.RemainInput = optionRemainInput.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uRemainInput_IsChecked)
				);
			}
			if (optionInput is not null) {
				await this.AppendInput(optionInput.Select(StorageHelper.Regularize).ToList());
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-ParallelExecute")) {
				option.NextBoolean(this.ParallelExecute);
			}
			if (option.Check("-EnableFilter")) {
				option.NextBoolean(this.EnableFilter);
			}
			if (option.Check("-EnableBatch")) {
				option.NextBoolean(this.EnableBatch);
			}
			if (option.Check("-RemainInput")) {
				option.NextBoolean(this.RemainInput);
			}
			if (option.Check("-Input")) {
				option.NextStringList(this.Input.Select((value) => (value.Item1)).ToList());
			}
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		// ----------------

		public async Task RefreshMatch (
		) {
			foreach (var group in this.uOptionList_ItemsSource) {
				foreach (var item in group.Children) {
					item.SingleMatched = false;
					item.BatchMatched = false;
					item.NameMatched = false;
					if (this.Input.Count != 0) {
						item.SingleMatched = true;
						item.BatchMatched = true;
						item.NameMatched = true;
						var nameRule = new Regex(item.ItemModel.Filter.Name, RegexOptions.IgnoreCase);
						foreach (var input in this.Input) {
							item.SingleMatched &= item.ItemModel.Filter.Type switch {
								OptionFilterFileObjectType.Any       => input.Item2 != null,
								OptionFilterFileObjectType.File      => input.Item2 == false,
								OptionFilterFileObjectType.Directory => input.Item2 == true,
								_                                    => throw new (),
							};
							item.BatchMatched &= item.ItemModel.Batchable && input.Item2 == true;
							item.NameMatched &= nameRule.IsMatch(input.Item1);
						}
					}
				}
			}
			return;
		}

		public async Task RefreshFilter (
		) {
			foreach (var group in this.uOptionList_ItemsSource) {
				foreach (var item in group.Children) {
					item.NotifyPropertyChanged(
						nameof(item.uRoot_Visibility),
						nameof(item.uRoot_IsEnabled)
					);
				}
				group.NotifyPropertyChanged(
					nameof(group.uRoot_Visibility)
				);
			}
			return;
		}

		// ----------------

		public async Task AppendInput (
			List<String> list
		) {
			foreach (var item in list) {
				if (this.Input.Any((value) => (value.Item1 == item))) {
					continue;
				}
				var itemType = null as Boolean?;
				if (StorageHelper.ExistFile(item)) {
					itemType = false;
				}
				if (StorageHelper.ExistDirectory(item)) {
					itemType = true;
				}
				this.Input.Add(new (item, itemType));
				this.uInputList_ItemsSource.Add(new () { Host = this, Path = item, Type = itemType });
			}
			this.NotifyPropertyChanged(
				nameof(this.uInputCount_Text)
			);
			await this.RefreshMatch();
			await this.RefreshFilter();
			return;
		}

		public async Task RemoveInput (
			List<String> list
		) {
			foreach (var item in list) {
				this.Input.RemoveAll((value) => (value.Item1 == item));
				this.uInputList_ItemsSource.Remove(this.uInputList_ItemsSource.First((value) => (value.Path == item)));
			}
			this.NotifyPropertyChanged(
				nameof(this.uInputCount_Text)
			);
			await this.RefreshMatch();
			await this.RefreshFilter();
			return;
		}

		public async Task ClearInput (
		) {
			this.Input.Clear();
			this.uInputList_ItemsSource.Clear();
			this.NotifyPropertyChanged(
				nameof(this.uInputCount_Text)
			);
			await this.RefreshMatch();
			await this.RefreshFilter();
			return;
		}

		// ----------------

		public async Task ExecuteForward (
			String?                     method,
			Dictionary<String, Object>? argument
		) {
			await App.Instance.AppendRecentLauncherItem(new () {
				Title = $"({this.Input.Count}) {String.Join(", ", this.Input.Select((value) => (StorageHelper.Name(value.Item1))))}",
				Type = ModuleType.ResourceForwarder,
				Option = await this.CollectOption(),
				Command = [],
			});
			var actualMethod = method is null ? null : $"{method}{(!this.EnableBatch ? "" : ".batch")}";
			var actualCommand = this.Input.Select((value) => (ModdingWorker.ForwardHelper.MakeArgumentForCommand(value.Item1, actualMethod, argument))).ToList();
			await ModdingWorker.ForwardHelper.ForwardMany(actualCommand, this.ParallelExecute);
			if (!this.RemainInput) {
				await this.ClearInput();
			}
			return;
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
				var item = await args.DataView.GetStorageItemsAsync();
				var input = item.Select((value) => (StorageHelper.GetLongPath(value.Path))).ToList();
				await this.AppendInput(input);
			}
			return;
		}

		#endregion

		#region setting

		public Boolean uParallelExecute_IsChecked {
			get {
				return this.ParallelExecute;
			}
		}

		public async void uParallelExecute_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.ParallelExecute = senders.IsChecked.AsNotNull();
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
			var senders = sender.AsClass<ToggleButton>();
			this.EnableFilter = senders.IsChecked.AsNotNull();
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
			var senders = sender.AsClass<ToggleButton>();
			this.EnableBatch = senders.IsChecked.AsNotNull();
			await this.RefreshFilter();
			return;
		}

		// ----------------

		public Boolean uRemainInput_IsChecked {
			get {
				return this.RemainInput;
			}
		}

		public async void uRemainInput_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.RemainInput = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region input

		public String uInputCount_Text {
			get {
				return this.Input.Count.ToString();
			}
		}

		// ----------------

		public async void uInputAdd_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			var item = senders.Tag.AsClass<String>() switch {
				"File"      => await StorageHelper.PickOpenFile(WindowHelper.Find(this.View), $"{nameof(ResourceForwarder)}.Input"),
				"Directory" => await StorageHelper.PickOpenDirectory(WindowHelper.Find(this.View), $"{nameof(ResourceForwarder)}.Input"),
				_           => throw new (),
			};
			if (item is not null) {
				await this.AppendInput([item]);
			}
			return;
		}

		// ----------------

		public ObservableCollection<MainPageInputItemController> uInputList_ItemsSource { get; } = [];

		public async void uInputList_ItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			var item = args.ClickedItem.AsClass<MainPageInputItemController>();
			await this.RemoveInput([item.Path]);
			return;
		}

		#endregion

		#region option

		public List<MainPageOptionGroupItemController> uOptionList_ItemsSource { get; set; } = default!;

		public async void uOptionList_ItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			var senders = sender.AsClass<TreeView>();
			if (args.InvokedItem is MainPageOptionGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (Object.ReferenceEquals(value.Content, groupItem))).AsNotNull();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is MainPageOptionItemItemController itemItem) {
				await this.ExecuteForward(itemItem.ItemModel.Method, null);
			}
			return;
		}

		#endregion

	}

	public class MainPageInputItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String Path { get; set; } = default!;

		public Boolean? Type { get; set; } = default!;

		#endregion

		#region view

		public String uRoot_ToolTip {
			get {
				return this.Path;
			}
		}

		public String uIcon_Glyph {
			get {
				return this.Type switch {
					null  => FluentIconGlyph.StatusCircleBlock,
					false => FluentIconGlyph.Document,
					true  => FluentIconGlyph.Folder,
				};
			}
		}

		public String uName_Text {
			get {
				return StorageHelper.Name(this.Path);
			}
		}

		#endregion

	}

	public class MainPageOptionItemTemplateSelector : DataTemplateSelector {

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

	public class MainPageOptionGroupItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public OptionGroupConfiguration GroupModel { get; set; } = default!;

		// ----------------

		public List<MainPageOptionItemItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return !this.Host.EnableFilter || this.Children.Any((value) => (value.NameMatched && (!this.Host.EnableBatch ? value.SingleMatched : value.BatchMatched)));
			}
		}

		public List<MainPageOptionItemItemController> uRoot_ItemsSource {
			get {
				return this.Children;
			}
		}

		public String uIcon_Glyph {
			get {
				return FluentIconGlyph.FindGlyph(this.GroupModel.Icon);
			}
		}

		public String uName_Text {
			get {
				return this.GroupModel.Name;
			}
		}

		#endregion

	}

	public class MainPageOptionItemItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public OptionConfiguration ItemModel { get; set; } = default!;

		// ----------------

		public Boolean SingleMatched { get; set; } = default!;

		public Boolean BatchMatched { get; set; } = default!;

		public Boolean NameMatched { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return (!this.Host.EnableFilter || (this.NameMatched && (!this.Host.EnableBatch ? this.SingleMatched : this.BatchMatched)));
			}
		}

		public Boolean uRoot_IsEnabled {
			get {
				return (!this.Host.EnableFilter || (this.NameMatched)) && (!this.Host.EnableBatch ? this.SingleMatched : this.BatchMatched);
			}
		}

		public String uIcon_Glyph {
			get {
				return FluentIconGlyph.FindGlyph(this.ItemModel.Icon);
			}
		}

		public String uName_Text {
			get {
				return this.ItemModel.Name;
			}
		}

		public String uPresetCount_Text {
			get {
				return this.ItemModel.Preset.Count(GF.NotNull).ToString();
			}
		}

		public MenuFlyout uPreset_Flyout {
			get {
				var menu = new MenuFlyout() {
					Placement = FlyoutPlacementMode.BottomEdgeAlignedRight,
				};
				foreach (var preset in this.ItemModel.Preset) {
					menu.Items.Add(
						preset is null
							? new MenuFlyoutSeparator() {
							}
							: new MenuFlyoutItem() {
								Text = preset.Name,
							}.ApplySelf((it) => {
								it.Click += async (_, _) => {
									await this.Host.ExecuteForward(this.ItemModel.Method, preset.Argument);
									return;
								};
							})
					);
				}
				return menu;
			}
		}

		#endregion

	}

}
