#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CommandSender {

	public sealed partial class SettingPanel : CustomControl {

		#region life

		private SettingPanelController Controller { get; }

		// ----------------

		public SettingPanel (
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

		public static readonly DependencyProperty DataProperty = DependencyProperty.Register(
			nameof(SettingPanel.Data),
			typeof(Setting),
			typeof(SettingPanel),
			new (new Setting() {
				ParallelForward = false,
			})
		);

		public Setting Data {
			get => this.GetValue(SettingPanel.DataProperty).As<Setting>();
			set => this.SetValue(SettingPanel.DataProperty, value);
		}

		#endregion

	}

	public partial class SettingPanelController : CustomController {

		#region data

		public SettingPanel View { get; init; } = default!;

		// ----------------

		public Setting Data => this.View.Data;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.NotifyPropertyChanged([
				nameof(this.uParallelForwardToggle_IsChecked),
			]);
			return;
		}

		#endregion

		#region parallel forward

		public Boolean uParallelForwardToggle_IsChecked {
			get {
				return this.Data.ParallelForward;
			}
		}

		public async void uParallelForwardToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Data.ParallelForward = senders.IsChecked.AsNotNull();
			this.NotifyPropertyChanged([
				nameof(this.uParallelForwardToggle_IsChecked),
			]);
			return;
		}

		#endregion

	}

}
