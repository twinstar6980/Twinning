#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Navigation;

namespace Helper.Module.ModuleLauncher {

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
			this.Controller.ApplyOption(args.Parameter.AsClass<List<String>>());
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

		#region tab item page

		public async Task<Boolean> TabItemPageRequestClose (
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
			this.uModuleJumperList_ItemsSource = Setting.Data.ModuleLauncher.ModuleJumperConfiguration.Select((value) => (new MainPageJumperItemController() { Host = this, Configuration = value })).ToList();
			this.uPinnedJumperList_ItemsSource = Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Select((value) => (new MainPageJumperItemController() { Host = this, Configuration = value })).ToObservableCollection();
			this.uRecentJumperList_ItemsSource = Setting.Data.ModuleLauncher.RecentJumperConfiguration.Select((value) => (new MainPageJumperItemController() { Host = this, Configuration = value })).ToObservableCollection();
			return;
		}

		public async void ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			try {
				var option = new CommandLineReader(optionView);
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			return;
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

		#region application setting

		public async void uOpenApplicationProgramFile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<HyperlinkButton>();
			await StorageHelper.RevealDirectory(StorageHelper.Parent(App.ProgramFile).AsNotNull());
			return;
		}

		public async void uOpenApplicationSettingFile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<HyperlinkButton>();
			await StorageHelper.RevealDirectory(StorageHelper.Parent(Setting.File).AsNotNull());
			return;
		}

		// ----------------

		public List<String> uApplicationThemeMode_ItemsSource {
			get {
				return Enum.GetValues<ElementTheme>().Select(ConvertHelper.ThemeToString).ToList();
			}
		}

		public Size uApplicationThemeMode_SelectedIndex {
			get {
				return (Size)Setting.Data.Application.ThemeMode;
			}
		}

		public async void uApplicationThemeMode_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			Setting.Data.Application.ThemeMode = (ElementTheme)senders.SelectedIndex;
			Setting.Save();
			return;
		}

		#endregion

		#region jumper

		public List<MainPageJumperItemController> uModuleJumperList_ItemsSource { get; set; } = default!;

		// ----------------

		public ObservableCollection<MainPageJumperItemController> uPinnedJumperList_ItemsSource { get; set; } = default!;

		public async void uAddPinnedJumperItem_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Add(new JumperConfiguration() {
				Title = "Untitled",
				ModuleType = ModuleType.ModuleLauncher,
				ModuleOption = new List<String>(),
				WindowOption = new List<String>(),
			});
			this.uPinnedJumperList_ItemsSource.Add(new MainPageJumperItemController() { Host = this, Configuration = Setting.Data.ModuleLauncher.PinnedJumperConfiguration[^1] });
			Setting.Save();
			return;
		}

		// ----------------

		public ObservableCollection<MainPageJumperItemController> uRecentJumperList_ItemsSource { get; set; } = default!;

		public async void uClearRecentJumperItem_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			Setting.Data.ModuleLauncher.RecentJumperConfiguration.Clear();
			this.uRecentJumperList_ItemsSource.Clear();
			Setting.Save();
			return;
		}

		#endregion

	}

	public class MainPageJumperItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public JumperConfiguration Configuration { get; set; } = default!;

		#endregion

		#region view

		public String uIcon_Glyph {
			get {
				return ModuleInformationConstant.Query(this.Configuration.ModuleType).Icon;
			}
		}

		public String uTitle_Text {
			get {
				return this.Configuration.Title;
			}
		}

		// ----------------

		public async void uOpenAsTab_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			JumperConfiguration.Launch(this.Configuration, false);
			return;
		}

		public async void uOpenAsWindow_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			JumperConfiguration.Launch(this.Configuration, true);
			return;
		}

		public async void uEdit_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Jumper Configuration", new JumperConfigurationPanel() {
				Configuration = this.Configuration,
				Stamp = UniqueStamp.Create(),
			});
			Setting.Save();
			this.NotifyPropertyChanged(
				nameof(this.uIcon_Glyph),
				nameof(this.uTitle_Text)
			);
			return;
		}

		// ----------------

		public async void uPinnedRemove_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Remove(this.Configuration);
			this.Host.uPinnedJumperList_ItemsSource.Remove(this);
			Setting.Save();
			return;
		}

		public async void uRecentRemove_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			Setting.Data.ModuleLauncher.RecentJumperConfiguration.Remove(this.Configuration);
			this.Host.uRecentJumperList_ItemsSource.Remove(this);
			Setting.Save();
			return;
		}

		public async void uRecentPin_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			Setting.Data.ModuleLauncher.RecentJumperConfiguration.Remove(this.Configuration);
			Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Add(this.Configuration);
			this.Host.uPinnedJumperList_ItemsSource.Add(new MainPageJumperItemController() { Host = this.Host, Configuration = this.Configuration });
			this.Host.uRecentJumperList_ItemsSource.Remove(this);
			Setting.Save();
			return;
		}

		// ----------------

		public async void uModuleSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Module Setting", this.Configuration.ModuleType switch {
				ModuleType.ModuleLauncher    => new ModuleLauncherSettingPanel(),
				ModuleType.ModdingWorker     => new ModdingWorkerSettingPanel(),
				ModuleType.ResourceForwarder => new ResourceForwarderSettingPanel(),
				ModuleType.CommandSender     => new CommandSenderSettingPanel(),
				ModuleType.AnimationViewer   => new AnimationViewerSettingPanel(),
				_                            => throw new ArgumentOutOfRangeException(),
			});
			Setting.Save();
			return;
		}

		#endregion

	}

}
