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

		public Boolean ParallelExecute { get; set; } = default!;

		public Boolean EnableFilter { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		public Boolean RemainInput { get; set; } = default!;

		public Boolean AutomaticClose { get; set; } = default!;

		public List<String> Input { get; set; } = [];

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.ParallelExecute = App.Setting.Data.ResourceForwarder.ParallelExecute;
			this.EnableFilter = App.Setting.Data.ResourceForwarder.EnableFilter;
			this.EnableBatch = App.Setting.Data.ResourceForwarder.EnableBatch;
			this.RemainInput = App.Setting.Data.ResourceForwarder.RemainInput;
			this.AutomaticClose = App.Setting.Data.ResourceForwarder.AutomaticClose;
			var optionConfiguration = new List<OptionGroupConfiguration>();
			try {
				optionConfiguration = JsonHelper.DeserializeText<List<OptionGroupConfiguration>>(StorageHelper.ReadFileTextSync(App.Setting.Data.ResourceForwarder.OptionConfiguration));
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to load option configuration.", e.ToString());
			}
			this.uOptionList_ItemsSource = optionConfiguration.Select((group) => (new MainPageOptionGroupItemController() {
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
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionParallelExecute = default(Boolean?);
			var optionEnableFilter = default(Boolean?);
			var optionEnableBatch = default(Boolean?);
			var optionRemainInput = default(Boolean?);
			var optionAutomaticClose = default(Boolean?);
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
				if (option.Check("-AutomaticClose")) {
					optionAutomaticClose = option.NextBoolean();
				}
				if (option.Check("-Input")) {
					optionInput = option.NextStringList();
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
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
			if (optionAutomaticClose is not null) {
				this.AutomaticClose = optionAutomaticClose.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticClose_Visibility)
				);
			}
			if (optionInput is not null) {
				await this.AppendInput(optionInput.Select(StorageHelper.Regularize).ToList());
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
				this.uInputList_ItemsSource.Add(new () { Host = this, Path = item });
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
				this.uInputList_ItemsSource.Remove(this.uInputList_ItemsSource.First((value) => (value.Path == item)));
			}
			return;
		}

		public async Task ClearInput (
		) {
			this.Input.Clear();
			this.uInputList_ItemsSource.Clear();
			return;
		}

		public async Task RefreshInput (
		) {
			foreach (var group in this.uOptionList_ItemsSource) {
				foreach (var item in group.Children) {
					if (this.EnableBatch) {
						item.TypeMatched = item.ItemModel.Batchable && this.Input.All((input) => (StorageHelper.ExistDirectory(input)));
						item.NameMatched = true;
					}
					else {
						item.TypeMatched = this.Input.All((input) => (item.ItemModel.Filter.Type switch {
							OptionFilterFileObjectType.Any       => StorageHelper.Exist(input),
							OptionFilterFileObjectType.File      => StorageHelper.ExistFile(input),
							OptionFilterFileObjectType.Directory => StorageHelper.ExistDirectory(input),
							_                                    => throw new (),
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

		public async Task Forward (
			String? method,
			Object? argument
		) {
			var state = await ModdingWorker.ForwardHelper.Forward(this.Input.Select((value) => (ModdingWorker.ForwardHelper.MakeArgumentForCommand(value, method is null ? null : $"{method}{(!this.EnableBatch ? "" : ".batch")}", argument))).ToList(), this.ParallelExecute);
			if (!state) {
				return;
			}
			if (this.AutomaticClose) {
				await App.MainWindow.RemoveTabItem(this.View);
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
				await this.AppendInput(data.Select((value) => (StorageHelper.Regularize(value.Path))).ToList());
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

		public async void uRemainInput_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.RemainInput = senders.IsChecked.AsNotNull();
			return;
		}

		// ----------------

		public Boolean uAutomaticClose_Visibility {
			get {
				return this.AutomaticClose;
			}
		}

		public async void uAutomaticClose_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			this.AutomaticClose = false;
			this.NotifyPropertyChanged(
				nameof(this.uAutomaticClose_Visibility)
			);
			return;
		}

		#endregion

		#region input

		public async void uInputItem_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.AsClass<SplitButton>();
			var flyout = new MenuFlyout() {
				Placement = FlyoutPlacementMode.BottomEdgeAlignedLeft,
			};
			flyout.Items.Add(new MenuFlyoutItem() {
				Icon = new FontIcon() { Glyph = FluentIconGlyph.Document },
				Text = "File",
			}.ApplySelf((it) => {
				it.Click += async (_, _) => {
					var item = await StorageHelper.PickOpenFile(WindowHelper.Find(this.View), $"{nameof(ResourceForwarder)}.Generic");
					if (item is not null) {
						await this.AppendInput([item]);
						await this.RefreshInput();
						await this.RefreshFilter();
					}
					return;
				};
			}));
			flyout.Items.Add(new MenuFlyoutItem() {
				Icon = new FontIcon() { Glyph = FluentIconGlyph.Folder },
				Text = "Directory",
			}.ApplySelf((it) => {
				it.Click += async (_, _) => {
					var item = await StorageHelper.PickOpenDirectory(WindowHelper.Find(this.View), $"{nameof(ResourceForwarder)}.Generic");
					if (item is not null) {
						await this.AppendInput([item]);
						await this.RefreshInput();
						await this.RefreshFilter();
					}
					return;
				};
			}));
			flyout.ShowAt(senders);
			return;
		}

		// ----------------

		public String uInputCount_Text {
			get {
				return this.Input.Count.ToString();
			}
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
			await this.RefreshInput();
			await this.RefreshFilter();
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
									await this.Host.Forward(this.ItemModel.Method, preset.Argument);
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
