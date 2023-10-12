#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class ModuleLauncherSettingPanel : UserControl {

		#region life

		public ModuleLauncherSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new ModuleLauncherSettingPanelController() { View = this };
		}

		// ----------------

		public ModuleLauncherSettingPanelController Controller { get; }

		#endregion

	}

	public class ModuleLauncherSettingPanelController : CustomController {

		#region data

		public ModuleLauncherSettingPanel View { get; init; } = default!;

		#endregion

	}

}
