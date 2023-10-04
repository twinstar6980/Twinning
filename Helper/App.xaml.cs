#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;
using Windows.UI.StartScreen;
using Microsoft.Windows.AppNotifications;

namespace Helper {

	public partial class App : Application {

		#region instance

		public static App Instance { get; set; } = default!;

		public static Module.MainWindow MainWindow { get; set; } = default!;

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
				Setting.Initialize();
				this.RegisterTaskJumpList().Wait(0);
				this.InitializeNotification();
				var optionWindowPosition = default(Tuple<Integer, Integer>?);
				var optionWindowSize = default(Tuple<Integer, Integer>?);
				var optionWindowAlwaysOnTop = default(Boolean?);
				var optionModuleType = default(Module.ModuleType?);
				var optionModuleOption = default(List<String>?);
				{
					var option = new CommandLineReader(Environment.GetCommandLineArgs()[1..].ToList());
					if (option.Ensure("----AppNotificationActivated:")) {
						// skip if launch by AppNotification
					}
					if (option.Ensure("-Embedding")) {
						// skip if launch by AppNotification
					}
					if (option.Ensure("-WindowPosition")) {
						optionWindowPosition = new Tuple<Integer, Integer>(
							option.NextInteger(),
							option.NextInteger()
						);
					}
					if (option.Ensure("-WindowSize")) {
						optionWindowSize = new Tuple<Integer, Integer>(
							option.NextInteger(),
							option.NextInteger()
						);
					}
					if (option.Ensure("-WindowAlwaysOnTop")) {
						optionWindowAlwaysOnTop = option.NextBoolean();
					}
					if (option.Ensure("-ModuleType")) {
						optionModuleType = option.NextEnumeration<Module.ModuleType>();
					}
					if (option.Ensure("-ModuleOption")) {
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
				(window as Module.MainWindow)!.Controller.InsertTabItem(optionModuleType ?? Module.ModuleType.ApplicationSetting, optionModuleOption ?? null).Wait(0);
				App.MainWindow = (window as Module.MainWindow)!;
			} catch (Exception e) {
				window = new Window();
				window.Content = new MBox() {
					Padding = new Thickness(16),
					Children = {
						new TextBlock() {
							HorizontalAlignment = HorizontalAlignment.Center,
							VerticalAlignment = VerticalAlignment.Center,
							TextWrapping = TextWrapping.Wrap,
							Text = e.ToString(),
						},
					},
				};
			}
			WindowHelper.Track(window);
			WindowHelper.Title(window, "TwinStar ToolKit - Helper");
			WindowHelper.Icon(window, $"{StorageHelper.GetPathParent(Environment.GetCommandLineArgs()[0])}/Asset/Logo.ico");
			Setting.AppearanceThemeMode = Setting.AppearanceThemeMode;
			WindowHelper.ApplyMicaBackdrop(window);
			WindowHelper.Activate(window);
			return;
		}

		// ----------------

		protected void OnUnhandledException (
			Object                                        sender,
			Microsoft.UI.Xaml.UnhandledExceptionEventArgs args
		) {
			try {
				if (App.MainWindow is not null) {
					_ = new ContentDialog() {
						XamlRoot = App.MainWindow.Content.XamlRoot,
						Title = "Unhandled Exception",
						Content = args.Exception.ToString(),
						CloseButtonText = "Cancel",
						DefaultButton = ContentDialogButton.Close,
					}.ShowAsync();
					args.Handled = true;
				}
			} catch (Exception) {
				// ignored
			}
			return;
		}

		// ----------------

		private AppNotificationManager AppNotificationManager = default!;

		private void NotificationManager_OnNotificationInvoked (
			AppNotificationManager            sender,
			AppNotificationActivatedEventArgs args
		) {
			Debug.WriteLine("NotificationManager_OnNotificationInvoked");
			if (App.MainWindow is not null) {
				WindowHelper.ShowAsForeground(App.MainWindow);
			}
			return;
		}

		private void InitializeNotification (
		) {
			this.AppNotificationManager = AppNotificationManager.Default;
			this.AppNotificationManager.NotificationInvoked += this.NotificationManager_OnNotificationInvoked;
			this.AppNotificationManager.Register();
			return;
		}

		public void PushNotification (
			AppNotification notification
		) {
			this.AppNotificationManager.Show(notification);
			return;
		}

		// ----------------

		public async Task RegisterTaskJumpList (
		) {
			if (!JumpList.IsSupported()) {
				return;
			}
			var list = await JumpList.LoadCurrentAsync();
			for (var index = 0; index < list.Items.Count;) {
				var item = list.Items[index];
				if (item.GroupName == "") {
					list.Items.RemoveAt(index);
				} else {
					index++;
				}
			}
			foreach (var module in Module.ModuleInformationConstant.Value) {
				var item = JumpListItem.CreateWithArguments(ProcessHelper.EncodeCommandLineString(null, new List<String>() { "-ModuleType", module.Type.ToString() }), "");
				item.Logo = new Uri("ms-appx:///Asset/Logo.png");
				item.GroupName = "";
				item.DisplayName = module.Title;
				item.Description = "";
				list.Items.Add(item);
			}
			await list.SaveAsync();
			return;
		}

		public async Task AppendRecentJumpList (
			String       title,
			List<String> argument
		) {
			if (!JumpList.IsSupported()) {
				return;
			}
			var argumentString = ProcessHelper.EncodeCommandLineString(null, argument);
			var list = await JumpList.LoadCurrentAsync();
			for (var index = 0; index < list.Items.Count;) {
				var item = list.Items[index];
				if (item.GroupName == "Recent" && item.Arguments == argumentString) {
					list.Items.RemoveAt(index);
				} else {
					index++;
				}
			}
			{
				var item = JumpListItem.CreateWithArguments(argumentString, "");
				item.Logo = new Uri($"ms-appx:///Asset/Logo.png");
				item.GroupName = "Recent";
				item.DisplayName = title;
				item.Description = argumentString;
				list.Items.Insert(0, item);
			}
			await list.SaveAsync();
			return;
		}

		#endregion

	}

}
