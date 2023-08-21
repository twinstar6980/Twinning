#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;

namespace Helper.Module.CommandForwarder {

	public sealed partial class MainPage : Page {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new MainPageController() { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public MethodConfigurationModel.Configuration Configuration { get; set; } = default!;

		#endregion

		#region initialize

		public void Initialize (
		) {
			try {
				this.Configuration = JsonHelper.Deserialize<MethodConfigurationModel.Configuration>(StorageHelper.ReadFileTextSync(Setting.CommandForwarderMethodConfiguration));
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(92, InfoBarSeverity.Error, "Failed to load method configuration.");
				this.Configuration = new MethodConfigurationModel.Configuration() {
					Group = new List<MethodConfigurationModel.MethodGroupConfiguration>(),
					QuickOption = new List<MethodConfigurationModel.QuickOptionGroupConfiguration>(),
				};
			}
			this.uAvailableMethod_ItemsSource = this.Configuration.Group.Select((group) => (new AvailableMethodGroupItemController() {
				Host = this,
				GroupModel = group,
				Children = group.Item.Select((item) => (new AvailableMethodItemItemController() {
					Host = this,
					GroupModel = group,
					ItemModel = item,
				})).ToList(),
			})).ToList();
			return;
		}

		#endregion

		#region available method

		public List<AvailableMethodGroupItemController> uAvailableMethod_ItemsSource { get; set; } = default!;

		public async void uAvailableMethod_OnItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			if (sender is not TreeView senders) { return; }
			if (args.InvokedItem is AvailableMethodGroupItemController groupItem) {
				var node = senders.RootNodes.ToList().Find((value) => (value.Content == groupItem)) ?? throw new Exception();
				node.IsExpanded = !node.IsExpanded;
			}
			if (args.InvokedItem is AvailableMethodItemItemController itemItem) {
				this.uSelectedMethod_ItemsSource.Add(new SelectedMethodItemController() {
					Host = this,
					GroupModel = itemItem.GroupModel,
					ItemModel = itemItem.ItemModel,
					Argument = MethodConfigurationHelper.MakeArgumentValueDefault(itemItem.ItemModel.Argument),
				});
				await Task.Delay(TimeSpan.FromMilliseconds(100));
				this.View.uSelectedMethodScrollViewer.ScrollToVerticalOffset((this.View.uSelectedMethodScrollViewer.Content as FrameworkElement)!.ActualHeight);
			}
			return;
		}

		#endregion

		#region selected method

		public ObservableCollection<SelectedMethodItemController> uSelectedMethod_ItemsSource { get; } = new ObservableCollection<SelectedMethodItemController>();

		// ----------------

		public async void uRunAllSelectedMethod_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!StorageHelper.ExistFile(Setting.CommandForwarderLaunchScript)) {
				MainWindow.Instance.Controller.PublishTip(92, InfoBarSeverity.Error, "Invalid path of launch script.");
				return;
			}
			var command = this.uSelectedMethod_ItemsSource.SelectMany((value) => (value.GenerateCommand())).ToList();
			ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, command).Wait(0);
			return;
		}

		#endregion

	}

	public class AvailableMethodItemTemplateSelector : DataTemplateSelector {

		public DataTemplate? GroupTemplate { get; set; }

		public DataTemplate? ItemTemplate { get; set; }

		// ----------------

		protected override DataTemplate? SelectTemplateCore (
			Object item
		) {
			return item switch {
				AvailableMethodGroupItemController => this.GroupTemplate,
				AvailableMethodItemItemController  => this.ItemTemplate,
				_                                  => null,
			};
		}

	}

	public class AvailableMethodGroupItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodConfigurationModel.MethodGroupConfiguration GroupModel { get; set; } = default!;

		// ----------------

		public List<AvailableMethodItemItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public List<AvailableMethodItemItemController> uRoot_ItemsSource {
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

	public class AvailableMethodItemItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodConfigurationModel.MethodGroupConfiguration GroupModel { get; set; } = default!;

		public MethodConfigurationModel.MethodConfiguration ItemModel { get; set; } = default!;

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

		#endregion

	}

	public class SelectedMethodItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public MethodConfigurationModel.MethodGroupConfiguration GroupModel { get; set; } = default!;

		public MethodConfigurationModel.MethodConfiguration ItemModel { get; set; } = default!;

		// ----------------

		public List<MethodConfigurationModel.ArgumentValue> Argument { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				return $"{this.GroupModel.Name} - {this.ItemModel.Name}";
			}
		}

		public async void uRemove_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			this.Host.uSelectedMethod_ItemsSource.Remove(this);
			return;
		}

		public async void uRun_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!StorageHelper.ExistFile(Setting.CommandForwarderLaunchScript)) {
				MainWindow.Instance.Controller.PublishTip(92, InfoBarSeverity.Error, "Invalid path of launch script.");
				return;
			}
			var command = this.GenerateCommand();
			ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, command).Wait(0);
			return;
		}

		// ----------------

		public List<MethodConfigurationModel.ArgumentConfiguration> uArgumentPanel_Configuration {
			get {
				return this.ItemModel.Argument;
			}
		}

		public List<MethodConfigurationModel.ArgumentValue> uArgumentPanel_Value {
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
				this.ItemModel.Id,
				"-argument",
				MethodConfigurationHelper.MakeArgumentValueString(this.ItemModel.Argument, this.Argument),
			};
		}

		#endregion

	}

}
