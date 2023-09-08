#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.CustomControl;

namespace Helper.Module {

	#region type

	public enum ModuleType {
		ApplicationSetting,
		CommandForwarder,
		AnimationViewer,
		CommandForwarderQuick,
	}

	#endregion

	#region information

	public class ModuleInformation {
		public required ModuleType Type;
		public required String     Icon;
		public required String     Title;
		public required Type       Page;
	}

	// ----------------

	public static class ModuleInformationConstant {
		public static readonly List<ModuleInformation> Value = new List<ModuleInformation> {
			new ModuleInformation {
				Type = ModuleType.ApplicationSetting,
				Icon = FluentIconGlyph.OEM,
				Title = "Application Setting",
				Page = typeof(ApplicationSetting.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.CommandForwarder,
				Icon = FluentIconGlyph.Send,
				Title = "Command Forwarder",
				Page = typeof(CommandForwarder.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Title = "Animation Viewer",
				Page = typeof(AnimationViewer.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.CommandForwarderQuick,
				Icon = FluentIconGlyph.Send,
				Title = "Command Forwarder - Quick",
				Page = typeof(CommandForwarder.QuickPage),
			},
		};
	}

	#endregion

}
