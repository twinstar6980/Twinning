#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class PackageBuilderSettingPanel : CustomControl {

		#region life

		public PackageBuilderSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new PackageBuilderSettingPanelController() { View = this };
		}

		// ----------------

		private PackageBuilderSettingPanelController Controller { get; }

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

	public class PackageBuilderSettingPanelController : CustomController {

		#region data

		public PackageBuilderSettingPanel View { get; init; } = default!;

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
