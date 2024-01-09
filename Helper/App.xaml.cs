#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.ApplicationModel;
using Windows.UI.StartScreen;
using Microsoft.UI.Xaml.Media;
using Microsoft.Windows.AppNotifications;

namespace Helper {

	public partial class App : Application {

		#region instance

		public static App Instance { get; private set; } = default!;

		public static View.MainWindow MainWindow { get; private set; } = default!;

		public static String ProgramFile { get; private set; } = default!;

		#endregion

		#region life

		public App (
		) {
			App.Instance = this;
			this.InitializeComponent();
		}

		// ----------------

		protected override async void OnLaunched (
			LaunchActivatedEventArgs args
		) {
			this.UnhandledException += this.OnUnhandledException;
			var window = default(Window);
			try {
				App.ProgramFile = StorageHelper.ToWindowsStyle(StorageHelper.Parent(Environment.GetCommandLineArgs()[0]) + "/Helper.exe");
				await Setting.Initialize();
				this.InitializeNotification();
				var optionWindowPosition = default(Tuple<Integer, Integer>?);
				var optionWindowSize = default(Tuple<Integer, Integer>?);
				var optionWindowAlwaysOnTop = default(Boolean?);
				var optionModuleType = default(View.ModuleType?);
				var optionModuleOption = default(List<String>?);
				{
					var option = new CommandLineReader(Environment.GetCommandLineArgs()[1..].ToList());
					if (option.Check("----AppNotificationActivated:")) {
						// skip if launch by AppNotification
					}
					if (option.Check("-Embedding")) {
						// skip if launch by AppNotification
					}
					if (option.Check("-WindowPosition")) {
						optionWindowPosition = new (
							option.NextInteger(),
							option.NextInteger()
						);
					}
					if (option.Check("-WindowSize")) {
						optionWindowSize = new (
							option.NextInteger(),
							option.NextInteger()
						);
					}
					if (option.Check("-WindowAlwaysOnTop")) {
						optionWindowAlwaysOnTop = option.NextBoolean();
					}
					if (option.Check("-ModuleType")) {
						optionModuleType = option.NextEnumeration<View.ModuleType>();
					}
					if (option.Check("-ModuleOption")) {
						optionModuleOption = option.NextStringList();
					}
					if (!option.Done()) {
						throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
					}
				}
				window = new View.MainWindow();
				if (optionWindowPosition is not null) {
					WindowHelper.Position(window, (Size)optionWindowPosition.Item1, (Size)optionWindowPosition.Item2);
				}
				if (optionWindowSize is not null) {
					WindowHelper.Size(window, (Size)optionWindowSize.Item1, (Size)optionWindowSize.Item2);
				}
				if (optionWindowAlwaysOnTop is not null) {
					WindowHelper.AlwaysOnTop(window, optionWindowAlwaysOnTop.Value);
				}
				if (optionWindowPosition is null) {
					WindowHelper.Center(window);
				}
				await window.AsClass<View.MainWindow>().InsertTabItem(optionModuleType ?? View.ModuleType.ModuleLauncher, optionModuleOption ?? []);
				App.MainWindow = window.AsClass<View.MainWindow>();
			}
			catch (Exception e) {
				window = new () {
					ExtendsContentIntoTitleBar = true,
					SystemBackdrop = new MicaBackdrop(),
					Content = new Control.Box() {
						Padding = new (16),
						Children = {
							new TextBlock() {
								HorizontalAlignment = HorizontalAlignment.Center,
								VerticalAlignment = VerticalAlignment.Center,
								TextWrapping = TextWrapping.Wrap,
								Text = e.ToString(),
							},
						},
					},
				};
			}
			WindowHelper.Track(window);
			WindowHelper.Title(window, Package.Current.DisplayName);
			WindowHelper.Icon(window, $"{StorageHelper.Parent(App.ProgramFile)}/Asset/Logo.ico");
			WindowHelper.Theme(window, Setting.Data.Application.ThemeMode);
			WindowHelper.Activate(window);
			return;
		}

