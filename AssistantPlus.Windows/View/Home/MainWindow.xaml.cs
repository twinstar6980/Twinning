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
			this.SetTitleBar(this.uTab.TabStripFooter.AsClass<UIElement>());
			this.AppWindow.TitleBar.PreferredHeightOption = TitleBarHeightOption.Standard;
			this.Controller = new () { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		private MainWindowController Controller { get; }

		// ----------------

		public async Task SetDefaultView (
		) {
			await ControlHelper.WaitUntilLoaded(this.Content.AsClass<FrameworkElement>());
			await this.ShowLauncherPanel();
			return;
		}

		public async Task SetDefaultView (
			String       title,
			ModuleType   type,
			List<String> option
		) {
			await ControlHelper.WaitUntilLoaded(this.Content.AsClass<FrameworkElement>());
			await this.InsertTabItem(title, type, option);
			return;
		}

		// ----------------

		public async void PublishNotification (
			InfoBarSeverity severity,
			String          title,
			String          message,
			Size            duration = 4000
		) => this.Controller.PublishNotification(severity, title, message, duration);

		public async Task ShowLauncherPanel (
		) => await this.Controller.ShowLauncherPanel();

		public async Task InsertTabItem (
			String       title,
			ModuleType   type,
			List<String> option
		) => await this.Controller.InsertTabItem(title, type, option);

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

		public async void PublishNotification (
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
			GF.AssertTest(this.LauncherPanelFlyout is null);
			this.LauncherPanelFlyout = new Flyout() {
				Content = new LauncherPanel() {
					Stamp = new (),
				},
			}.ApplySelf((it) => {
				it.Closed += (_, _) => {
					this.LauncherPanelFlyout = null;
					return;
				};
			});
			this.LauncherPanelFlyout.ShowAt(this.View.Content.AsClass<FrameworkElement>(), new () { Placement = FlyoutPlacementMode.Full });
			return;
		}

		// ----------------

		public async Task InsertTabItem (
			String       title,
			ModuleType   type,
			List<String> option
		) {
			if (this.LauncherPanelFlyout is not null) {
				this.LauncherPanelFlyout.Hide();
			}
			var model = ModuleHelper.Query(type);
			var frame = new Frame() {
				ContentTransitions = [
					new NavigationThemeTransition() {
						DefaultNavigationTransitionInfo = new DrillInNavigationTransitionInfo(),
					},
				],
			};
			frame.NavigateToType(model.MainPage, option, new () { IsNavigationStackEnabled = false, TransitionInfoOverride = new DrillInNavigationTransitionInfo() });
			this.uTab_TabItemsSource.Add(new () { Host = this, Title = title, Type = type, Frame = frame });
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
			var state = await item.Frame.Content.AsClass<IModulePage>().ModulePageRequestClose();
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
			var senders = sender.AsClass<Button>();
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
			var senders = sender.AsClass<TabView>();
			await this.RemoveTabItem(args.Item.AsClass<MainWindowTabItemController>().Frame.Content.AsClass<Page>());
			return;
		}

		public async void uTab_AddTabButtonClick (
			TabView sender,
			Object  args
		) {
			var senders = sender.AsClass<TabView>();
			await this.ShowLauncherPanel();
			return;
		}

		// ----------------

		public async void uTabKeyboardAccelerator_Invoked (
			KeyboardAccelerator                 sender,
			KeyboardAcceleratorInvokedEventArgs args
		) {
			var senders = sender.AsClass<KeyboardAccelerator>();
			if (senders.Modifiers == VirtualKeyModifiers.Control && senders.Key == VirtualKey.W) {
				args.Handled = true;
				if (this.View.uTab.SelectedItem is null) {
					this.View.Close();
				}
				else {
					await this.RemoveTabItem(this.View.uTab.SelectedItem.AsClass<MainWindowTabItemController>().Frame.Content.AsClass<Page>());
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

		#endregion

	}

	public interface IModulePage {

		Task<Boolean> ModulePageRequestClose (
		);

	}

}
