#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Media.Animation;
using Windows.Foundation;
using Microsoft.UI.Xaml.Input;
using FluentIconGlyph = Helper.CommonControl.FluentIconGlyph;

namespace Helper.Module {

	public partial class MainWindow : Window {

		#region life

		public MainWindow (
		) {
			this.InitializeComponent();
			this.ExtendsContentIntoTitleBar = true;
			this.SetTitleBar(this.uTab.TabStripFooter.AsClass<UIElement>());
			this.Controller = new MainWindowController() { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		private MainWindowController Controller { get; }

		// ----------------

		public async Task InsertTabItem (
			ModuleType   type,
			List<String> option
		) => await this.Controller.InsertTabItem(type, option);

		public async Task RemoveTabItem (
			Page content
		) => await this.Controller.RemoveTabItem(content);

		public async void PublishTip (
			InfoBarSeverity severity,
			String          title,
			String          message,
			Size            duration = 4000
		) => this.Controller.PublishTip(severity, title, message, duration);

		#endregion

	}

	public class MainWindowController : CustomController {

		#region data

		public MainWindow View { get; init; } = default!;

		#endregion

		#region initialize

		public void Initialize (
		) {
			return;
		}

		// ----------------

		public async Task InsertTabItem (
			ModuleType   type,
			List<String> option
		) {
			var model = ModuleInformationConstant.Query(type);
			var frame = new Frame() {
				ContentTransitions = new TransitionCollection() {
					new NavigationThemeTransition() {
						DefaultNavigationTransitionInfo = new DrillInNavigationTransitionInfo(),
					},
				},
			};
			frame.Navigate(model.Page, option);
			this.uTab_TabItemsSource.Add(new MainWindowTabItemController() { Host = this, Model = model, Frame = frame });
			await Task.Delay(40);
			this.View.uTab.SelectedItem = this.uTab_TabItemsSource.Last();
			return;
		}

		public async Task RemoveTabItem (
			MainWindowTabItemController item
		) {
			var lastSelectedItem = this.View.uTab.SelectedItem;
			this.View.uTab.SelectedItem = item;
			var state = await item.Frame.Content.AsClass<ITabItemPage>().TabItemPageRequestClose();
			if (!state) {
				this.View.uTab.SelectedItem = lastSelectedItem;
			}
			else {
				this.uTab_TabItemsSource.Remove(item);
				if (this.uTab_TabItemsSource.Count == 0) {
					this.View.Close();
				}
			}
			return;
		}

		public async Task RemoveTabItem (
			Page content
		) {
			var item = this.uTab_TabItemsSource.FirstOrDefault((value) => (Object.ReferenceEquals(value.Frame.Content, content)));
			if (item is not null) {
				await this.RemoveTabItem(item);
			}
			return;
		}

		// ----------------

		public async void PublishTip (
			InfoBarSeverity severity,
			String          title,
			String          message,
			Size            duration = 4000
		) {
			this.uTip_IsOpen = false;
			this.uTip_Severity = severity;
			this.uTip_Title = title;
			this.uTip_Message = message;
			this.NotifyPropertyChanged(
				nameof(this.uTip_IsOpen),
				nameof(this.uTip_Severity),
				nameof(this.uTip_Title),
				nameof(this.uTip_Message)
			);
			await Task.Delay(80);
			this.uTip_IsOpen = true;
			this.NotifyPropertyChanged(
				nameof(this.uTip_IsOpen)
			);
			this.uTip_vDelayCount++;
			await Task.Delay(duration);
			this.uTip_vDelayCount--;
			if (this.uTip_vDelayCount == 0) {
				this.uTip_IsOpen = false;
				this.NotifyPropertyChanged(
					nameof(this.uTip_IsOpen)
				);
			}
			return;
		}

		#endregion

		#region tab

		public ObservableCollection<MainWindowTabItemController> uTab_TabItemsSource { get; } = new ObservableCollection<MainWindowTabItemController>();

		public async void uTab_TabCloseRequested (
			TabView                           sender,
			TabViewTabCloseRequestedEventArgs args
		) {
			var senders = sender.AsClass<TabView>();
			await this.RemoveTabItem(args.Item.AsClass<MainWindowTabItemController>());
			return;
		}

		public async void uTab_AddTabButtonClick (
			TabView sender,
			Object  args
		) {
			var senders = sender.AsClass<TabView>();
			var menu = new MenuFlyout();
			foreach (var jumper in Setting.Data.ModuleLauncher.ModuleJumperConfiguration) {
				var module = ModuleInformationConstant.Query(jumper.ModuleType);
				var menuItem = new MenuFlyoutItem() {
					Icon = new FontIcon() { Glyph = module.Icon },
					Text = jumper.Title,
				};
				menuItem.Click += async (_, _) => {
					await this.InsertTabItem(jumper.ModuleType, jumper.ModuleOption);
					return;
				};
				menu.Items.Add(menuItem);
			}
			menu.Items.Add(new MenuFlyoutSeparator());
			var pinnedList = new MenuFlyoutSubItem() {
				Icon = new FontIcon() { Glyph = FluentIconGlyph.Pinned },
				Text = "Pinned",
			};
			foreach (var jumper in Setting.Data.ModuleLauncher.PinnedJumperConfiguration) {
				var module = ModuleInformationConstant.Query(jumper.ModuleType);
				var menuItem = new MenuFlyoutItem() {
					Icon = new FontIcon() { Glyph = module.Icon },
					Text = jumper.Title,
				};
				menuItem.Click += async (_, _) => {
					await this.InsertTabItem(jumper.ModuleType, jumper.ModuleOption);
					return;
				};
				pinnedList.Items.Add(menuItem);
			}
			menu.Items.Add(pinnedList);
			var recentList = new MenuFlyoutSubItem() {
				Icon = new FontIcon() { Glyph = FluentIconGlyph.Recent },
				Text = "Recent",
			};
			foreach (var jumper in Setting.Data.ModuleLauncher.RecentJumperConfiguration) {
				var module = ModuleInformationConstant.Query(jumper.ModuleType);
				var menuItem = new MenuFlyoutItem() {
					Icon = new FontIcon() { Glyph = module.Icon },
					Text = jumper.Title,
				};
				menuItem.Click += async (_, _) => {
					await this.InsertTabItem(jumper.ModuleType, jumper.ModuleOption);
					return;
				};
				recentList.Items.Add(menuItem);
			}
			menu.Items.Add(recentList);
			menu.ShowAt(sender.TabStripFooter.AsClass<UIElement>(), new FlyoutShowOptions() {
				Placement = FlyoutPlacementMode.BottomEdgeAlignedLeft,
				Position = new Point(-40.0, +40.0),
			});
			return;
		}

		// ----------------

		public async void uTabKeyboardControlW_OnInvoked (
			KeyboardAccelerator                 sender,
			KeyboardAcceleratorInvokedEventArgs args
		) {
			var senders = sender.AsClass<KeyboardAccelerator>();
			args.Handled = true;
			await this.RemoveTabItem(this.View.uTab.SelectedItem.AsClass<MainWindowTabItemController>());
			return;
		}

		#endregion

		#region tip

		public Boolean uTip_IsOpen { get; set; } = false;

		public InfoBarSeverity uTip_Severity { get; set; } = InfoBarSeverity.Informational;

		public String uTip_Title { get; set; } = "";

		public String uTip_Message { get; set; } = "";

		// ----------------

		public Size uTip_vDelayCount { get; set; } = 0;

		#endregion

	}

	public class MainWindowTabItemController : CustomController {

		#region data

		public MainWindowController Host { get; init; } = default!;

		// ----------------

		public ModuleInformation Model { get; set; } = default!;

		public Frame Frame { get; set; } = default!;

		#endregion

		#region view

		public String uRoot_Header {
			get {
				return this.Model.Name;
			}
		}

		public FontIconSource uRoot_IconSource {
			get {
				return new FontIconSource() { Glyph = this.Model.Icon };
			}
		}

		public Frame uRoot_Content {
			get {
				return this.Frame;
			}
		}

		#endregion

	}

	public interface ITabItemPage {

		Task<Boolean> TabItemPageRequestClose (
		);

	}

}
