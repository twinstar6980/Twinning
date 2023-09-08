#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Media.Animation;

namespace Helper.Module {

	public partial class MainWindow : Window {

		#region life

		public MainWindow (
		) {
			MainWindow.Instance = this;
			this.InitializeComponent();
			this.ExtendsContentIntoTitleBar = true;
			this.SetTitleBar(this.uTab.TabStripFooter as UIElement);
			this.Controller = new MainWindowController() { View = this };
			this.Controller.Initialize();
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
		) {
			return;
		}

		// ----------------

		public void PushTabItem (
			ModuleType    type,
			List<String>? option,
			Boolean       isClosable
		) {
			var model = ModuleInformationConstant.Value[(Size)type];
			var frame = new Frame() {
				ContentTransitions = new TransitionCollection() {
					new NavigationThemeTransition() {
						DefaultNavigationTransitionInfo = new DrillInNavigationTransitionInfo(),
					},
				},
			};
			frame.Navigate(model.Page, option);
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

		#region tab

		public ObservableCollection<TabItemController> uTab_TabItemsSource { get; } = new ObservableCollection<TabItemController>();

		public TabItemController? uTab_SelectedItem { get; set; } = null;

		public async void uTab_OnTabCloseRequested (
			TabView                           sender,
			TabViewTabCloseRequestedEventArgs args
		) {
			if (sender is not TabView senders) { return; }
			this.uTab_TabItemsSource.Remove(args.Item as TabItemController ?? throw new NullReferenceException());
			return;
		}

		#endregion

		#region tip

		public Boolean uTip_IsOpen { get; set; } = false;

		public InfoBarSeverity uTip_Severity { get; set; } = InfoBarSeverity.Informational;

		public String uTip_Title { get; set; } = "";

		public String uTip_Message { get; set; } = "";

		// ----------------

		public Size uTip__DelayCount { get; set; } = 0;

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
			await Task.Delay(TimeSpan.FromMilliseconds(100));
			this.uTip_IsOpen = true;
			this.NotifyPropertyChanged(nameof(this.uTip_IsOpen));
			this.uTip__DelayCount++;
			await Task.Delay(TimeSpan.FromMilliseconds(duration));
			this.uTip__DelayCount--;
			if (this.uTip__DelayCount == 0) {
				this.uTip_IsOpen = false;
				this.NotifyPropertyChanged(nameof(this.uTip_IsOpen));
			}
			return;
		}

		#endregion

	}

	public class TabItemController : CustomController {

		#region data

		public MainWindowController Host { get; init; } = default!;

		// ----------------

		public ModuleInformation Model { get; set; } = default!;

		public Boolean IsCloseable { get; set; } = default!;

		public Frame Frame { get; set; } = default!;

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
