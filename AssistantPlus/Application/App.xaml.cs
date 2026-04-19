#pragma warning disable 0,

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Microsoft.UI.Xaml.Media;
using Windows.ApplicationModel;

namespace Twinning.AssistantPlus {

	public partial class App : Application {

		#region singleton

		public static App Instance { get; private set; } = default!;

		#endregion

		#region life

		public StoragePath PackageDirectory { get; private set; }

		public StoragePath ProgramFile { get; private set; }

		public StoragePath SharedDirectory { get; private set; }

		public StoragePath CacheDirectory { get; private set; }

		public SettingProvider Setting { get; }

		public View.Home.MainWindow MainWindow { get; private set; }

		public Boolean MainWindowIsInitialized {
			get {
				// ReSharper disable once ConditionIsAlwaysTrueOrFalseAccordingToNullableAPIContract
				return this.MainWindow != null;
			}
		}

		// ----------------

		public App(
		) {
			// ReSharper disable once ConditionIsAlwaysTrueOrFalseAccordingToNullableAPIContract
			AssertTest(App.Instance == null);
			App.Instance = this;
			this.PackageDirectory = null!;
			this.ProgramFile = null!;
			this.SharedDirectory = null!;
			this.CacheDirectory = null!;
			this.Setting = new ();
			this.MainWindow = null!;
			this.InitializeComponent();
			return;
		}

		// ----------------

		protected override async void OnLaunched(
			LaunchActivatedEventArgs args
		) {
			try {
				var argument = Environment.GetCommandLineArgs()[1..];
				this.PackageDirectory = await StorageHelper.Query(StorageQueryType.ApplicationPackage);
				this.ProgramFile = this.PackageDirectory.Join("Application.exe");
				this.SharedDirectory = await StorageHelper.Query(StorageQueryType.ApplicationShared);
				this.CacheDirectory = this.SharedDirectory.Join("cache");
				await ApplicationExceptionManager.Instance.Initialize(this);
				await ApplicationExceptionManager.Instance.Listen(async (exception) => {
					await this.HandleException(exception, this.MainWindow);
					return;
				});
				var needShowOnboarding = false;
				try {
					await this.Setting.Load();
				}
				catch (Exception) {
					needShowOnboarding = true;
				}
				await this.Setting.Save(apply: false);
				await ApplicationNotificationManager.Instance.Initialize();
				await ApplicationJumpListManager.Instance.Initialize();
				this.MainWindow = new ();
				if (this.Setting.Data.WindowSizeState) {
					WindowHelper.SetSize(this.MainWindow, this.Setting.Data.WindowSizeWidth.CastPrimitive<Size>(), this.Setting.Data.WindowSizeHeight.CastPrimitive<Size>());
				}
				if (this.Setting.Data.WindowPositionState) {
					WindowHelper.SetPosition(this.MainWindow, this.Setting.Data.WindowPositionX.CastPrimitive<Size>(), this.Setting.Data.WindowPositionY.CastPrimitive<Size>());
				}
				else {
					WindowHelper.SetAtCenter(this.MainWindow);
				}
				_ = this.MainWindow.DispatcherQueue.EnqueueAsync(async () => {
					await ControlHelper.PostTask(this.MainWindow.Content.As<FrameworkElement>(), async () => {
						if (needShowOnboarding) {
							await this.MainWindow.ShowOnboarding();
						}
						await ApplicationNotificationManager.Instance.Listen(async () => {
							WindowHelper.SetAsForeground(this.MainWindow);
							return;
						});
						if (argument.Length == 1 && argument[0].StartsWith($"{ApplicationInformation.Identifier}:")) {
							await this.HandleLink(new (argument[0]));
						}
						else if (argument.Length >= 1 && argument[0] == "application") {
							await this.HandleCommand(argument[1..].ToList());
						}
						else {
							await this.MainWindow.ShowLauncher();
						}
					});
				}).SelfLet(ApplicationExceptionManager.Instance.WrapTask);
				await this.Setting.Apply();
				WindowHelper.Activate(this.MainWindow);
			}
			catch (Exception e) {
				_ = this.HandleExceptionFatal(e);
			}
			return;
		}

