#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Navigation;

namespace Helper.Module.ResourceForwarder {

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

		private MainPageController Controller { get; }

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

		public List<OptionGroupConfiguration> OptionConfiguration { get; set; } = default!;

		// ----------------

		public Boolean AutomaticClose { get; set; } = default!;

		public Boolean ParallelExecute { get; set; } = default!;

		public Boolean EnableFilter { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		public Boolean RemainInput { get; set; } = default!;

		public List<String> Input { get; set; } = new List<String>();

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.AutomaticClose = Setting.ResourceForwarderAutomaticClose;
			this.ParallelExecute = Setting.ResourceForwarderParallelExecute;
			this.EnableFilter = Setting.ResourceForwarderEnableFilter;
			this.EnableBatch = Setting.ResourceForwarderEnableBatch;
			this.RemainInput = Setting.ResourceForwarderRemainInput;
			try {
				this.OptionConfiguration = JsonHelper.Deserialize<List<OptionGroupConfiguration>>(StorageHelper.ReadFileTextSync(Setting.ResourceForwarderOptionConfiguration));
			} catch (Exception e) {
				App.MainWindow.Controller.PublishTip(InfoBarSeverity.Error, "Failed to load option configuration.", e.ToString());
				this.OptionConfiguration = new List<OptionGroupConfiguration>();
			}
			this.uOption_ItemsSource = this.OptionConfiguration.Select((group) => (new MainPageOptionGroupItemController() {
				Host = this,
				GroupModel = group,
				Children = group.Item.Select((item) => (new MainPageOptionItemItemController() {
					Host = this,
					ItemModel = item,
					TypeMatched = false,
					NameMatched = false,
				})).ToList(),
			})).ToList();
			return;
		}

		public async void ApplyOption (
			List<String>? optionView
		) {
			while (!this.View.IsLoaded) {
				await Task.Delay(40);
			}
			var optionAutomaticClose = default(Boolean?);
			var optionParallelExecute = default(Boolean?);
			var optionEnableFilter = default(Boolean?);
			var optionEnableBatch = default(Boolean?);
			var optionRemainInput = default(Boolean?);
			var optionInput = default(List<String>?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Ensure("-AutomaticClose")) {
					optionAutomaticClose = option.NextBoolean();
				}
				if (option.Ensure("-ParallelExecute")) {
					optionParallelExecute = option.NextBoolean();
				}
				if (option.Ensure("-EnableFilter")) {
					optionEnableFilter = option.NextBoolean();
				}
				if (option.Ensure("-EnableBatch")) {
					optionEnableBatch = option.NextBoolean();
				}
				if (option.Ensure("-RemainInput")) {
					optionRemainInput = option.NextBoolean();
				}
				if (option.Ensure("-Input")) {
					optionInput = option.NextStringList();
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
			if (optionParallelExecute is not null) {
				this.ParallelExecute = optionParallelExecute.Value;
				this.NotifyPropertyChanged(
					nameof(this.uParallelExecute_IsChecked)
				);
			}
			if (optionEnableFilter is not null) {
				this.EnableFilter = optionEnableFilter.Value;
				this.NotifyPropertyChanged(
					nameof(this.uEnableFilter_IsChecked)
				);
			}
			if (optionEnableBatch is not null) {
				this.EnableBatch = optionEnableBatch.Value;
				this.NotifyPropertyChanged(
					nameof(this.uEnableBatch_IsChecked)
				);
			}
			if (optionRemainInput is not null) {
				this.RemainInput = optionRemainInput.Value;
				this.NotifyPropertyChanged(
					nameof(this.uRemainInput_IsChecked)
				);
			}
			if (optionInput is not null) {
				await this.AppendInput(optionInput.Select(StorageHelper.Normalize).ToList());
			}
			if (optionEnableFilter is not null || optionEnableBatch is not null || optionInput is not null) {
				await this.RefreshInput();
				await this.RefreshFilter();
			}
			return;
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		// ----------------

		public async Task AppendInput (
			List<String> list
		) {
			foreach (var item in list) {
				if (this.Input.Contains(item)) {
					continue;
				}
				this.Input.Add(item);
				this.uInput_ItemsSource.Add(new MainPageInputItemController() { Host = this, Path = item });
			}
			return;
		}

		public async Task RemoveInput (
			List<String> list
		) {
			foreach (var item in list) {
				if (!this.Input.Contains(item)) {
					continue;
				}
				this.Input.Remove(item);
				this.uInput_ItemsSource.Remove(this.uInput_ItemsSource.First(value => (value.Path == item)));
			}
			return;
		}

		public async Task ClearInput (
		) {
			this.Input.Clear();
			this.uInput_ItemsSource.Clear();
			return;
		}

		public async Task RefreshInput (
		) {
			foreach (var group in this.uOption_ItemsSource) {
				foreach (var item in group.Children) {
					if (this.EnableBatch) {
						item.TypeMatched = item.ItemModel.Batchable && this.Input.All((input) => (StorageHelper.ExistDirectory(input)));
						item.NameMatched = true;
					} else {
						item.TypeMatched = this.Input.All((input) => (item.ItemModel.Filter.Type switch {
							OptionFilterFileObjectType.Any       => StorageHelper.Exist(input),
							OptionFilterFileObjectType.File      => StorageHelper.ExistFile(input),
							OptionFilterFileObjectType.Directory => StorageHelper.ExistDirectory(input),
							_                                    => throw new ArgumentOutOfRangeException(),
						}));
						item.NameMatched = this.Input.All((input) => (Regex.IsMatch(input, item.ItemModel.Filter.Name, RegexOptions.IgnoreCase)));
					}
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uRequireInputTip_Visibility),
				nameof(this.uInputCount_Text)
			);
			return;
		}

		public async Task RefreshFilter (
		) {
			foreach (var group in this.uOption_ItemsSource) {
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

		public async Task Forward (
			String? method,
			Object? argument
		) {
			var state = await ModdingWorker.ForwardHelper.Forward(this.Input.Select((value) => (ModdingWorker.ForwardHelper.MakeArgumentForCommand(value, method is null ? null : $"{method}{(!this.EnableBatch ? "" : ".batch")}", argument))).ToList(), this.ParallelExecute);
			if (!state) {
				return;
			}
			if (this.AutomaticClose) {
				await App.MainWindow.Controller.RemoveTabItem(this.View);
				return;
			}
			if (!this.RemainInput) {
				await this.ClearInput();
				await this.RefreshInput();
				await this.RefreshFilter();
			}
			return;
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
				await this.AppendInput(item.Select((value) => (StorageHelper.Normalize(value.Path))).ToList());
				await this.RefreshInput();
				await this.RefreshFilter();
			}
			return;
		}

		#endregion

		#region require input tip

		public Boolean uRequireInputTip_Visibility {
			get {
				return this.Input.Count == 0;
			}
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

		public Boolean uParallelExecute_IsChecked {
			get {
				return this.ParallelExecute;
			}
		}

		public async void uParallelExecute_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.ParallelExecute = senders.IsChecked!.Value;
			return;
		}

		// ----------------

		public Boolean uEnableFilter_IsChecked {
			get {
				return this.EnableFilter;
			}
		}

		public async void uEnableFilter_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.EnableFilter = senders.IsChecked!.Value;
			await this.RefreshFilter();
			return;
		}

		// ----------------

		public Boolean uEnableBatch_IsChecked {
			get {
				return this.EnableBatch;
			}
		}

		public async void uEnableBatch_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.EnableBatch = senders.IsChecked!.Value;
			await this.RefreshInput();
			await this.RefreshFilter();
			return;
		}

		// ----------------

		public Boolean uRemainInput_IsChecked {
			get {
				return this.RemainInput;
			}
		}

		public async void uRemainInput_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.RemainInput = senders.IsChecked!.Value;
			return;
		}

