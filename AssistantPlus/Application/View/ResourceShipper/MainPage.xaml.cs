#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Navigation;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus.View.ResourceShipper {

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

		public List<OptionGroupConfiguration> OptionConfiguration { get; set; } = default!;

		// ----------------

		public Boolean ParallelForward { get; set; } = default!;

		public Boolean EnableFilter { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		public List<Tuple<String, Boolean?>> Resource { get; set; } = [];

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.OptionConfiguration = [];
			this.ParallelForward = App.Setting.Data.ResourceShipper.ParallelForward;
			this.EnableFilter = App.Setting.Data.ResourceShipper.EnableFilter;
			this.EnableBatch = App.Setting.Data.ResourceShipper.EnableBatch;
			try {
				this.OptionConfiguration = JsonHelper.DeserializeText<List<OptionGroupConfiguration>>(StorageHelper.ReadFileTextSync(App.Setting.Data.ResourceShipper.OptionConfiguration));
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to load option configuration.", e.ToString());
			}
			this.uOptionList_ItemsSource = this.OptionConfiguration.Select((group) => (new MainPageOptionGroupItemController() {
				Host = this,
				Configuration = group,
				Children = group.Item.Select((item) => (new MainPageOptionItemItemController() {
					Host = this,
					Configuration = item,
					SingleMatched = false,
					BatchMatched = false,
					NameMatched = false,
				})).ToList(),
			})).ToList();
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionParallelForward = default(Boolean?);
			var optionEnableFilter = default(Boolean?);
			var optionEnableBatch = default(Boolean?);
			var optionResource = default(List<Tuple<String>>?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Check("-ParallelForward")) {
					optionParallelForward = option.NextBoolean();
				}
				if (option.Check("-EnableFilter")) {
					optionEnableFilter = option.NextBoolean();
				}
				if (option.Check("-EnableBatch")) {
					optionEnableBatch = option.NextBoolean();
				}
				if (option.Check("-Resource")) {
					optionResource = [];
					while (!option.Done()) {
						optionResource.Add(new (
							option.NextString()
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
			if (optionEnableFilter != null) {
				this.EnableFilter = optionEnableFilter.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uEnableFilter_IsChecked)
				);
			}
			if (optionEnableBatch != null) {
				this.EnableBatch = optionEnableBatch.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uEnableBatch_IsChecked)
				);
			}
			if (optionResource != null) {
				await this.AppendResource(optionResource.Select((item) => (StorageHelper.Regularize(item.Item1))).ToList());
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-ParallelForward")) {
				option.NextBoolean(this.ParallelForward);
			}
			if (option.Check("-EnableFilter")) {
				option.NextBoolean(this.EnableFilter);
			}
			if (option.Check("-EnableBatch")) {
				option.NextBoolean(this.EnableBatch);
			}
			if (option.Check("-Resource")) {
				foreach (var item in this.Resource) {
					option.NextString(item.Item1);
				}
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
					if (this.Resource.Count != 0) {
						item.SingleMatched = true;
						item.BatchMatched = true;
						item.NameMatched = true;
						var nameRule = new Regex(item.Configuration.Filter.Name, RegexOptions.IgnoreCase);
						foreach (var resource in this.Resource) {
							item.SingleMatched &= item.Configuration.Filter.Type switch {
								FilterType.Any       => resource.Item2 != null,
								FilterType.File      => resource.Item2 == false,
								FilterType.Directory => resource.Item2 == true,
								_                    => throw new (),
							};
							item.BatchMatched &= item.Configuration.Batchable && resource.Item2 == true;
							item.NameMatched &= nameRule.IsMatch(resource.Item1);
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
			this.NotifyPropertyChanged(
				nameof(this.uResourceCount_Text)
			);
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
			this.NotifyPropertyChanged(
				nameof(this.uResourceCount_Text)
			);
			await this.RefreshMatch();
			await this.RefreshFilter();
			return;
		}

		public async Task ForwardResource (
			String?                     method,
			Dictionary<String, Object>? argument
		) {
			var actualMethod = method == null ? null : ModdingWorker.ForwardHelper.MakeMethodForBatchable(method, this.EnableBatch);
			var actualCommand = this.Resource.Select((value) => (ModdingWorker.ForwardHelper.MakeArgumentForCommand(value.Item1, actualMethod, argument))).ToList();
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
						it.LostFocus += (sender, args) => {
							var senders = sender.As<TextBox>();
							item = ConvertHelper.ParseStringListFromStringWithLine(senders.Text).Select(StorageHelper.Regularize).ToList();
							senders.Text = ConvertHelper.MakeStringListToStringWithLine(item);
							return;
						};
					}), new ("Cancel", "Continue", null));
					if (dialogResult == ContentDialogResult.Primary) {
						await this.AppendResource(item);
					}
					break;
				}
				case "AppendFile": {
					var item = await StorageHelper.PickLoadFile(WindowHelper.Find(this.View), $"{nameof(ResourceShipper)}.Resource");
					if (item != null) {
						await this.AppendResource([item]);
					}
					break;
				}
				case "AppendDirectory": {
					var item = await StorageHelper.PickLoadDirectory(WindowHelper.Find(this.View), $"{nameof(ResourceShipper)}.Resource");
					if (item != null) {
						await this.AppendResource([item]);
					}
					break;
				}
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

	public class MainPageResourceItemController : CustomController {

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

	public class MainPageOptionGroupItemController : CustomController {

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
				return !this.Host.EnableFilter || this.Children.Any((value) => (value.Host.EnableBatch ? value.BatchMatched : value.SingleMatched && value.NameMatched));
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

		public String uName_Text {
			get {
				return this.Configuration.Name;
			}
		}

		#endregion

	}

	public class MainPageOptionItemItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public OptionConfiguration Configuration { get; set; } = default!;

		// ----------------

		public Boolean SingleMatched { get; set; } = default!;

		public Boolean BatchMatched { get; set; } = default!;

		public Boolean NameMatched { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return !this.Host.EnableFilter || (this.Host.EnableBatch ? this.BatchMatched : this.SingleMatched && this.NameMatched);
			}
		}

		public Boolean uRoot_IsEnabled {
			get {
				return (this.Host.EnableBatch ? this.BatchMatched : this.SingleMatched && (!this.Host.EnableFilter || this.NameMatched));
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