		#endregion

		#region utility

		public async Task AppendRecentLauncherItem(
			ModuleLauncherConfiguration launcher
		) {
			var pinnedItem = this.Setting.Data.ModuleLauncher.Pinned.Find((value) => ModuleHelper.CompareLauncher(value, launcher));
			if (pinnedItem != null) {
				return;
			}
			var recentItem = this.Setting.Data.ModuleLauncher.Recent.Find((value) => ModuleHelper.CompareLauncher(value, launcher));
			if (recentItem != null) {
				this.Setting.Data.ModuleLauncher.Recent.Remove(recentItem);
				this.Setting.Data.ModuleLauncher.Recent.Insert(0, recentItem);
			}
			else {
				this.Setting.Data.ModuleLauncher.Recent.Insert(0, launcher);
			}
			await this.Setting.Save();
			return;
		}

		public async Task ExecuteLauncher(
			ModuleLauncherConfiguration launcher,
			Boolean                     forNewWindow
		) {
			if (!forNewWindow) {
				await this.MainWindow.InsertPage(launcher);
			}
			else {
				await ProcessHelper.RunProcess(this.ProgramFile, ModuleHelper.GenerateArgument(launcher), null, null, false);
			}
			return;
		}

		// ----------------

		private async Task HandleException(
			Exception exception,
			Window?   window
		) {
			try {
				if (window != null) {
					await window.DispatcherQueue.EnqueueAsync(async () => {
						await ControlHelper.PostTask(window.Content.As<FrameworkElement>(), async () => {
							await ControlHelper.ShowDialogAsAutomatic(window.Content.As<FrameworkElement>(), "Unhandled Exception", new TextBlock() {
								HorizontalAlignment = HorizontalAlignment.Stretch,
								VerticalAlignment = VerticalAlignment.Stretch,
								Style = window.Content.As<FrameworkElement>().FindResource("BodyTextBlockStyle").As<Style>(),
								IsTextSelectionEnabled = true,
								TextWrapping = TextWrapping.Wrap,
								Text = ConvertHelper.GenerateExceptionMessage(exception),
							}, null);
						});
					});
				}
			}
			catch (Exception) {
				// ignored
			}
			return;
		}

		private async Task HandleExceptionFatal(
			Exception exception
		) {
			try {
				var window = new Window() {
					SystemBackdrop = new MicaBackdrop(),
					Content = new Grid(),
				};
				window.Closed += async (_, _) => {
					// if the user close the window externally, the dialog task will not complete, so put the step to close MainWindow in the Closed event
					if (App.Instance.MainWindowIsInitialized) {
						WindowHelper.Close(this.MainWindow);
					}
					return;
				};
				WindowHelper.SetIcon(window, this.PackageDirectory.Join("Asset").Join("Logo.ico").EmitNative());
				WindowHelper.SetTitle(window, ApplicationInformation.Name);
				WindowHelper.SetTitleBar(window, true, null, false);
				WindowHelper.Activate(window);
				await this.HandleException(exception, window);
				WindowHelper.Close(window);
			}
			catch (Exception) {
				// ignored
			}
			return;
		}

		// ----------------

		public async Task HandleLaunch(
			String       title,
			ModuleType   type,
			List<String> option
		) {
			await this.MainWindow.InsertPage(new () {
				Title = title,
				Type = type,
				Option = option,
				Command = [],
			});
			return;
		}

