#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Navigation;

namespace Helper.Module.CommandForwarder {

	public sealed partial class QuickPage : Page {

		#region life

		public QuickPage (
		) {
			this.InitializeComponent();
			this.Controller = new QuickPageController() { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		protected override void OnNavigatedTo (
			NavigationEventArgs args
		) {
			if (args.Parameter is List<String> parameter) {
				this.Controller.AppendInput(parameter.Select(StorageHelper.Normalize).ToList());
				this.Controller.RefreshInput();
				this.Controller.RefreshFilter();
			}
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private QuickPageController Controller { get; }

		#endregion

	}

	public class QuickPageController : CustomController {

		#region data

		public QuickPage View { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.Configuration Configuration { get; set; } = default!;

		// ----------------

		public List<String> Input { get; set; } = new List<String>();

		public Boolean RemainInput { get; set; } = false;

		public Boolean ParallelExecute { get; set; } = false;

		public Boolean EnableBatch { get; set; } = false;

		public Boolean EnableFilter { get; set; } = true;

		public Boolean AutomaticClose { get; set; } = true;

		#endregion

		#region initialize

		public void Initialize (
		) {
			try {
				this.Configuration = JsonHelper.Deserialize<CommandConfigurationModel.Configuration>(StorageHelper.ReadFileTextSync(Setting.CommandForwarderCommandConfiguration));
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to load command configuration.", e.ToString());
				this.Configuration = new CommandConfigurationModel.Configuration() {
					Method = new List<CommandConfigurationModel.MethodGroupConfiguration>(),
					QuickOption = new List<CommandConfigurationModel.QuickOptionGroupConfiguration>(),
				};
			}
			this.uOption_ItemsSource = this.Configuration.QuickOption.Select((group) => (new QuickOptionGroupItemController() {
				Host = this,
				GroupModel = group,
				Children = group.Item.Select((item) => (new QuickOptionItemItemController() {
					Host = this,
					ItemModel = item,
					TypeMatched = false,
					NameMatched = false,
				})).ToList(),
			})).ToList();
			return;
		}

		// ----------------

		public async void AppendInput (
			List<String> list
		) {
			foreach (var item in list) {
				if (this.Input.Contains(item)) {
					continue;
				}
				this.Input.Add(item);
				this.uInput_ItemsSource.Add(new QuickInputItemController() { Host = this, Path = item });
			}
			return;
		}

		public async void RemoveInput (
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

		public async void ClearInput (
		) {
			this.Input.Clear();
			this.uInput_ItemsSource.Clear();
			return;
		}

		public async void RefreshInput (
		) {
			foreach (var group in this.uOption_ItemsSource) {
				foreach (var item in group.Children) {
					if (this.EnableBatch) {
						item.TypeMatched = item.ItemModel.Batchable && this.Input.All((input) => (StorageHelper.ExistDirectory(input)));
						item.NameMatched = true;
					} else {
						item.TypeMatched = this.Input.All((input) => (item.ItemModel.Filter.Type switch {
							CommandConfigurationModel.QuickOptionFilterFileObjectType.Any       => StorageHelper.Exist(input),
							CommandConfigurationModel.QuickOptionFilterFileObjectType.File      => StorageHelper.ExistFile(input),
							CommandConfigurationModel.QuickOptionFilterFileObjectType.Directory => StorageHelper.ExistDirectory(input),
							_                                                                   => throw new ArgumentOutOfRangeException(),
						}));
						item.NameMatched = this.Input.All((input) => (Regex.IsMatch(input, item.ItemModel.Filter.Name, RegexOptions.IgnoreCase)));
					}
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uRequireInputTip_Visibility),
				nameof(this.uInputCount_Content)
			);
			return;
		}

		public async void RefreshFilter (
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

		public void Forward (
			String? method,
			Object? argument
		) {
			try {
				var processArgument = new List<String>();
				foreach (var input in this.Input) {
					processArgument.Add(input);
					if (method is not null) {
						processArgument.Add("-method");
						processArgument.Add($"{method}{(!this.EnableBatch ? "" : ".batch")}");
					}
					if (argument is not null) {
						processArgument.Add("-argument");
						processArgument.Add(JsonHelper.Serialize(argument, false));
					}
					if (this.ParallelExecute) {
						ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, processArgument).Wait(0);
						processArgument.Clear();
					}
				}
				if (!this.ParallelExecute) {
					ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, processArgument).Wait(0);
				}
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to create process.", e.ToString());
				return;
			}
			if (this.AutomaticClose) {
				WindowHelper.GetForElement(this.View).Close();
				return;
			}
			if (!this.RemainInput) {
				this.ClearInput();
				this.RefreshInput();
				this.RefreshFilter();
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
				this.AppendInput(item.Select((value) => (StorageHelper.Normalize(value.Path))).ToList());
				this.RefreshInput();
				this.RefreshFilter();
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
			this.RefreshInput();
			this.RefreshFilter();
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
			this.RefreshFilter();
			return;
		}

		#endregion

		#region input

		public String uInputCount_Content {
			get {
				return $"{this.Input.Count}";
			}
		}

		// ----------------

		public ObservableCollection<QuickInputItemController> uInput_ItemsSource { get; } = new ObservableCollection<QuickInputItemController>();

		public async void uInput_OnItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			if (sender is not ListView senders) { return; }
			if (args.ClickedItem is not QuickInputItemController item) {
				return;
			}
			this.RemoveInput(new List<String>() { item.Path });
			this.RefreshInput();
			this.RefreshFilter();
			return;
		}

		#endregion

		#region option

		public List<QuickOptionGroupItemController> uOption_ItemsSource { get; set; } = default!;

		public async void uOption_OnItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			if (sender is not TreeView senders) { return; }
			if (args.InvokedItem is QuickOptionGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (value.Content == groupItem)) ?? throw new Exception();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is QuickOptionItemItemController itemItem) {
				this.Forward(itemItem.ItemModel.Method, null);
			}
			return;
		}

		#endregion

	}

	public class QuickInputItemController : CustomController {

		#region data

		public QuickPageController Host { get; init; } = default!;

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

	public class QuickOptionItemTemplateSelector : DataTemplateSelector {

		public DataTemplate? GroupTemplate { get; set; }

		public DataTemplate? ItemTemplate { get; set; }

		// ----------------

		protected override DataTemplate? SelectTemplateCore (
			Object item
		) {
			return item switch {
				QuickOptionGroupItemController => this.GroupTemplate,
				QuickOptionItemItemController  => this.ItemTemplate,
				_                              => null,
			};
		}

	}

	public class QuickOptionGroupItemController : CustomController {

		#region data

		public QuickPageController Host { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.QuickOptionGroupConfiguration GroupModel { get; set; } = default!;

		// ----------------

		public List<QuickOptionItemItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return this.Host.Input.Count != 0 && (!this.Host.EnableFilter || this.Children.Any((value) => (value.TypeMatched && value.NameMatched)));
			}
		}

		public List<QuickOptionItemItemController> uRoot_ItemsSource {
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

	public class QuickOptionItemItemController : CustomController {

		#region data

		public QuickPageController Host { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.QuickOptionConfiguration ItemModel { get; set; } = default!;

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
					var item = default(MenuFlyoutItemBase);
					if (preset is null) {
						item = new MenuFlyoutSeparator() {
						};
					} else {
						item = new MenuFlyoutItem() {
							Text = preset.Name,
						};
						(item as MenuFlyoutItem)!.Click += (sender, args) => {
							this.Host.Forward(this.ItemModel.Method, preset.Argument);
							return;
						};
					}
					menu.Items.Add(item);
				}
				return menu;
			}
		}

		#endregion

	}

}
