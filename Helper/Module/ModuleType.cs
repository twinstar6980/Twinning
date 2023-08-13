#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.CustomControl;

namespace Helper.Module {

	#region type

	public enum ModuleType {
		HomeSetting,
		CommandForwarder,
		AnimationViewer,
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
				Type = ModuleType.HomeSetting,
				Icon = FluentIconGlyph.OEM,
				Title = "Home & Setting",
				Page = typeof(HomeSetting.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.CommandForwarder,
				Icon = FluentIconGlyph.CommandPrompt,
				Title = "Command Forwarder",
				Page = typeof(CommandForwarder.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Title = "Animation Viewer",
				Page = typeof(AnimationViewer.MainPage),
			},
		};
	}

	#endregion

}
