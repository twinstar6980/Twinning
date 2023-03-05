#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using CommunityToolkit.WinUI;

namespace Helper.Module {

	public partial class MainWindow : Window {

		#region life

		public MainWindow (
		) {
			this.InitializeComponent();
			this.ExtendsContentIntoTitleBar = true;
			this.SetTitleBar(this.uTab.TabStripFooter as UIElement);
			if (new BackdropHelper(this).TryApplyMica()) {
				((Grid)this.Content).Background = null;
			}
			MainWindow.sInstance = this;
		}

		#endregion

		#region instance

		private static MainWindow? sInstance = null;

		public static MainWindow Instance {
			get {
				return MainWindow.sInstance!;
			}
		}

		#endregion

		#region tab item

		private TabViewItem CreateTabItem (
			ModuleType type,
			Boolean    isClosable
		) {
			var information = ModuleInformationConstant.Value[(Size)type];
			var tabFrame = new Frame();
			tabFrame.Navigate(information.Page);
			return new TabViewItem {
				Header = information.Title,
				IconSource = new FontIconSource() {
					FontSize = 20,
					Glyph = information.Icon,
				},
				Content = tabFrame,
				IsClosable = isClosable,
			};
		}

		public void PushTabItem (
			ModuleType type,
			Boolean    isClosable
		) {
			this.uTab.TabItems.Add(this.CreateTabItem(type, isClosable));
			this.DispatcherQueue.EnqueueAsync(
				async Task () => {
					await Task.Delay(20);
					this.uTab.SelectedItem = this.uTab.TabItems.Last();
				}
			);
			return;
		}

		#endregion

		#region tab

		private async void uTab_OnTabCloseRequested (
			TabView                           sender,
			TabViewTabCloseRequestedEventArgs args
		) {
			this.uTab.TabItems.Remove(args.Tab);
		}

		private async void uTab_OnLoaded (
			Object          sender,
			RoutedEventArgs e
		) {
			this.uTab.TabItems.Add(this.CreateTabItem(ModuleType.HomeSetting, false));
		}

		#endregion

	}

}