		public async Task HandleForward(
			List<StoragePath> resource
		) {
			var forwardOption = (await Task.WhenAll(Enum.GetValues<ModuleType>().Select(async (value) => await ModuleHelper.Query(value).GenerateForwardOption(resource)))).ToList();
			var targetType = forwardOption[this.Setting.Data.ForwarderDefaultTarget.CastPrimitive<Size>()] != null ? this.Setting.Data.ForwarderDefaultTarget : null as ModuleType?;
			var canContinue = this.Setting.Data.ForwarderImmediateJump && targetType != null;
			if (!canContinue) {
				canContinue = await ControlHelper.ShowDialogAsAutomatic(this.MainWindow.Content.As<FrameworkElement>(), "Forward", new ItemsRepeater() {
					HorizontalAlignment = HorizontalAlignment.Stretch,
					VerticalAlignment = VerticalAlignment.Stretch,
					ItemsSource = Enum.GetValues<ModuleType>().Select((item) => new RadioButton() {
						HorizontalAlignment = HorizontalAlignment.Stretch,
						VerticalAlignment = VerticalAlignment.Stretch,
						IsEnabled = forwardOption[item.CastPrimitive<Size>()] != null,
						IsChecked = item == targetType,
						Content = ModuleHelper.Query(item).Name,
					}.SelfAlso((it) => {
						it.Click += async (_, _) => {
							targetType = item;
							return;
						};
					})).ToList(),
				}, new ("Cancel", "Continue", null)) == ContentDialogResult.Primary;
			}
			if (canContinue && targetType != null) {
				await this.HandleLaunch(ModuleHelper.Query(targetType.AsNotNull()).Name, targetType.AsNotNull(), forwardOption[targetType.AsNotNull().CastPrimitive<Size>()].AsNotNull());
			}
			return;
		}

		public async Task HandleCommand(
			List<String> command
		) {
			var optionWindowPosition = default(Tuple<Integer, Integer>?);
			var optionWindowSize = default(Tuple<Integer, Integer>?);
			var optionLaunch = default(Tuple<String, ModuleType, List<String>>?);
			var optionForward = default(Tuple<List<StoragePath>>?);
			var option = new CommandLineReader(command);
			if (option.Check("-window_position")) {
				optionWindowPosition = new (
					option.NextInteger(),
					option.NextInteger()
				);
			}
			if (option.Check("-window_size")) {
				optionWindowSize = new (
					option.NextInteger(),
					option.NextInteger()
				);
			}
			if (option.Check("-launch")) {
				optionLaunch = new (
					option.NextString(),
					option.NextString().SelfLet((it) => ConvertHelper.ParseEnumerationFromStringOfSnakeCase<ModuleType>(it)),
					option.NextStringList()
				);
			}
			if (option.Check("-forward")) {
				optionForward = new (
					option.NextStringList().Select((it) => new StoragePath(it)).ToList()
				);
			}
			if (!option.Done()) {
				throw new ($"Too many option '{String.Join(' ', option.NextStringList())}'.");
			}
			if (optionWindowSize != null) {
				WindowHelper.SetSize(this.MainWindow, optionWindowSize.Item1.CastPrimitive<Size>(), optionWindowSize.Item2.CastPrimitive<Size>());
			}
			if (optionWindowPosition != null) {
				WindowHelper.SetPosition(this.MainWindow, optionWindowPosition.Item1.CastPrimitive<Size>(), optionWindowPosition.Item2.CastPrimitive<Size>());
			}
			if (optionWindowPosition == null && optionWindowSize != null) {
				WindowHelper.SetAtCenter(this.MainWindow);
			}
			if (optionLaunch != null) {
				await this.HandleLaunch(optionLaunch.Item1, optionLaunch.Item2, optionLaunch.Item3);
			}
			if (optionForward != null) {
				await this.HandleForward(optionForward.Item1);
			}
			return;
		}

		public async Task HandleLink(
			Uri link
		) {
			if (link.Scheme != ApplicationInformation.Identifier || link.Authority != "" || link.AbsolutePath != "/application") {
				throw new ($"Invalid link.");
			}
			var command = link.GetComponents(UriComponents.Query, UriFormat.UriEscaped)
				.Split("&")
				.Where((item) => item.Count((it) => it == '=') == 1)
				.Select((item) => item.Split("=").Select(Uri.UnescapeDataString).ToList())
				.Where((item) => item[0] == "command")
				.Select((item) => item[1])
				.ToList();
			await this.HandleCommand(command);
			return;
		}

		#endregion

	}

}
