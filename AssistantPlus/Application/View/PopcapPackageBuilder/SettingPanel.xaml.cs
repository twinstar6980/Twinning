#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.PopcapPackageBuilder {

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
			]);
			return;
		}

		#endregion

	}

}
