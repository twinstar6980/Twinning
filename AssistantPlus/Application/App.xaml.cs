#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel;
using Windows.UI.StartScreen;
using Microsoft.UI.Xaml.Media;
using Microsoft.Windows.AppNotifications;

namespace AssistantPlus {

	public partial class App : Application {

		#region instance

		public static App Instance { get; private set; } = default!;

		public static SettingProvider Setting { get; private set; } = default!;

		public static View.Home.MainWindow MainWindow { get; private set; } = default!;

		public static String PackageDirectory { get; private set; } = default!;

		public static String ProgramFile { get; private set; } = default!;

		public static String SharedDirectory { get; private set; } = default!;

		public static String CacheDirectory { get; private set; } = default!;

		// ----------------

		public static Boolean MainWindowIsInitialized {
			get {
				// ReSharper disable once ConditionIsAlwaysTrueOrFalseAccordingToNullableAPIContract
				return App.MainWindow != null;
			}
		}

		#endregion

		#region life

		public App (
		) {
			App.Instance = this;
			App.Setting = new ();
			this.InitializeComponent();
		}

		// ----------------

		protected override async void OnLaunched (
			LaunchActivatedEventArgs args
		) {
			AppDomain.CurrentDomain.ProcessExit += this.OnProcessExit;
			this.UnhandledException += this.OnUnhandledException;
			TaskScheduler.UnobservedTaskException += this.OnUnobservedTaskException;
			var window = default(Window);
			try {
				var argument = Environment.GetCommandLineArgs();
				App.PackageDirectory = StorageHelper.Parent(argument[0]).AsNotNull();
				App.ProgramFile = $"{App.PackageDirectory}/Application.exe";
				App.SharedDirectory = StorageHelper.Regularize($"{Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)}/{Package.Current.Id.Name}");
				App.CacheDirectory = $"{App.SharedDirectory}/Cache";
				argument = argument[1..];
				try {
					await App.Setting.Load();
				}
				catch (Exception) {
					await App.Setting.Reset();
				}
				await App.Setting.Save();
				this.RegisterNotification();
				var optionWindowPosition = default(Tuple<Integer, Integer>?);
				var optionWindowSize = default(Tuple<Integer, Integer>?);
				var optionLaunch = default(Tuple<String, ModuleType, List<String>>?);
				var optionForward = default(Tuple<List<String>>?);
				{
					var command = new List<String>();
					if (argument.Length >= 1 && argument[0] == "Application") {
						command = argument[1..].ToList();
					}
					if (argument.Length == 1 && argument[0].StartsWith("twinstar.twinning.assistant-plus:")) {
						var link = new Uri(argument[0]);
						if (link.Scheme != "twinstar.twinning.assistant-plus" || link.Authority != "" || link.AbsolutePath != "/Application") {
							throw new ($"Invalid link.");
						}
						command = link.GetComponents(UriComponents.Query, UriFormat.UriEscaped)
							.Split("&")
							.Select((item) => (item.Split("=").Select(Uri.UnescapeDataString).ToList()))
							.Select((item) => (new KeyValuePair<String, String>(item[0], item[1])))
							.Where((item) => (item.Key == "Command"))
							.Select((item) => (item.Value))
							.ToList();
					}
					var option = new CommandLineReader(command);
					if (option.Check("-WindowPosition")) {
						optionWindowPosition = new (
							option.NextInteger(),
							option.NextInteger()
						);
					}
					else {
						if (App.Setting.Data.Window.Position.State) {
							optionWindowPosition = new (App.Setting.Data.Window.Position.X, App.Setting.Data.Window.Position.Y);
						}
					}
					if (option.Check("-WindowSize")) {
						optionWindowSize = new (
							option.NextInteger(),
							option.NextInteger()
						);
					}
					else {
						if (App.Setting.Data.Window.Size.State) {
							optionWindowSize = new (App.Setting.Data.Window.Size.Width, App.Setting.Data.Window.Size.Height);
						}
					}
					if (option.Check("-Launch")) {
						optionLaunch = new (
							option.NextString(),
							option.NextString().SelfLet((it) => (Enum.Parse<ModuleType>(it))),
							option.NextStringList()
						);
					}
					if (option.Check("-Forward")) {
						optionForward = new (
							option.NextStringList()
						);
					}
					if (!option.Done()) {
						throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
					}
				}
				window = new View.Home.MainWindow();
				if (optionWindowSize != null) {
					WindowHelper.Size(window, (Size)optionWindowSize.Item1, (Size)optionWindowSize.Item2);
				}
				if (optionWindowPosition != null) {
					WindowHelper.Position(window, (Size)optionWindowPosition.Item1, (Size)optionWindowPosition.Item2);
				}
				else {
					WindowHelper.Center(window);
				}
				if (optionLaunch != null) {
					_ = window.As<View.Home.MainWindow>().SetDefaultView(new () {
						Title = optionLaunch.Item1,
						Type = optionLaunch.Item2,
						Option = optionLaunch.Item3,
						Command = [],
					});
				}
				else if (optionForward != null) {
					_ = window.As<View.Home.MainWindow>().SetDefaultView(new () {
						Title = ModuleHelper.Query(ModuleType.ResourceShipper).Name,
						Type = ModuleType.ResourceShipper,
						Option = ["-Resource", ..optionForward.Item1],
						Command = [],
					});
				}
				else {
					_ = window.As<View.Home.MainWindow>().SetDefaultView();
				}
				App.MainWindow = window.As<View.Home.MainWindow>();
				await App.Setting.Apply();
			}
			catch (Exception e) {
				window = new () {
					ExtendsContentIntoTitleBar = true,
					SystemBackdrop = new MicaBackdrop(),
					Content = new Control.Box() {
						RequestedTheme = ElementTheme.Default,
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
			WindowHelper.Icon(window, $"{App.PackageDirectory}/Asset/Logo.ico");
			WindowHelper.Activate(window);
			return;
		}

		// ----------------

		private void OnProcessExit (
			Object?   sender,
			EventArgs args
		) {
			this.UnregisterNotification();
			return;
		}

		private void OnUnhandledException (
			Object                                        sender,
			Microsoft.UI.Xaml.UnhandledExceptionEventArgs args
		) {
			args.Handled = true;
			this.HandleException(args.Exception);
			return;
		}

		private void OnUnobservedTaskException (
			Object?                          sender,
			UnobservedTaskExceptionEventArgs args
		) {
			args.SetObserved();
			this.HandleException(args.Exception);
			return;
		}

		#endregion

		#region utility

		private void HandleException (
			Exception exception
		) {
			if (App.MainWindowIsInitialized) {
				App.MainWindow.DispatcherQueue.EnqueueAsync(() => {
					try {
						_ = ControlHelper.ShowDialogAsAutomatic(App.MainWindow.Content, "Unhandled Exception", new TextBlock() {
							HorizontalAlignment = HorizontalAlignment.Stretch,
							VerticalAlignment = VerticalAlignment.Stretch,
							IsTextSelectionEnabled = true,
							TextWrapping = TextWrapping.Wrap,
							Text = exception.ToString(),
						}, null);
					}
					catch (Exception) {
						// ignored
					}
				});
			}
			return;
		}

		// ----------------

		private void OnNotificationInvoked (
			AppNotificationManager            sender,
			AppNotificationActivatedEventArgs args
		) {
			if (App.MainWindowIsInitialized) {
				WindowHelper.ShowAsForeground(App.MainWindow);
			}
			return;
		}

		private void RegisterNotification (
		) {
			AppNotificationManager.Default.NotificationInvoked += this.OnNotificationInvoked;
			AppNotificationManager.Default.Register();
			return;
		}

		private void UnregisterNotification (
		) {
			AppNotificationManager.Default.Unregister();
			return;
		}

		public void PushNotification (
			AppNotification notification
		) {
			AppNotificationManager.Default.Show(notification);
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
			foreach (var launcher in App.Setting.Data.ModuleLauncher.Module) {
				list.Items.Add(JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, ModuleHelper.GenerateArgument(launcher)), "").SelfAlso((it) => {
					it.Logo = new ("ms-appx:///Asset/Logo.png");
					it.GroupName = "";
					it.DisplayName = launcher.Title;
					it.Description = "";
				}));
			}
			foreach (var launcher in App.Setting.Data.ModuleLauncher.Pinned) {
				list.Items.Add(JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, ModuleHelper.GenerateArgument(launcher)), "").SelfAlso((it) => {
					it.Logo = new ("ms-appx:///Asset/Logo.png");
					it.GroupName = "Pinned";
					it.DisplayName = launcher.Title;
					it.Description = "";
				}));
			}
			foreach (var launcher in App.Setting.Data.ModuleLauncher.Recent) {
				list.Items.Add(JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, ModuleHelper.GenerateArgument(launcher)), "").SelfAlso((it) => {
					it.Logo = new ("ms-appx:///Asset/Logo.png");
					it.GroupName = "Recent";
					it.DisplayName = launcher.Title;
					it.Description = "";
				}));
			}
			await list.SaveAsync();
			return;
		}

		// ----------------

		public async Task AppendPinnedLauncherItem (
			ModuleLauncherConfiguration launcher
		) {
			App.Setting.Data.ModuleLauncher.Pinned.Add(launcher);
			await App.Setting.Save();
			return;
		}

		public async Task AppendRecentLauncherItem (
			ModuleLauncherConfiguration launcher
		) {
			var pinnedItem = App.Setting.Data.ModuleLauncher.Pinned.Find((value) => (ModuleHelper.CompareLauncher(value, launcher)));
			if (pinnedItem != null) {
				return;
			}
			var recentItem = App.Setting.Data.ModuleLauncher.Recent.Find((value) => (ModuleHelper.CompareLauncher(value, launcher)));
			if (recentItem != null) {
				App.Setting.Data.ModuleLauncher.Recent.Remove(recentItem);
				App.Setting.Data.ModuleLauncher.Recent.Insert(0, recentItem);
			}
			else {
				App.Setting.Data.ModuleLauncher.Recent.Insert(0, launcher);
			}
			await App.Setting.Save();
			return;
		}

		// ----------------

		public async Task ExecuteLauncher (
			ModuleLauncherConfiguration launcher,
			Boolean                     forNewWindow
		) {
			if (!forNewWindow) {
				await App.MainWindow.InsertTabItem(launcher);
			}
			else {
				await ProcessHelper.SpawnChild(App.ProgramFile, ModuleHelper.GenerateArgument(launcher), false);
			}
			return;
		}

		#endregion

	}

}
