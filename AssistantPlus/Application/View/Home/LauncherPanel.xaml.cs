#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.Home {

	public sealed partial class LauncherPanel : CustomControl {

		#region life

		private LauncherPanelController Controller { get; }

		// ----------------

		public LauncherPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
		) {
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		public event Action? PanelEnter;

		public void OnPanelEnter (
		) => this.PanelEnter?.Invoke();

		// ----------------

		public event Action? PanelExit;

		public void OnPanelExit (
		) => this.PanelExit?.Invoke();

		#endregion

	}

	public partial class LauncherPanelController : CustomController {

		#region data

		public LauncherPanel View { get; init; } = default!;

		// ----------------

		public Action PanelEnter => this.View.OnPanelEnter;

		public Action PanelExit => this.View.OnPanelExit;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.uModuleLauncherList_ItemsSource = App.Setting.Data.ModuleLauncher.Module.Select((value) => (new LauncherPageLauncherItemController() { Host = this, Category = ModuleLauncherCategory.Module, Configuration = value })).ToList();
			this.uPinnedLauncherList_ItemsSource = App.Setting.Data.ModuleLauncher.Pinned.Select((value) => (new LauncherPageLauncherItemController() { Host = this, Category = ModuleLauncherCategory.Pinned, Configuration = value })).ToObservableCollection();
			this.uRecentLauncherList_ItemsSource = App.Setting.Data.ModuleLauncher.Recent.Select((value) => (new LauncherPageLauncherItemController() { Host = this, Category = ModuleLauncherCategory.Recent, Configuration = value })).ToObservableCollection();
			this.NotifyPropertyChanged([
				nameof(this.uModuleLauncherList_ItemsSource),
				nameof(this.uPinnedLauncherList_ItemsSource),
				nameof(this.uRecentLauncherList_ItemsSource),
			]);
			return;
		}

		#endregion

		#region launcher

		public async void uCommand_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			this.PanelExit();
			var command = new List<String>();
			var canContinue = await ControlHelper.ShowDialogAsAutomatic(this.View, "Command", new TextBox() {
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				TextWrapping = TextWrapping.Wrap,
				AcceptsReturn = true,
				Text = "",
			}.SelfAlso((it) => {
				it.LostFocus += async (_, _) => {
					command = ConvertHelper.ParseStringListFromStringWithLine(it.Text);
					it.Text = ConvertHelper.MakeStringListToStringWithLine(command);
					return;
				};
			}), new ("Cancel", "Continue", null)) == ContentDialogResult.Primary;
			if (canContinue) {
				await App.Instance.HandleCommand(command);
			}
			else {
				this.PanelEnter();
			}
			return;
		}

		// ----------------

		public List<LauncherPageLauncherItemController> uModuleLauncherList_ItemsSource { get; set; } = default!;

		public async void uModuleLauncherList_ItemClick (
			Object             sender,
			ItemClickEventArgs args
		) {
			var senders = sender.As<ListView>();
			await App.Instance.ExecuteLauncher(args.ClickedItem.As<LauncherPageLauncherItemController>().Configuration, false);
			return;
		}

		// ----------------

		public ObservableCollection<LauncherPageLauncherItemController> uPinnedLauncherList_ItemsSource { get; set; } = default!;

		public async void uPinnedLauncherList_ItemClick (
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
				Type = ModuleType.CoreTaskWorker,
				Option = [],
				Command = [],
			});
			this.uPinnedLauncherList_ItemsSource.Add(new () { Host = this, Category = ModuleLauncherCategory.Pinned, Configuration = App.Setting.Data.ModuleLauncher.Pinned[^1] });
			await App.Setting.Save();
			return;
		}

		// ----------------

		public ObservableCollection<LauncherPageLauncherItemController> uRecentLauncherList_ItemsSource { get; set; } = default!;

		public async void uRecentLauncherList_ItemClick (
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
			this.PanelExit();
			if (await ControlHelper.ShowDialogForConfirm(this.View, null, null)) {
				App.Setting.Data.ModuleLauncher.Recent.Clear();
				this.uRecentLauncherList_ItemsSource.Clear();
				await App.Setting.Save();
			}
			this.PanelEnter();
			return;
		}

		#endregion

	}

	public partial class LauncherPageLauncherItemController : CustomController {

		#region data

		public LauncherPanelController Host { get; init; } = default!;

		// ----------------

		public ModuleLauncherCategory Category { get; set; } = default!;

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
			this.Host.PanelExit();
			await ControlHelper.ShowDialogAsFixed(this.Host.View, "Launcher Configuration", new LauncherConfigurationPanel() {
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				Data = this.Configuration,
				Stamp = UniqueStamp.Create(),
			}, null);
			this.NotifyPropertyChanged([
				nameof(this.uIcon_Glyph),
				nameof(this.uTitle_Text),
			]);
			await App.Setting.Save();
			this.Host.PanelEnter();
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
			AssertTest(this.Category == ModuleLauncherCategory.Module);
			this.Host.PanelExit();
			await ControlHelper.ShowDialogAsFixed(this.Host.View, "Module Setting", ModuleHelper.Query(this.Configuration.Type).SettingPanel(), null);
			await App.Setting.Save();
			this.Host.PanelEnter();
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
			AssertTest(this.Category == ModuleLauncherCategory.Pinned || this.Category == ModuleLauncherCategory.Recent);
			if (this.Category == ModuleLauncherCategory.Pinned) {
				this.Host.PanelExit();
				if (await ControlHelper.ShowDialogForConfirm(this.Host.View, null, null)) {
					App.Setting.Data.ModuleLauncher.Pinned.Remove(this.Configuration);
					this.Host.uPinnedLauncherList_ItemsSource.Remove(this);
					await App.Setting.Save();
				}
				this.Host.PanelEnter();
			}
			if (this.Category == ModuleLauncherCategory.Recent) {
				App.Setting.Data.ModuleLauncher.Recent.Remove(this.Configuration);
				this.Host.uRecentLauncherList_ItemsSource.Remove(this);
				await App.Setting.Save();
			}
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
			AssertTest(this.Category == ModuleLauncherCategory.Recent);
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
