#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.ModuleLauncher {

	public sealed partial class ModuleLauncherSettingPanel : CustomControl {

		#region life

		public ModuleLauncherSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private ModuleLauncherSettingPanelController Controller { get; }

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

	public class ModuleLauncherSettingPanelController : CustomController {

		#region data

		public ModuleLauncherSettingPanel View { get; init; } = default!;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
			);
			return;
		}

		#endregion

	}

}
