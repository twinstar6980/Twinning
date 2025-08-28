#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Microsoft.UI.Xaml.Media;
using Windows.ApplicationModel;

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

		public static String ForwarderExtensionStateFile { get; private set; } = default!;

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
			return;
		}

		// ----------------

		protected override async void OnLaunched (
			LaunchActivatedEventArgs args
		) {
			var window = default(Window);
			try {
				ExceptionHelper.Initialize(this, this.HandleException);
				var argument = Environment.GetCommandLineArgs();
				App.PackageDirectory = StorageHelper.Parent(argument[0]).AsNotNull();
				App.ProgramFile = $"{App.PackageDirectory}/Application.exe";
				App.SharedDirectory = StorageHelper.Regularize($"{Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)}/{Package.Current.Id.Name}");
				App.CacheDirectory = $"{App.SharedDirectory}/Cache";
				App.ForwarderExtensionStateFile = $"{App.SharedDirectory}/Forwarder";
				argument = argument[1..];
				try {
					await App.Setting.Load();
				}
				catch (Exception) {
					await App.Setting.Reset();
				}
				await App.Setting.Save();
				unsafe {
					Win32.PInvoke.AddDllDirectory($"{App.PackageDirectory}/Asset/Library");
				}
				NotificationHelper.Initialize();
				App.MainWindow = new ();
				window = App.MainWindow;
				if (App.Setting.Data.WindowSizeState) {
					WindowHelper.SetSize(App.MainWindow, App.Setting.Data.WindowSizeWidth.CastPrimitive<Size>(), App.Setting.Data.WindowSizeHeight.CastPrimitive<Size>());
				}
				if (App.Setting.Data.WindowPositionState) {
					WindowHelper.SetPosition(App.MainWindow, App.Setting.Data.WindowPositionX.CastPrimitive<Size>(), App.Setting.Data.WindowPositionY.CastPrimitive<Size>());
				}
				else {
					WindowHelper.SetAsCenter(App.MainWindow);
				}
				_ = App.MainWindow.DispatcherQueue.EnqueueAsync(async () => {
					await ControlHelper.WaitUntilLoaded(App.MainWindow.Content.As<FrameworkElement>());
					if (argument.Length == 1 && argument[0].StartsWith("twinstar.twinning.assistant-plus:")) {
						await this.HandleLink(new (argument[0]));
					}
					else if (argument.Length >= 1 && argument[0] == "Application") {
						await this.HandleCommand(argument[1..].ToList());
					}
					else {
						await App.MainWindow.ShowLauncherPanel();
					}
				}).SelfLet(ExceptionHelper.WrapTask);
				await App.Setting.Apply();
			}
			catch (Exception e) {
				window = new () {
					SystemBackdrop = new MicaBackdrop(),
					Content = new Control.Box() {
						RequestedTheme = ElementTheme.Default,
						Padding = new (16),
						Children = {
							new TextBlock() {
								HorizontalAlignment = HorizontalAlignment.Center,
								VerticalAlignment = VerticalAlignment.Center,
								IsTextSelectionEnabled = true,
								TextWrapping = TextWrapping.Wrap,
								Text = ExceptionHelper.GenerateMessage(e),
							},
						},
					},
				};
				WindowHelper.SetTitleBar(window, true, null, false);
			}
			WindowHelper.SetIcon(window, $"{App.PackageDirectory}/Asset/Logo.ico");
			WindowHelper.SetTitle(window, Package.Current.DisplayName);
			WindowHelper.Activate(window);
			return;
		}

		#endregion

		#region utility

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

		// ----------------

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
							Text = ExceptionHelper.GenerateMessage(exception),
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

		public async Task HandleLaunch (
			String       title,
			ModuleType   type,
			List<String> option
		) {
			await ControlHelper.WaitUntilLoaded(App.MainWindow.Content.As<FrameworkElement>());
			await App.MainWindow.InsertTabItem(new () {
				Title = title,
				Type = type,
				Option = option,
				Command = [],
			});
			return;
		}

		public async Task HandleForward (
			List<String> resource
		) {
			var forwardOption = Enum.GetValues<ModuleType>().Select((value) => ModuleHelper.Query(value).GenerateForwardOption(resource)).ToList();
			var targetType = forwardOption[App.Setting.Data.ForwarderDefaultTarget.CastPrimitive<Size>()] != null ? App.Setting.Data.ForwarderDefaultTarget : null as ModuleType?;
			var canContinue = (App.Setting.Data.ForwarderImmediateJump && targetType != null) || (await ControlHelper.ShowDialogAsAutomatic(App.MainWindow.Content, "Forward", new ItemsRepeater() {
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				ItemsSource = Enum.GetValues<ModuleType>().Select((item) => new RadioButton() {
					HorizontalAlignment = HorizontalAlignment.Stretch,
					VerticalAlignment = VerticalAlignment.Stretch,
					IsEnabled = forwardOption[item.CastPrimitive<Size>()] != null,
					IsChecked = item == targetType,
					Content = ModuleHelper.Query(item).Name,
				}.SelfAlso((it) => {
					it.Click += (sender, args) => {
						targetType = item;
						return;
					};
				})).ToList(),
			}, new ("Cancel", "Continue", null)) == ContentDialogResult.Primary);
			if (canContinue && targetType != null) {
				await this.HandleLaunch(ModuleHelper.Query(targetType.AsNotNull()).Name, targetType.AsNotNull(), forwardOption[targetType.AsNotNull().CastPrimitive<Size>()].AsNotNull());
			}
			return;
		}

		public async Task HandleCommand (
			List<String> command
		) {
			var optionWindowPosition = default(Tuple<Integer, Integer>?);
			var optionWindowSize = default(Tuple<Integer, Integer>?);
			var optionLaunch = default(Tuple<String, ModuleType, List<String>>?);
			var optionForward = default(Tuple<List<String>>?);
			var option = new CommandLineReader(command);
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
				throw new ($"Too many option '{String.Join(' ', option.NextStringList())}'.");
			}
			if (optionWindowSize != null) {
				WindowHelper.SetSize(App.MainWindow, optionWindowSize.Item1.CastPrimitive<Size>(), optionWindowSize.Item2.CastPrimitive<Size>());
			}
			if (optionWindowPosition != null) {
				WindowHelper.SetPosition(App.MainWindow, optionWindowPosition.Item1.CastPrimitive<Size>(), optionWindowPosition.Item2.CastPrimitive<Size>());
			}
			if (optionWindowPosition == null && optionWindowSize != null) {
				WindowHelper.SetAsCenter(App.MainWindow);
			}
			if (optionLaunch != null) {
				await this.HandleLaunch(optionLaunch.Item1, optionLaunch.Item2, optionLaunch.Item3);
			}
			if (optionForward != null) {
				await this.HandleForward(optionForward.Item1);
			}
			return;
		}

		public async Task HandleLink (
			Uri link
		) {
			if (link.Scheme != "twinstar.twinning.assistant-plus" || link.Authority != "" || link.AbsolutePath != "/Application") {
				throw new ($"Invalid link.");
			}
			var command = link.GetComponents(UriComponents.Query, UriFormat.UriEscaped)
				.Split("&")
				.Where((item) => (item.Count((it) => (it == '=')) == 1))
				.Select((item) => (item.Split("=").Select(Uri.UnescapeDataString).ToList()))
				.Select((item) => (new KeyValuePair<String, String>(item[0], item[1])))
				.Where((item) => (item.Key == "Command"))
				.Select((item) => (item.Value))
				.ToList();
			await this.HandleCommand(command);
			return;
		}

		#endregion

	}

}
