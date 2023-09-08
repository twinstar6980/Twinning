#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;
using Microsoft.UI.Xaml.Navigation;

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

		protected override void OnNavigatedTo (
			NavigationEventArgs args
		) {
			if (args.Parameter is List<String> option) {
				this.Controller.ApplyCommandOption(option);
			}
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.Configuration Configuration { get; set; } = default!;

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
			this.uAvailableMethod_ItemsSource = this.Configuration.Method.Select((group) => (new AvailableMethodGroupItemController() {
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

		public async void ApplyCommandOption (
			List<String> optionView
		) {
			while (!this.View.IsLoaded) {
				await Task.Delay(TimeSpan.FromMilliseconds(50));
			}
			try {
				var option = new CommandLineReader(optionView);
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
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
					Argument = CommandConfigurationHelper.MakeArgumentValueDefault(itemItem.ItemModel.Argument),
					EnableBatch = false,
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

		public async void uRunWithAllSelectedMethodByParallel_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			try {
				foreach (var item in this.uSelectedMethod_ItemsSource) {
					ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, item.GenerateCommand()).Wait(0);
				}
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to create process.", e.ToString());
				return;
			}
			return;
		}

		public async void uRunWithAllSelectedMethodBySequence_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			try {
				if (this.uSelectedMethod_ItemsSource.Count != 0) {
					ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, this.uSelectedMethod_ItemsSource.SelectMany((item) => (item.GenerateCommand())).ToList()).Wait(0);
				}
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to create process.", e.ToString());
				return;
			}
			return;
		}

		public async void uRunWithoutAnySelectedMethod_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			try {
				ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, new List<String>()).Wait(0);
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to create process.", e.ToString());
				return;
			}
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

		public CommandConfigurationModel.MethodGroupConfiguration GroupModel { get; set; } = default!;

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

		public CommandConfigurationModel.MethodGroupConfiguration GroupModel { get; set; } = default!;

		public CommandConfigurationModel.MethodConfiguration ItemModel { get; set; } = default!;

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

	public class SelectedMethodItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.MethodGroupConfiguration GroupModel { get; set; } = default!;

		public CommandConfigurationModel.MethodConfiguration ItemModel { get; set; } = default!;

		public Boolean EnableBatch { get; set; } = default!;

		// ----------------

		public List<CommandConfigurationModel.ArgumentValue> Argument { get; set; } = default!;

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

		public async void uRun_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			try {
				ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, this.GenerateCommand()).Wait(0);
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to create process.", e.ToString());
				return;
			}
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

		public List<CommandConfigurationModel.ArgumentConfiguration> uArgumentPanel_Configuration {
			get {
				return this.ItemModel.Argument;
			}
		}

		public List<CommandConfigurationModel.ArgumentValue> uArgumentPanel_Value {
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
				CommandConfigurationHelper.MakeArgumentObjectString(this.ItemModel.Argument, this.Argument),
			};
		}

		#endregion

	}

}
