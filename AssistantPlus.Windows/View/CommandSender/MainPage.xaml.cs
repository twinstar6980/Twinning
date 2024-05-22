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

		#endregion

		#region initialize

		public void Initialize (
		) {
			var methodConfiguration = new List<MethodGroupConfiguration>();
			try {
				methodConfiguration = JsonHelper.DeserializeText<List<MethodGroupConfiguration>>(StorageHelper.ReadFileTextSync(App.Setting.Data.CommandSender.MethodConfiguration));
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to load method configuration.", e.ToString());
			}
			this.uAvailableMethodList_ItemsSource = methodConfiguration.Select((group) => (new MainPageAvailableMethodGroupItemController() {
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
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			try {
				var option = new CommandLineReader(optionView);
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		#endregion

		#region page

		public async void uPage_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Page>();
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Page>();
			return;
		}

		#endregion

		#region available method

		public List<MainPageAvailableMethodGroupItemController> uAvailableMethodList_ItemsSource { get; set; } = default!;

		public async void uAvailableMethodList_ItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			var senders = sender.AsClass<TreeView>();
			if (args.InvokedItem is MainPageAvailableMethodGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (Object.ReferenceEquals(value.Content, groupItem))).AsNotNull();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is MainPageAvailableMethodItemItemController itemItem) {
				this.uSelectedMethodList_ItemsSource.Add(new () {
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

		public ObservableCollection<MainPageSelectedMethodItemController> uSelectedMethodList_ItemsSource { get; } = [];

		// ----------------

		public async void uSelectedMethodForward_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			switch (senders.Tag.AsClass<String>()) {
				case "None": {
					await ModdingWorker.ForwardHelper.Forward([]);
					break;
				}
				case "Sequence": {
					await ModdingWorker.ForwardHelper.Forward(this.uSelectedMethodList_ItemsSource.Select((value) => (value.GenerateCommand())).ToList(), false);
					break;
				}
				case "Parallel": {
					await ModdingWorker.ForwardHelper.Forward(this.uSelectedMethodList_ItemsSource.Select((value) => (value.GenerateCommand())).ToList(), true);
					break;
				}
				default: throw new ();
			}
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
				return ConvertHelper.MakeBooleanToFloaterOfOpacityVisibility(this.ItemModel.Batchable is not null);
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

		public async void uBatch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.EnableBatch = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged(
				nameof(this.uBatch_IsChecked)
			);
			return;
		}

		public async void uForward_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			await ModdingWorker.ForwardHelper.Forward(this.GenerateCommand());
			return;
		}

		public async void uRemove_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			this.Host.uSelectedMethodList_ItemsSource.Remove(this);
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
			return [
				"?",
				"-method",
				$"{this.ItemModel.Id}{(!this.EnableBatch ? "" : ".batch")}",
				"-argument",
				ConfigurationHelper.MakeArgumentObjectString(this.ItemModel.Argument, this.Argument),
			];
		}

		#endregion

	}

}
