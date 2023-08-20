#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Media.Animation;

namespace Helper.Module {

	public partial class MainWindow : Window {

		#region life

		public MainWindow (
			ModuleType initialTab          = ModuleType.HomeSetting,
			Object?    initialTabParameter = null
		) {
			MainWindow.Instance = this;
			this.InitializeComponent();
			this.ExtendsContentIntoTitleBar = true;
			this.SetTitleBar(this.uTab.TabStripFooter as UIElement);
			if (new BackdropHelper(this).TryApplyMica()) {
				(this.Content as Panel)!.Background = null;
			}
			this.Controller = new MainWindowController() { View = this };
			this.Controller.Initialize(initialTab, initialTabParameter);
		}

		// ----------------

		public MainWindowController Controller { get; }

		#endregion

		#region instance

		public static MainWindow Instance { get; set; } = default!;

		#endregion

	}

	public class MainWindowController : CustomController {

		#region data

		public MainWindow View { get; init; } = default!;

		#endregion

		#region initialize

		public void Initialize (
			ModuleType initialTabType,
			Object?    initialTabParameter = null
		) {
			this.PushTabItem(initialTabType, initialTabParameter, false);
			return;
		}

		#endregion

		#region tab

		public ObservableCollection<TabItemController> uTab_TabItemsSource { get; } = new ObservableCollection<TabItemController>();

		public TabItemController? uTab_SelectedItem { get; set; } = null;

		public async void uTab_OnTabCloseRequested (
			TabView                           sender,
			TabViewTabCloseRequestedEventArgs args
		) {
			if (sender is not TabView senders) { return; }
			this.uTab_TabItemsSource.RemoveAt(this.uTab_TabItemsSource.Count - 1);
			return;
		}

		// ----------------

		public void PushTabItem (
			ModuleType type,
			Object?    parameter,
			Boolean    isClosable
		) {
			var model = ModuleInformationConstant.Value[(Size)type];
			var frame = new Frame() {
				ContentTransitions = new TransitionCollection() {
					new NavigationThemeTransition() {
						DefaultNavigationTransitionInfo = new DrillInNavigationTransitionInfo(),
					},
				},
			};
			frame.Navigate(model.Page, parameter);
			this.uTab_TabItemsSource.Add(new TabItemController() { Host = this, Model = model, IsCloseable = isClosable, Frame = frame });
			this.uTab_SelectedItem = this.uTab_TabItemsSource.Last();
			this.View.DispatcherQueue.EnqueueAsync(
				async Task () => {
					await Task.Delay(40);
					this.NotifyPropertyChanged(
						nameof(this.uTab_SelectedItem)
					);
				}
			);
			return;
		}

		#endregion

		#region tip

		public Thickness uTip_Margin { get; set; } = new Thickness(0);

		public Boolean uTip_IsOpen { get; set; } = false;

		public InfoBarSeverity uTip_Severity { get; set; } = InfoBarSeverity.Informational;

		public String uTip_Title { get; set; } = "";

		// ----------------

		public async void PublishTip (
			Floater         position,
			InfoBarSeverity severity,
			String          title,
			Size            duration = 4000
		) {
			this.uTip_Margin = new Thickness(0, 0, 0, position);
			this.uTip_IsOpen = true;
			this.uTip_Severity = severity;
			this.uTip_Title = title;
			this.NotifyPropertyChanged(
				nameof(this.uTip_Margin),
				nameof(this.uTip_IsOpen),
				nameof(this.uTip_Severity),
				nameof(this.uTip_Title)
			);
			await Task.Delay(TimeSpan.FromMilliseconds(duration));
			this.uTip_IsOpen = false;
			this.NotifyPropertyChanged(
				nameof(this.uTip_IsOpen)
			);
			return;
		}

		#endregion

	}

	public class TabItemController : CustomController {

		#region data

		public MainWindowController Host { get; init; } = default!;

		// ----------------

		public ModuleInformation Model { get; init; } = default!;

		public Boolean IsCloseable { get; init; } = default!;

		public Frame Frame { get; init; } = default!;

		#endregion

		#region view

		public String uRoot_Header {
			get {
				return this.Model.Title;
			}
		}

		public FontIconSource uRoot_IconSource {
			get {
				return new FontIconSource() {
					Glyph = this.Model.Icon,
				};
			}
		}

		public Boolean uRoot_IsCloseable {
			get {
				return this.IsCloseable;
			}
		}

		public Frame uRoot_Content {
			get {
				return this.Frame;
			}
		}

		#endregion

	}

}
