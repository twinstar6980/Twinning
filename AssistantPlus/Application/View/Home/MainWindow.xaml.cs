#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.System;
using Windows.ApplicationModel;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media.Animation;

namespace AssistantPlus.View.Home {

	public partial class MainWindow : Window {

		#region life

		private MainWindowController Controller { get; }

		// ----------------

		public MainWindow (
		) {
			this.InitializeComponent();
			WindowHelper.SetIcon(this, $"{App.PackageDirectory}/Asset/Logo.ico");
			WindowHelper.SetTitle(this, Package.Current.DisplayName);
			WindowHelper.SetTitleBar(this, true, this.uTab.TabStripFooter.As<UIElement>(), false);
			this.Controller = new () { View = this };
			this.Controller.InitializeView();
			return;
		}

		#endregion

		#region action

		public async Task PushNotification (
			InfoBarSeverity severity,
			String          title,
			String          message,
			Size            duration = 4000
		) {
			await this.Controller.PushNotification(severity, title, message, duration);
			return;
		}

		public async Task ShowLauncherPanel (
		) {
			await this.Controller.ShowLauncherPanel();
			return;
		}

		public async Task InsertTabItem (
			ModuleLauncherConfiguration configuration
		) {
			await this.Controller.InsertTabItem(configuration);
			return;
		}

		public async Task RemoveTabItem (
			Page content
		) {
			await this.Controller.RemoveTabItem(content);
			return;
		}

		#endregion

	}

	public partial class MainWindowController : CustomController {

		#region data

		public MainWindow View { get; init; } = default!;

		// ----------------

		#endregion

		#region life

		public void InitializeView (
		) {
			return;
		}

		#endregion

		#region action