		// ----------------

		protected void OnUnhandledException (
			Object                                        sender,
			Microsoft.UI.Xaml.UnhandledExceptionEventArgs args
		) {
			// ReSharper disable once ConditionIsAlwaysTrueOrFalseAccordingToNullableAPIContract
			if (App.MainWindow is not null) {
				args.Handled = true;
				try {
					_ = ControlHelper.ShowDialogSimple(App.MainWindow.Content, "Unhandled Exception", args.Exception.ToString());
				}
				catch (Exception) {
					// ignored
				}
			}
			return;
		}

		#endregion

		#region utility

		private AppNotificationManager NotificationManager = default!;

		private void NotificationManager_NotificationInvoked (
			AppNotificationManager            sender,
			AppNotificationActivatedEventArgs args
		) {
			// ReSharper disable once ConditionIsAlwaysTrueOrFalseAccordingToNullableAPIContract
			if (App.MainWindow is not null) {
				WindowHelper.ShowAsForeground(App.MainWindow);
			}
			return;
		}

		private void InitializeNotification (
		) {
			this.NotificationManager = AppNotificationManager.Default;
			this.NotificationManager.NotificationInvoked += this.NotificationManager_NotificationInvoked;
			this.NotificationManager.Register();
			return;
		}

		public void PushNotification (
			AppNotification notification
		) {
			this.NotificationManager.Show(notification);
			return;
		}

		// ----------------

		public async Task RegisterShellJumpList (
		) {
			if (!JumpList.IsSupported()) {
				return;
			}
			var list = await JumpList.LoadCurrentAsync();
			list.Items.Clear();
			foreach (var jumper in Setting.Data.ModuleLauncher.ModuleJumperConfiguration) {
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, View.ModuleLauncher.JumperConfiguration.GenerateArgument(jumper)), "");
				item.Logo = new ("ms-appx:///Asset/Logo.png");
				item.GroupName = "";
				item.DisplayName = jumper.Title;
				item.Description = "";
				list.Items.Add(item);
			}
			foreach (var jumper in Setting.Data.ModuleLauncher.PinnedJumperConfiguration) {
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, View.ModuleLauncher.JumperConfiguration.GenerateArgument(jumper)), "");
				item.Logo = new ("ms-appx:///Asset/Logo.png");
				item.GroupName = "Pinned";
				item.DisplayName = jumper.Title;
				item.Description = "";
				list.Items.Add(item);
			}
			foreach (var jumper in Setting.Data.ModuleLauncher.RecentJumperConfiguration) {
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, View.ModuleLauncher.JumperConfiguration.GenerateArgument(jumper)), "");
				item.Logo = new ("ms-appx:///Asset/Logo.png");
				item.GroupName = "Recent";
				item.DisplayName = jumper.Title;
				item.Description = "";
				list.Items.Add(item);
			}
			await list.SaveAsync();
			return;
		}

		// ----------------

		public async Task AppendRecentJumperItem (
			View.ModuleLauncher.JumperConfiguration configuration
		) {
			var pinnedItem = Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Find((value) => (View.ModuleLauncher.JumperConfiguration.CompareForModule(value, configuration)));
			if (pinnedItem is not null) {
				return;
			}
			var recentItem = Setting.Data.ModuleLauncher.RecentJumperConfiguration.Find((value) => (View.ModuleLauncher.JumperConfiguration.CompareForModule(value, configuration)));
			if (recentItem is not null) {
				Setting.Data.ModuleLauncher.RecentJumperConfiguration.Remove(recentItem);
				Setting.Data.ModuleLauncher.RecentJumperConfiguration.Insert(0, recentItem);
			}
			else {
				Setting.Data.ModuleLauncher.RecentJumperConfiguration.Insert(0, configuration);
			}
			await Setting.Save();
			return;
		}

		#endregion

	}

}
