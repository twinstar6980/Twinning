#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;
using Microsoft.UI.Xaml.Navigation;

namespace Helper.Module.CommandSender {

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

		public List<MethodGroupConfiguration> MethodConfiguration { get; set; } = default!;

		#endregion

		#region initialize

		public void Initialize (
		) {
			try {
				this.MethodConfiguration = JsonHelper.Deserialize<List<MethodGroupConfiguration>>(StorageHelper.ReadFileTextSync(Setting.Data.CommandSender.MethodConfiguration));
			} catch (Exception e) {
				App.MainWindow.Controller.PublishTip(InfoBarSeverity.Error, "Failed to load method configuration.", e.ToString());
				this.MethodConfiguration = new List<MethodGroupConfiguration>();
			}
			this.uAvailableMethod_ItemsSource = this.MethodConfiguration.Select((group) => (new MainPageAvailableMethodGroupItemController() {
				Host = this,
				GroupModel = group,
				Children = group.Item.Select((item) => (new MainPageAvailableMethodItemItemController() {
					Host = this,
					GroupModel = group,
					ItemModel = item,
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
			try {
				var option = new CommandLineReader(optionView);
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			} catch (Exception e) {
				App.MainWindow.Controller.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			return;
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		#endregion

		#region page

		public async void uPage_OnDragOver (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			return;
		}

		public async void uPage_OnDrop (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			args.Handled = true;
			return;
		}

		#endregion

		#region available method

		public List<MainPageAvailableMethodGroupItemController> uAvailableMethod_ItemsSource { get; set; } = default!;

		public async void uAvailableMethod_OnItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			if (sender is not TreeView senders) { return; }
			if (args.InvokedItem is MainPageAvailableMethodGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (value.Content == groupItem)) ?? throw new Exception();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is MainPageAvailableMethodItemItemController itemItem) {
				this.uSelectedMethod_ItemsSource.Add(new MainPageSelectedMethodItemController() {
					Host = this,
					GroupModel = itemItem.GroupModel,
					ItemModel = itemItem.ItemModel,
					Argument = ConfigurationHelper.MakeArgumentValueDefault(itemItem.ItemModel.Argument),
					EnableBatch = false,
				});
				await Task.Delay(40);
				this.View.uSelectedMethodScrollViewer.ChangeView(null, this.View.uSelectedMethodScrollViewer.ExtentHeight, null, false);
			}
			return;
		}

		#endregion

		#region selected method

		public ObservableCollection<MainPageSelectedMethodItemController> uSelectedMethod_ItemsSource { get; } = new ObservableCollection<MainPageSelectedMethodItemController>();

		// ----------------

		public async void uForwardWithSelectedMethodByParallel_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await ModdingWorker.ForwardHelper.Forward(this.uSelectedMethod_ItemsSource.Select((value) => (value.GenerateCommand())).ToList(), true);
			return;
		}

		public async void uForwardWithSelectedMethodBySequence_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await ModdingWorker.ForwardHelper.Forward(this.uSelectedMethod_ItemsSource.Select((value) => (value.GenerateCommand())).ToList(), false);
			return;
		}

		public async void uForwardWithoutSelectedMethod_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await ModdingWorker.ForwardHelper.Forward(new List<String>());
			return;
		}

		#endregion

	}

	public class MainPageAvailableMethodItemTemplateSelector : DataTemplateSelector {

		public DataTemplate? GroupTemplate { get; set; }

		public DataTemplate? ItemTemplate { get; set; }

		// ----------------

		protected override DataTemplate? SelectTemplateCore (
			Object item
		) {
			return item switch {
				MainPageAvailableMethodGroupItemController => this.GroupTemplate,
				MainPageAvailableMethodItemItemController  => this.ItemTemplate,
				_                                          => null,
			};
		}

	}

	public class MainPageAvailableMethodGroupItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodGroupConfiguration GroupModel { get; set; } = default!;

		// ----------------

		public List<MainPageAvailableMethodItemItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public List<MainPageAvailableMethodItemItemController> uRoot_ItemsSource {
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

	public class MainPageAvailableMethodItemItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodGroupConfiguration GroupModel { get; set; } = default!;

		public MethodConfiguration ItemModel { get; set; } = default!;

		#endregion

		#region view

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

		public Floater uBatch_Opacity {
			get {
				return ConvertHelper.BooleanToFloaterOfOpacityVisibility(this.ItemModel.Batchable is not null);
			}
		}

		#endregion

	}

	public class MainPageSelectedMethodItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodGroupConfiguration GroupModel { get; set; } = default!;

		public MethodConfiguration ItemModel { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		// ----------------

		public List<ArgumentValue> Argument { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				return $"{this.GroupModel.Name} - {this.ItemModel.Name}";
			}
		}

		public Boolean uBatch_IsEnabled {
			get {
				return this.ItemModel.Batchable is not null;
			}
		}

		public Boolean uBatch_IsChecked {
			get {
				return this.EnableBatch;
			}
		}

		public async void uBatch_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.EnableBatch = senders.IsChecked!.Value;
			this.NotifyPropertyChanged(
				nameof(this.uBatch_IsChecked)
			);
			return;
		}

		public async void uForward_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await ModdingWorker.ForwardHelper.Forward(this.GenerateCommand());
			return;
		}

		public async void uRemove_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			this.Host.uSelectedMethod_ItemsSource.Remove(this);
			return;
		}

		// ----------------

		public List<ArgumentConfiguration> uArgumentPanel_Configuration {
			get {
				return this.ItemModel.Argument;
			}
		}

		public List<ArgumentValue> uArgumentPanel_Value {
			get {
				return this.Argument;
			}
		}

		public UniqueStamp uArgumentPanel_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

		#region command

		public List<String> GenerateCommand (
		) {
			return new List<String>() {
				"?",
				"-method",
				$"{this.ItemModel.Id}{(!this.EnableBatch ? "" : ".batch")}",
				"-argument",
				ConfigurationHelper.MakeArgumentObjectString(this.ItemModel.Argument, this.Argument),
			};
		}

		#endregion

	}

}
