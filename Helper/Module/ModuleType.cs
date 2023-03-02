#pragma warning disable 0,
// ReSharper disable

using Helper;
using FluentIconGlyph = Helper.CustomControl.FluentIconGlyph;

namespace Helper.Module {

	#region type

	public enum ModuleType {
		HomeSetting,
		AnimationViewer,
		//ObjectEditor,
		//LevelCreator,
		//MapDesigner,
		//PackageBuilder,
	}

	#endregion

	#region information

	public class ModuleInformation {
		public required ModuleType Type;
		public required String     Icon;
		public required String     Title;
		public required String     Description;
		public required Type       Page;
	}

	// ----------------

	public static class ModuleInformationConstant {
		public static readonly List<ModuleInformation> Value = new () {
			new ModuleInformation {
				Type = ModuleType.HomeSetting,
				Icon = FluentIconGlyph.OEM,
				Title = "Home & Setting",
				Description = "View all modules and settings.",
				Page = typeof(HomeSetting.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Title = "Animation Viewer",
				Description = "A viewer for PvZ-2 animation.",
				Page = typeof(AnimationViewer.MainPage),
			},
		};
	}

	#endregion

}
