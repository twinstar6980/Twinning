#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.System;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media.Animation;

namespace AssistantPlus.View.Home {

	public partial class MainWindow : Window {

		#region life

		public MainWindow (
		) {
			this.InitializeComponent();
			this.ExtendsContentIntoTitleBar = true;
			this.SetTitleBar(this.uTab.TabStripFooter.As<UIElement>());
			this.AppWindow.TitleBar.PreferredHeightOption = TitleBarHeightOption.Standard;
			this.Controller = new () { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		private MainWindowController Controller { get; }

		// ----------------

		public async Task SetDefaultView (
		) {
			await ControlHelper.WaitUntilLoaded(this.Content.As<FrameworkElement>());
			await this.ShowLauncherPanel();
			return;
		}

		public async Task SetDefaultView (
			ModuleLauncherConfiguration configuration
		) {
			await ControlHelper.WaitUntilLoaded(this.Content.As<FrameworkElement>());
			await this.InsertTabItem(configuration);
			return;
		}

		// ----------------

		public async void PushNotification (
			InfoBarSeverity severity,
			String          title,
			String          message,
			Size            duration = 4000
		) => this.Controller.PushNotification(severity, title, message, duration);

		public async Task ShowLauncherPanel (
		) => await this.Controller.ShowLauncherPanel();

		public async Task InsertTabItem (
			ModuleLauncherConfiguration configuration
		) => await this.Controller.InsertTabItem(configuration);

		public async Task RemoveTabItem (
			Page content
		) => await this.Controller.RemoveTabItem(content);

		#endregion

	}

	public class MainWindowController : CustomController {

		#region data

		public MainWindow View { get; init; } = default!;

		// ----------------

		public Flyout? LauncherPanelFlyout { get; set; } = null;

		#endregion

		#region initialize

		public void Initialize (
		) {
			return;
		}

		// ----------------

		public async void PushNotification (
			InfoBarSeverity severity,
			String          title,
			String          message,
			Size            duration = 4000
		) {
			this.View.uNotificationsBehavior.Show(new () {
				Severity = severity,
				Title = title,
				Message = message,
				Duration = TimeSpan.FromMilliseconds(duration),
			});
			return;
		}

		// ----------------

		public async Task ShowLauncherPanel (
		) {
			GF.AssertTest(this.LauncherPanelFlyout == null);
			this.LauncherPanelFlyout = new Flyout() {
				FlyoutPresenterStyle = this.View.Content.As<FrameworkElement>().FindResource("VerticalScrollFlyoutPresenterStyle").As<Style>(),
				Content = new LauncherPanel() {
					Stamp = new (),
				},
			}.SelfAlso((it) => {
				it.Closed += (_, _) => {
					this.LauncherPanelFlyout = null;
					return;
				};
			});
			this.LauncherPanelFlyout.ShowAt(this.View.Content.As<FrameworkElement>(), new () { Placement = FlyoutPlacementMode.Full });
			return;
		}

		// ----------------

		public async Task InsertTabItem (
			ModuleLauncherConfiguration configuration
		) {
			if (this.LauncherPanelFlyout != null) {
				this.LauncherPanelFlyout.Hide();
			}
			var model = ModuleHelper.Query(configuration.Type);
			var frame = new Frame() {
				ContentTransitions = [
					new NavigationThemeTransition() {
						DefaultNavigationTransitionInfo = new DrillInNavigationTransitionInfo(),
					},
				],
			};
			frame.NavigateToType(model.MainPage, configuration.Option, new () { IsNavigationStackEnabled = false, TransitionInfoOverride = new DrillInNavigationTransitionInfo() });
			this.uTab_TabItemsSource.Add(new () { Host = this, Title = configuration.Title, Type = configuration.Type, Frame = frame });
			this.NotifyPropertyChanged(
				nameof(this.uBlank_Visibility)
			);
			await Task.Delay(40);
			this.View.uTab.SelectedItem = this.uTab_TabItemsSource.Last();
			this.View.uTab.CloseButtonOverlayMode = TabViewCloseButtonOverlayMode.OnPointerOver;
			return;
		}

		public async Task RemoveTabItem (
			Page content
		) {
			var item = this.uTab_TabItemsSource.FirstOrDefault((value) => (Object.ReferenceEquals(value.Frame.Content, content))).AsNotNull();
			var lastSelectedItem = this.View.uTab.SelectedItem;
			this.View.uTab.SelectedItem = item;
			var state = await item.Frame.Content.As<IModulePage>().ModulePageRequestClose();
			if (!state) {
				this.View.uTab.SelectedItem = lastSelectedItem;
			}
			else {
				this.uTab_TabItemsSource.Remove(item);
				if (this.uTab_TabItemsSource.Count == 0) {
					await this.ShowLauncherPanel();
					await Task.Delay(400);
					this.NotifyPropertyChanged(
						nameof(this.uBlank_Visibility)
					);
				}
			}
			return;
		}

		#endregion

		#region blank

		public Boolean uBlank_Visibility {
			get {
				return this.uTab_TabItemsSource.Count == 0;
			}
		}

		// ----------------

		public async void uBlankLaunch_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await this.ShowLauncherPanel();
			return;
		}

		#endregion

		#region tab

		public ObservableCollection<MainWindowTabItemController> uTab_TabItemsSource { get; } = [];

		public async void uTab_TabCloseRequested (
			TabView                           sender,
			TabViewTabCloseRequestedEventArgs args
		) {
			var senders = sender.As<TabView>();
			await this.RemoveTabItem(args.Item.As<MainWindowTabItemController>().Frame.Content.As<Page>());
			return;
		}

		public async void uTab_AddTabButtonClick (
			TabView sender,
			Object  args
		) {
			var senders = sender.As<TabView>();
			await this.ShowLauncherPanel();
			return;
		}

		// ----------------

		public async void uTabKeyboardAccelerator_Invoked (
			KeyboardAccelerator                 sender,
			KeyboardAcceleratorInvokedEventArgs args
		) {
			var senders = sender.As<KeyboardAccelerator>();
			if (senders.Modifiers == VirtualKeyModifiers.Control && senders.Key == VirtualKey.W) {
				args.Handled = true;
				if (this.View.uTab.SelectedItem == null) {
					this.View.Close();
				}
				else {
					await this.RemoveTabItem(this.View.uTab.SelectedItem.As<MainWindowTabItemController>().Frame.Content.As<Page>());
				}
			}
			if (senders.Modifiers == VirtualKeyModifiers.Control && senders.Key == VirtualKey.T) {
				args.Handled = true;
				await this.ShowLauncherPanel();
			}
			return;
		}

		#endregion

	}