		public async Task PushNotification (
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

		public async Task InsertTabItem (
			ModuleLauncherConfiguration configuration
		) {
			if (this.View.uLauncherFlyout.IsOpen) {
				await this.HideLauncherPanel();
			}
			var model = ModuleHelper.Query(configuration.Type);
			var frame = new Frame() {
				ContentTransitions = [
					new NavigationThemeTransition() {
						DefaultNavigationTransitionInfo = new DrillInNavigationTransitionInfo(),
					},
				],
			};
			frame.NavigateToType(model.MainPage, null, new () { IsNavigationStackEnabled = false, TransitionInfoOverride = new DrillInNavigationTransitionInfo() });
			frame.Content.As<Page>().Tag = configuration.Option;
			this.uTab_TabItemsSource.Add(new () { Host = this, Title = configuration.Title, Type = configuration.Type, Frame = frame });
			this.NotifyPropertyChanged([
				nameof(this.uBlank_Visibility),
			]);
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
			var state = await item.Frame.Content.As<IModulePage>().ModulePageGetController().CloseView();
			if (!state) {
				this.View.uTab.SelectedItem = lastSelectedItem;
			}
			else {
				this.uTab_TabItemsSource.Remove(item);
				if (this.uTab_TabItemsSource.Count == 0) {
					await this.ShowLauncherPanel();
					await Task.Delay(400);
					this.NotifyPropertyChanged([
						nameof(this.uBlank_Visibility),
					]);
				}
			}
			return;
		}

		public async Task RenameTabItem (
			Page   content,
			String title
		) {
			var item = this.uTab_TabItemsSource.FirstOrDefault((value) => (Object.ReferenceEquals(value.Frame.Content, content))).AsNotNull();
			item.Title = title;
			item.NotifyPropertyChanged([
				nameof(item.uRoot_Header),
			]);
			return;
		}

		public async Task KeepTabItem (
			Page content
		) {
			var item = this.uTab_TabItemsSource.FirstOrDefault((value) => (Object.ReferenceEquals(value.Frame.Content, content))).AsNotNull();
			var configuration = new ModuleLauncherConfiguration() {
				Title = item.Title,
				Type = item.Type,
				Option = await item.Frame.Content.As<IModulePage>().ModulePageGetController().CollectOption(),
				Command = [],
			};
			App.Setting.Data.ModuleLauncher.Pinned.Add(configuration);
			await App.Setting.Save();
			return;
		}

		public async Task DuplicateTabItem (
			Page content
		) {
			var item = this.uTab_TabItemsSource.FirstOrDefault((value) => (Object.ReferenceEquals(value.Frame.Content, content))).AsNotNull();
			var configuration = new ModuleLauncherConfiguration() {
				Title = item.Title,
				Type = item.Type,
				Option = await item.Frame.Content.As<IModulePage>().ModulePageGetController().CollectOption(),
				Command = [],
			};
			await this.InsertTabItem(configuration);
			return;
		}

		// ----------------

		public async Task ShowLauncherPanel (
		) {
			GF.AssertTest(!this.View.uLauncherFlyout.IsOpen);
			this.NotifyPropertyChanged([
				nameof(this.uLauncher_Stamp),
			]);
			this.View.uLauncherFlyout.ShowAt(this.View.Content.As<FrameworkElement>());
			return;
		}

		public async Task HideLauncherPanel (
		) {
			GF.AssertTest(this.View.uLauncherFlyout.IsOpen);
			this.View.uLauncherFlyout.Hide();
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

		public async void uTab_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<TabView>();
			foreach (var item in args.RemovedItems.Cast<MainWindowTabItemController>()) {
				await item.Frame.Content.As<IModulePage>().ModulePageGetController().ExitView();
			}
			foreach (var item in args.AddedItems.Cast<MainWindowTabItemController>()) {
				await item.Frame.Content.As<IModulePage>().ModulePageGetController().EnterView();
			}
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

		#region launcher

		public async void uLauncher_PanelEnter (
		) {
			await this.ShowLauncherPanel();
		}

		public async void uLauncher_PanelExit (
		) {
			await this.HideLauncherPanel();
		}

		public UniqueStamp uLauncher_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		#endregion

		#region blank

		public Boolean uBlank_Visibility {
			get {
				return this.uTab_TabItemsSource.Count == 0;
			}
		}

		// ----------------

		public async void uBlankLauncher_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			await this.ShowLauncherPanel();
			return;
		}

		#endregion

	}

	public partial class MainWindowTabItemController : CustomController {

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
				case "Rename": {
					var title = "";
					var canContinue = await ControlHelper.ShowDialogAsAutomatic(App.MainWindow.Content, "Tab Rename", new TextBox() {
						HorizontalAlignment = HorizontalAlignment.Stretch,
						VerticalAlignment = VerticalAlignment.Stretch,
						TextWrapping = TextWrapping.NoWrap,
						AcceptsReturn = false,
						Text = "",
					}.SelfAlso((it) => {
						it.LostFocus += async (_, _) => {
							title = it.Text;
							return;
						};
					}), new ("Cancel", "Continue", null)) == ContentDialogResult.Primary;
					if (canContinue) {
						await this.Host.RenameTabItem(this.Frame.Content.As<Page>(), title);
					}
					break;
				}
				case "Keep": {
					await this.Host.KeepTabItem(this.Frame.Content.As<Page>());
					break;
				}
				case "Duplicate": {
					await this.Host.DuplicateTabItem(this.Frame.Content.As<Page>());
					break;
				}
				default: throw new UnreachableException();
			}
			return;
		}

		#endregion

	}

	public interface IModulePage {

		#region interface

		IModulePageController ModulePageGetController (
		);

		#endregion

	}

	public interface IModulePageController {

		#region life

		void InitializeView (
		);

		// ----------------

		Task OpenView (
		);

		Task<Boolean> CloseView (
		);

		// ----------------

		Task EnterView (
		);

		Task ExitView (
		);

		// ----------------

		Task ApplyOption (
			List<String> optionView
		);

		Task<List<String>> CollectOption (
		);

		#endregion

	}

}
