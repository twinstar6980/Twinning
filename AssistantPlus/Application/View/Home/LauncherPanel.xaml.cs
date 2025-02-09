#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.Home {

	public sealed partial class LauncherPanel : CustomControl {

		#region life

		public LauncherPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private LauncherPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		#endregion

	}

	public class LauncherPanelController : CustomController {

		#region data

		public LauncherPanel View { get; init; } = default!;

		// ----------------

		#endregion

		#region update

		public async void Update (
		) {
			this.uModuleLauncherList_ItemsSource = App.Setting.Data.ModuleLauncher.Module.Select((value) => (new LauncherPageLauncherItemController() { Host = this, Category = ModuleLauncherCategory.Module, Configuration = value })).ToList();
			this.uPinnedLauncherList_ItemsSource = App.Setting.Data.ModuleLauncher.Pinned.Select((value) => (new LauncherPageLauncherItemController() { Host = this, Category = ModuleLauncherCategory.Pinned, Configuration = value })).ToObservableCollection();
			this.uRecentLauncherList_ItemsSource = App.Setting.Data.ModuleLauncher.Recent.Select((value) => (new LauncherPageLauncherItemController() { Host = this, Category = ModuleLauncherCategory.Recent, Configuration = value })).ToObservableCollection();
			this.NotifyPropertyChanged(
				nameof(this.uModuleLauncherList_ItemsSource),
				nameof(this.uPinnedLauncherList_ItemsSource),
				nameof(this.uRecentLauncherList_ItemsSource)
			);
			return;
		}

		#endregion

		#region launcher

		public List<LauncherPageLauncherItemController> uModuleLauncherList_ItemsSource { get; set; } = default!;

		public async void uModuleLauncherList_OnItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			var senders = sender.As<ListView>();
			await App.Instance.ExecuteLauncher(args.ClickedItem.As<LauncherPageLauncherItemController>().Configuration, false);
			return;
		}

		// ----------------

		public ObservableCollection<LauncherPageLauncherItemController> uPinnedLauncherList_ItemsSource { get; set; } = default!;

		public async void uPinnedLauncherList_OnItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			var senders = sender.As<ListView>();
			await App.Instance.ExecuteLauncher(args.ClickedItem.As<LauncherPageLauncherItemController>().Configuration, false);
			return;
		}

		public async void uAddPinnedLauncherItem_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			App.Setting.Data.ModuleLauncher.Pinned.Add(new () {
				Title = "Untitled",
				Type = ModuleType.ModdingWorker,
				Option = [],
				Command = [],
			});
			this.uPinnedLauncherList_ItemsSource.Add(new () { Host = this, Category = ModuleLauncherCategory.Pinned, Configuration = App.Setting.Data.ModuleLauncher.Pinned[^1] });
			await App.Setting.Save();
			return;
		}

		// ----------------

		public ObservableCollection<LauncherPageLauncherItemController> uRecentLauncherList_ItemsSource { get; set; } = default!;

		public async void uRecentLauncherList_OnItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			var senders = sender.As<ListView>();
			await App.Instance.ExecuteLauncher(args.ClickedItem.As<LauncherPageLauncherItemController>().Configuration, false);
			return;
		}

		public async void uClearRecentLauncherItem_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (await ControlHelper.ShowDialogForConfirm(this.View, null, null)) {
				App.Setting.Data.ModuleLauncher.Recent.Clear();
				this.uRecentLauncherList_ItemsSource.Clear();
				await App.Setting.Save();
			}
			return;
		}

		#endregion

	}

	public class LauncherPageLauncherItemController : CustomController {

		#region data

		public LauncherPanelController Host { get; init; } = default!;

		// ----------------

		public ModuleLauncherCategory Category { get; set; } = default;

		public ModuleLauncherConfiguration Configuration { get; set; } = default!;

		#endregion

		#region view

		public String uIcon_Glyph {
			get {
				return ModuleHelper.Query(this.Configuration.Type).Icon;
			}
		}

		public String uTitle_Text {
			get {
				return this.Configuration.Title;
			}
		}

		// ----------------

		public async void uEdit_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			await ControlHelper.ShowDialogAsFixed(this.Host.View, "Launcher Configuration", new LauncherConfigurationPanel() {
				Data = this.Configuration,
				Stamp = UniqueStamp.Create(),
			}, null);
			await App.Setting.Save();
			this.NotifyPropertyChanged(
				nameof(this.uIcon_Glyph),
				nameof(this.uTitle_Text)
			);
			return;
		}

		public async void uLaunchAsWindow_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			await App.Instance.ExecuteLauncher(this.Configuration, true);
			return;
		}

		// ----------------

		public Boolean uSetting_Visibility {
			get {
				return this.Category == ModuleLauncherCategory.Module;
			}
		}

		public async void uSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.Category == ModuleLauncherCategory.Module);
			var panelType = ModuleHelper.Query(this.Configuration.Type).SettingPanel;
			var panelObject = Activator.CreateInstance(panelType).AsNotNull().As<UIElement>();
			var settingData = typeof(SettingData).GetField(this.Configuration.Type.ToString()).AsNotNull().GetValue(App.Setting.Data);
			panelType.GetProperty("Data").AsNotNull().SetValue(panelObject, settingData);
			await ControlHelper.ShowDialogAsFixed(this.Host.View, "Module Setting", panelObject, null);
			await App.Setting.Save();
			return;
		}

		public Boolean uRemove_Visibility {
			get {
				return this.Category == ModuleLauncherCategory.Pinned || this.Category == ModuleLauncherCategory.Recent;
			}
		}

		public async void uRemove_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.Category == ModuleLauncherCategory.Pinned || this.Category == ModuleLauncherCategory.Recent);
			if (this.Category == ModuleLauncherCategory.Pinned) {
				App.Setting.Data.ModuleLauncher.Pinned.Remove(this.Configuration);
				this.Host.uPinnedLauncherList_ItemsSource.Remove(this);
			}
			else {
				App.Setting.Data.ModuleLauncher.Recent.Remove(this.Configuration);
				this.Host.uRecentLauncherList_ItemsSource.Remove(this);
			}
			await App.Setting.Save();
			return;
		}

		public Boolean uPin_Visibility {
			get {
				return this.Category == ModuleLauncherCategory.Recent;
			}
		}

		public async void uPin_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.Category == ModuleLauncherCategory.Recent);
			App.Setting.Data.ModuleLauncher.Recent.Remove(this.Configuration);
			App.Setting.Data.ModuleLauncher.Pinned.Add(this.Configuration);
			this.Host.uPinnedLauncherList_ItemsSource.Add(new () { Host = this.Host, Category = ModuleLauncherCategory.Pinned, Configuration = this.Configuration });
			this.Host.uRecentLauncherList_ItemsSource.Remove(this);
			await App.Setting.Save();
			return;
		}

		#endregion

	}

}
