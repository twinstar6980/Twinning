#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.Home {

	public sealed partial class OnboardingPanel : CustomControl {

		#region life

		private OnboardingPanelController Controller { get; }

		// ----------------

		public OnboardingPanel (
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

	public partial class OnboardingPanelController : CustomController {

		#region data

		public OnboardingPanel View { get; init; } = default!;

		// ----------------

		public Action PanelEnter => this.View.OnPanelEnter;

		public Action PanelExit => this.View.OnPanelExit;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			return;
		}

		#endregion

		#region feature

		public async void uImportSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var target = await StorageHelper.PickLoadFile(App.MainWindow, $"@application.setting_file");
			if (target != null) {
				await App.Setting.Load(target);
				await App.Setting.Save();
				this.PanelExit();
			}
			return;
		}

		public async void uQuickSetup_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var target = await StorageHelper.PickLoadDirectory(App.MainWindow, $"@application.home_directory");
			if (target != null) {
				await App.Setting.QuickSetup(target);
				await App.Setting.Save();
				this.PanelExit();
			}
			return;
		}

		#endregion

	}

}
