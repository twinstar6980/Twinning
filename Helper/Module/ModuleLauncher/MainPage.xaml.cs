#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Controls;
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
			this.Controller.ApplyOption(args.Parameter as List<String>);
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		public MainPageController Controller { get; }

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

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.uModuleJumperList_ItemsSource = new List<MainPageJumperItemController>(Setting.Data.ModuleLauncher.ModuleJumperConfiguration.Select((value) => (new MainPageJumperItemController() { Host = this, Configuration = value })));
			this.uPinnedJumperList_ItemsSource = new ObservableCollection<MainPageJumperItemController>(Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Select((value) => (new MainPageJumperItemController() { Host = this, Configuration = value })));
			this.uRecentJumperList_ItemsSource = new ObservableCollection<MainPageJumperItemController>(Setting.Data.ModuleLauncher.RecentJumperConfiguration.Select((value) => (new MainPageJumperItemController() { Host = this, Configuration = value })));
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

		#region application setting

		public async void uOpenApplicationProgramFile_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not HyperlinkButton senders) { return; }
			await Windows.System.Launcher.LaunchFolderPathAsync(StorageHelper.ToWindowsStyle(StorageHelper.Parent(App.ProgramFile)!));
			return;
		}

		public async void uOpenApplicationSettingFile_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not HyperlinkButton senders) { return; }
			await Windows.System.Launcher.LaunchFolderPathAsync(StorageHelper.ToWindowsStyle(StorageHelper.Parent(Setting.File)!));
			return;
		}

		// ----------------

		public List<String> uApplicationThemeMode_ItemsSource {
			get {
				return new[] { ElementTheme.Default, ElementTheme.Light, ElementTheme.Dark }.Select((value) => (ConvertHelper.ThemeToString(value))).ToList();
			}
		}

		public Size uApplicationThemeMode_SelectedIndex {
			get {
				return (Size)Setting.Data.Application.ThemeMode;
			}
		}

		public async void uApplicationThemeMode_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			var newValue = (ElementTheme)senders.SelectedIndex;
			Setting.Data.Application.ThemeMode = newValue;
			Setting.Save();
			return;
		}

		#endregion

		#region jumper list

		public List<MainPageJumperItemController> uModuleJumperList_ItemsSource { get; set; } = default!;

		// ----------------

		public ObservableCollection<MainPageJumperItemController> uPinnedJumperList_ItemsSource { get; set; } = default!;

		public async void uAddPinnedJumperItem_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
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

		public async void uClearRecentJumperItem_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
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

		public async void uOpenAsTab_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			JumperConfiguration.Launch(this.Configuration, false);
			return;
		}

		public async void uOpenAsWindow_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			JumperConfiguration.Launch(this.Configuration, true);
			return;
		}

		public async void uEdit_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			await new ContentDialog() {
				XamlRoot = this.Host.View.XamlRoot,
				Resources = new ResourceDictionary() {
					new KeyValuePair<Object, Object>("ContentDialogMinWidth", 720.0),
					new KeyValuePair<Object, Object>("ContentDialogMaxWidth", 720.0),
					new KeyValuePair<Object, Object>("ContentDialogMinHeight", 640.0),
					new KeyValuePair<Object, Object>("ContentDialogMaxHeight", 640.0),
				},
				Title = "Jumper Configuration",
				Content = new ScrollViewer() {
					IsTabStop = true,
					Padding = new Thickness(12, 0, 12, 0),
					VerticalScrollMode = ScrollMode.Enabled,
					VerticalScrollBarVisibility = ScrollBarVisibility.Visible,
					Content = new JumperConfigurationPanel() {
						Configuration = this.Configuration,
						Stamp = UniqueStamp.Default,
					},
				},
				CloseButtonText = "Close",
				DefaultButton = ContentDialogButton.Close,
			}.ShowAsync();
			this.NotifyPropertyChanged(
				nameof(this.uIcon_Glyph),
				nameof(this.uTitle_Text)
			);
			Setting.Save();
			return;
		}

		// ----------------

		public async void uPinnedRemove_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Remove(this.Configuration);
			this.Host.uPinnedJumperList_ItemsSource.Remove(this);
			Setting.Save();
			return;
		}

		public async void uRecentRemove_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			Setting.Data.ModuleLauncher.RecentJumperConfiguration.Remove(this.Configuration);
			this.Host.uRecentJumperList_ItemsSource.Remove(this);
			Setting.Save();
			return;
		}

		public async void uRecentPin_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			Setting.Data.ModuleLauncher.RecentJumperConfiguration.Remove(this.Configuration);
			Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Add(this.Configuration);
			this.Host.uPinnedJumperList_ItemsSource.Add(new MainPageJumperItemController() { Host = this.Host, Configuration = this.Configuration });
			this.Host.uRecentJumperList_ItemsSource.Remove(this);
			Setting.Save();
			return;
		}

		// ----------------

		public async void uModuleSetting_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var page = this.Configuration.ModuleType switch {
				ModuleType.ModuleLauncher    => new ModuleLauncherSettingPanel() as FrameworkElement,
				ModuleType.ModdingWorker     => new ModdingWorkerSettingPanel(),
				ModuleType.ResourceForwarder => new ResourceForwarderSettingPanel(),
				ModuleType.CommandSender     => new CommandSenderSettingPanel(),
				ModuleType.AnimationViewer   => new AnimationViewerSettingPanel(),
				_                            => throw new ArgumentOutOfRangeException(),
			};
			await new ContentDialog() {
				XamlRoot = this.Host.View.XamlRoot,
				Resources = new ResourceDictionary() {
					new KeyValuePair<Object, Object>("ContentDialogMinWidth", 720.0),
					new KeyValuePair<Object, Object>("ContentDialogMaxWidth", 720.0),
					new KeyValuePair<Object, Object>("ContentDialogMinHeight", 640.0),
					new KeyValuePair<Object, Object>("ContentDialogMaxHeight", 640.0),
				},
				Title = "Module Setting",
				Content = new ScrollViewer() {
					IsTabStop = true,
					Padding = new Thickness(12, 0, 12, 0),
					VerticalScrollMode = ScrollMode.Enabled,
					VerticalScrollBarVisibility = ScrollBarVisibility.Visible,
					Content = page,
				},
				CloseButtonText = "Close",
				DefaultButton = ContentDialogButton.Close,
			}.ShowAsync();
			Setting.Save();
			return;
		}

		#endregion

	}

}
