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

		public static Module.MainWindow MainWindow { get; private set; } = default!;

		public static String ProgramFile { get; private set; } = default!;

		#endregion

		#region life

		public App (
		) {
			App.Instance = this;
			this.InitializeComponent();
		}

		// ----------------

		protected override void OnLaunched (
			LaunchActivatedEventArgs args
		) {
			this.UnhandledException += this.OnUnhandledException;
			var window = default(Window);
			try {
				App.ProgramFile = StorageHelper.ToWindowsStyle(StorageHelper.Parent(Environment.GetCommandLineArgs()[0]) + "/Helper.exe");
				Setting.Initialize();
				this.InitializeNotification();
				var optionWindowPosition = default(Tuple<Integer, Integer>?);
				var optionWindowSize = default(Tuple<Integer, Integer>?);
				var optionWindowAlwaysOnTop = default(Boolean?);
				var optionModuleType = default(Module.ModuleType?);
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
						optionWindowPosition = new Tuple<Integer, Integer>(
							option.NextInteger(),
							option.NextInteger()
						);
					}
					if (option.Check("-WindowSize")) {
						optionWindowSize = new Tuple<Integer, Integer>(
							option.NextInteger(),
							option.NextInteger()
						);
					}
					if (option.Check("-WindowAlwaysOnTop")) {
						optionWindowAlwaysOnTop = option.NextBoolean();
					}
					if (option.Check("-ModuleType")) {
						optionModuleType = option.NextEnumeration<Module.ModuleType>();
					}
					if (option.Check("-ModuleOption")) {
						optionModuleOption = option.NextStringList();
					}
					if (!option.Done()) {
						throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
					}
				}
				window = new Module.MainWindow();
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
				window.AsClass<Module.MainWindow>().InsertTabItem(optionModuleType ?? Module.ModuleType.ModuleLauncher, optionModuleOption ?? new List<String>()).Wait(0);
				App.MainWindow = window.AsClass<Module.MainWindow>();
			}
			catch (Exception e) {
				window = new Window() {
					ExtendsContentIntoTitleBar = true,
					SystemBackdrop = new MicaBackdrop(),
					Content = new CommonControl.Box() {
						Padding = new Thickness(16),
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
			try {
				// ReSharper disable once ConditionIsAlwaysTrueOrFalseAccordingToNullableAPIContract
				if (App.MainWindow is not null) {
					ControlHelper.ShowDialogSimple(App.MainWindow.Content, "Unhandled Exception", args.Exception.ToString()).Wait(0);
					args.Handled = true;
				}
			}
			catch (Exception) {
				// ignored
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
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, Module.ModuleLauncher.JumperConfiguration.GenerateArgument(jumper)), "");
				item.Logo = new Uri("ms-appx:///Asset/Logo.png");
				item.GroupName = "";
				item.DisplayName = jumper.Title;
				item.Description = "";
				list.Items.Add(item);
			}
			foreach (var jumper in Setting.Data.ModuleLauncher.PinnedJumperConfiguration) {
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, Module.ModuleLauncher.JumperConfiguration.GenerateArgument(jumper)), "");
				item.Logo = new Uri("ms-appx:///Asset/Logo.png");
				item.GroupName = "Pinned";
				item.DisplayName = jumper.Title;
				item.Description = "";
				list.Items.Add(item);
			}
			foreach (var jumper in Setting.Data.ModuleLauncher.RecentJumperConfiguration) {
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, Module.ModuleLauncher.JumperConfiguration.GenerateArgument(jumper)), "");
				item.Logo = new Uri("ms-appx:///Asset/Logo.png");
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
			Module.ModuleLauncher.JumperConfiguration configuration
		) {
			var pinnedItem = Setting.Data.ModuleLauncher.PinnedJumperConfiguration.Find((value) => (Module.ModuleLauncher.JumperConfiguration.CompareForModule(value, configuration)));
			if (pinnedItem is not null) {
				return;
			}
			var recentItem = Setting.Data.ModuleLauncher.RecentJumperConfiguration.Find((value) => (Module.ModuleLauncher.JumperConfiguration.CompareForModule(value, configuration)));
			if (recentItem is not null) {
				Setting.Data.ModuleLauncher.RecentJumperConfiguration.Remove(recentItem);
				Setting.Data.ModuleLauncher.RecentJumperConfiguration.Insert(0, recentItem);
			}
			else {
				Setting.Data.ModuleLauncher.RecentJumperConfiguration.Insert(0, configuration);
			}
			Setting.Save();
			return;
		}

		#endregion

	}

	// Global-Function
	public static class GF {

		public static void AssertTest (
			[DoesNotReturnIf(false)] Boolean condition,
			String?                          message = null
		) {
			if (!condition) {
				throw new Exception("Assertion failed" + (message is null ? "" : $" : {message}"));
			}
			return;
		}

		public static void AssertFail (
			String? message = null
		) {
			GF.AssertTest(false, message);
			return;
		}

		// ----------------

		public static T ReturnSelf<T> (
			this T target
		) {
			return target;
		}

		// ----------------

		public static String ToString<T> (
			this T target
		)
			where T : notnull {
			return target.ToString().AsNotNull();
		}

		// ----------------

		public static Boolean IsNull<T> (
			[NotNullWhen(false)] this T? target
		) {
			return target is null;
		}

		public static Boolean NotNull<T> (
			[NotNullWhen(true)] this T? target
		) {
			return target is not null;
		}

		// ----------------

		// TODO
		public static T AsNotNullX<T> (
			[NotNull] this T? target
		) {
			return target ?? throw new NullReferenceException();
		}

		public static T AsNotNull<T> (
			[NotNull] this T? target
		)
			where T : struct {
			return target ?? throw new NullReferenceException();
		}

		public static T AsNotNull<T> (
			[NotNull] this T? target
		)
			where T : class {
			return target ?? throw new NullReferenceException();
		}

		// ----------------

		public static T AsStruct<T> (
			this Object? target
		)
			where T : struct {
			return target as T? ?? throw new NullReferenceException();
		}

		public static T AsClass<T> (
			this Object? target
		)
			where T : class {
			return target as T ?? throw new NullReferenceException();
		}

		// ----------------

		public static T? AsStructOrNull<T> (
			this Object? target
		)
			where T : struct {
			return target as T?;
		}

		public static T? AsClassOrNull<T> (
			this Object? target
		)
			where T : class {
			return target as T;
		}

		// ----------------

		public static ObservableCollection<TSource> ToObservableCollection<TSource> (
			this IEnumerable<TSource> source
		) {
			return new ObservableCollection<TSource>(source);
		}

	}

}
