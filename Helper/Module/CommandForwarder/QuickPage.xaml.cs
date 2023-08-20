#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
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
				this.Controller.Input = parameter;
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

		public MethodConfigurationModel.Configuration Configuration { get; set; } = null!;

		// ----------------

		public List<String>? Input { get; set; } = null;

		public Boolean Filter { get; set; } = true;

		public Boolean Parallel { get; set; } = false;

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.Input = null;
			this.Filter = true;
			this.Parallel = false;
			try {
				this.Configuration = JsonHelper.Deserialize<MethodConfigurationModel.Configuration>(StorageHelper.ReadFileTextSync(Setting.CommandForwarderMethodConfiguration));
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(16, InfoBarSeverity.Error, "Failed to load method configuration.");
				this.Configuration = new MethodConfigurationModel.Configuration() {
					Group = new List<MethodConfigurationModel.MethodGroupConfiguration>(),
					QuickOption = new List<MethodConfigurationModel.QuickOptionGroupConfiguration>(),
				};
			}
			this.uOption_ItemsSource = this.Configuration.QuickOption.Select((group) => (new QuickOptionGroupItemController() {
				Host = this,
				GroupModel = group,
				Children = group.Item.Select((item) => (new QuickOptionItemItemController() {
					Host = this,
					ItemModel = item,
					Available = false,
				})).ToList(),
			})).ToList();
			return;
		}

		// ----------------

		public Boolean CheckInput (
			String                                                  input,
			MethodConfigurationModel.QuickOptionFilterConfiguration filter
		) {
			var state = true;
			state &= filter.Type switch {
				null  => true,
				false => StorageHelper.ExistFile(input),
				true  => StorageHelper.ExistDirectory(input),
			};
			state &= filter.Name is null || Regex.IsMatch(input, filter.Name);
			return state;
		}

		public async void RefreshInput (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uInputCount_Content),
				nameof(this.uInputTip_Subtitle)
			);
			foreach (var group in this.uOption_ItemsSource) {
				foreach (var item in group.Children) {
					item.Available = this.Input is not null && this.Input.All((inputItem) => (this.CheckInput(inputItem, item.ItemModel.Filter)));
				}
			}
			return;
		}

		public async void RefreshFilter (
		) {
			foreach (var group in this.uOption_ItemsSource) {
				foreach (var item in group.Children) {
					item.NotifyPropertyChanged(nameof(item.uRoot_Visibility));
				}
				group.NotifyPropertyChanged(nameof(group.uRoot_Visibility));
			}
			return;
		}

		#endregion

		#region setting

		public String uInputCount_Content {
			get {
				if (this.Input is null) {
					return "";
				}
				return $"{this.Input.Count} - Input";
			}
		}

		public async void uInputCount_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (this.Input is null) { return; }
			this.View.uInputTip.IsOpen = true;
			return;
		}

		public String uInputTip_Subtitle {
			get {
				if (this.Input is null) {
					return "";
				}
				return String.Join('\n', this.Input);
			}
		}

		// ----------------

		public Boolean uParallel_IsChecked {
			get {
				return this.Parallel;
			}
		}

		public async void uParallel_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.Parallel = senders.IsChecked!.Value;
			return;
		}

		// ----------------

		public Boolean uFilter_IsChecked {
			get {
				return this.Filter;
			}
		}

		public async void uFilter_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			this.Filter = senders.IsChecked!.Value;
			this.RefreshFilter();
			return;
		}

		#endregion

		#region option

		public List<QuickOptionGroupItemController> uOption_ItemsSource { get; set; } = null!;

		public async void uOption_OnItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			if (sender is not TreeView senders) { return; }
			Debug.Assert(this.Input is not null);
			if (args.InvokedItem is QuickOptionItemItemController selectedItem) {
				if (!StorageHelper.ExistFile(Setting.CommandForwarderLaunchScript)) {
					MainWindow.Instance.Controller.PublishTip(16, InfoBarSeverity.Error, "Invalid path of launch script.");
					return;
				}
				var configuration = selectedItem.ItemModel;
				var argument = new List<String>();
				foreach (var input in this.Input) {
					argument.Add(input);
					if (configuration.Method is not null) {
						argument.Add("-method");
						argument.Add(configuration.Method);
					}
					argument.Add("-argument");
					argument.Add(JsonHelper.Serialize(configuration.Argument));
					if (this.Parallel) {
						ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, argument).Wait(0);
						argument.Clear();
					}
				}
				if (!this.Parallel) {
					ProcessHelper.CreateProcessForCommandScript(Setting.CommandForwarderLaunchScript, argument).Wait(0);
				}
				MainWindow.Instance.Close();
			}
			return;
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

		public MethodConfigurationModel.QuickOptionGroupConfiguration GroupModel { get; init; } = default!;

		public List<QuickOptionItemItemController> Children { get; init; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return !this.Host.Filter || this.Children.Any((value) => (value.Available));
			}
		}

		public String uRoot_Content {
			get {
				return this.GroupModel.Name;
			}
		}

		public List<QuickOptionItemItemController> uRoot_ItemsSource {
			get {
				return this.Children;
			}
		}

		#endregion

	}

	public class QuickOptionItemItemController : CustomController {

		#region data

		public QuickPageController Host { get; init; } = default!;

		// ----------------

		public MethodConfigurationModel.QuickOptionConfiguration ItemModel { get; init; } = default!;

		// ----------------

		public Boolean Available { get; set; } = default!;

		#endregion

		#region view

		public Boolean uRoot_Visibility {
			get {
				return !this.Host.Filter || this.Available;
			}
		}

		public String uRoot_Content {
			get {
				return this.ItemModel.Name;
			}
		}

		#endregion

	}

}