	public class MainWindowTabItemController : CustomController {

		#region data

		public MainWindowController Host { get; init; } = default!;

		// ----------------

		public String Title { get; set; } = default!;

		public ModuleType Type { get; set; } = default!;

		public Frame Frame { get; set; } = default!;

		#endregion

		#region view

		public FontIconSource uRoot_IconSource {
			get {
				return new () { Glyph = ModuleHelper.Query(this.Type).Icon };
			}
		}

		public String uRoot_Header {
			get {
				return this.Title;
			}
		}

		public Frame uRoot_Content {
			get {
				return this.Frame;
			}
		}

		// ----------------

		public async void uCommand_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			switch (senders.Tag.As<String>()) {
				case "Keep": {
					var configuration = new ModuleLauncherConfiguration() {
						Title = "Untitled",
						Type = this.Type,
						Option = await this.Frame.Content.As<IModulePage>().ModulePageCollectOption(),
						Command = [],
					};
					await ControlHelper.ShowDialogAsFixed(this.Host.View.Content, "Launcher Configuration", new LauncherConfigurationPanel() {
						Data = configuration,
						Stamp = UniqueStamp.Create(),
					}, null);
					await App.Instance.AppendPinnedLauncherItem(configuration);
					await App.Setting.Save();
					break;
				}
				case "Duplicate": {
					var configuration = new ModuleLauncherConfiguration() {
						Title = this.Title,
						Type = this.Type,
						Option = await this.Frame.Content.As<IModulePage>().ModulePageCollectOption(),
						Command = [],
					};
					await this.Host.InsertTabItem(configuration);
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

	}

	public interface IModulePage {

		Task ModulePageApplyOption (
			List<String> optionView
		);

		Task<List<String>> ModulePageCollectOption (
		);

		Task<Boolean> ModulePageRequestClose (
		);

	}

	public interface IModulePageController {

		void Initialize (
		);

		Task ApplyOption (
			List<String> optionView
		);

		Task<List<String>> CollectOption (
		);

		Task<Boolean> RequestClose (
		);

	}

}