		#endregion

		#region input

		public String uInputCount_Text {
			get {
				return $"{this.Input.Count}";
			}
		}

		// ----------------

		public ObservableCollection<MainPageInputItemController> uInput_ItemsSource { get; } = new ObservableCollection<MainPageInputItemController>();

		public async void uInput_OnItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			if (sender is not ListView senders) { return; }
			if (args.ClickedItem is not MainPageInputItemController item) {
				return;
			}
			await this.RemoveInput(new List<String>() { item.Path });
			await this.RefreshInput();
			await this.RefreshFilter();
			return;
		}

		#endregion

		#region option

		public List<MainPageOptionGroupItemController> uOption_ItemsSource { get; set; } = default!;

		public async void uOption_OnItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			if (sender is not TreeView senders) { return; }
			if (args.InvokedItem is MainPageOptionGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (value.Content == groupItem)) ?? throw new Exception();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is MainPageOptionItemItemController itemItem) {
				await this.Forward(itemItem.ItemModel.Method, null);
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

		#endregion

		#region view

		public String uRoot_ToolTip {
			get {
				return this.Path;
			}
		}

		public String uIcon_Glyph {
			get {
				if (StorageHelper.ExistFile(this.Path)) {
					return FluentIconGlyph.Document;
				}
				if (StorageHelper.ExistDirectory(this.Path)) {
					return FluentIconGlyph.Folder;
				}
				return FluentIconGlyph.StatusCircleBlock;
			}
		}

		public String uName_Text {
			get {
				return StorageHelper.GetPathName(this.Path);
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
				return this.Host.Input.Count != 0 && (!this.Host.EnableFilter || this.Children.Any((value) => (value.TypeMatched && value.NameMatched)));
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

		public Boolean TypeMatched { get; set; } = default!;

		public Boolean NameMatched { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return this.Host.Input.Count != 0 && (!this.Host.EnableFilter || (this.TypeMatched && this.NameMatched));
			}
		}

		public Boolean uRoot_IsEnabled {
			get {
				return this.Host.Input.Count != 0 && (this.TypeMatched && (!this.Host.EnableFilter || (this.NameMatched)));
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
				return $"{this.ItemModel.Preset.Count((value) => (value is not null))}";
			}
		}

		public MenuFlyout uPreset_Flyout {
			get {
				var menu = new MenuFlyout() {
					Placement = FlyoutPlacementMode.BottomEdgeAlignedRight,
				};
				foreach (var preset in this.ItemModel.Preset) {
					var menuItem = default(MenuFlyoutItemBase);
					if (preset is null) {
						menuItem = new MenuFlyoutSeparator() {
						};
					} else {
						menuItem = new MenuFlyoutItem() {
							Text = preset.Name,
						};
						(menuItem as MenuFlyoutItem)!.Click += async (_, _) => {
							await this.Host.Forward(this.ItemModel.Method, preset.Argument);
							return;
						};
					}
					menu.Items.Add(menuItem);
				}
				return menu;
			}
		}

		#endregion

	}

}
