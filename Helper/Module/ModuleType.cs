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
				Description = "...",
				Page = typeof(HomeSetting.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Title = "Animation Viewer",
				Description = "...",
				Page = typeof(AnimationViewer.MainPage),
			},
			//new ModuleInformation {
			//	Type = ModuleType.ObjectEditor,
			//	Icon = FluentIconGlyph.Code,
			//	Title = "Object Editor",
			//	Description = "...",
			//	Page = typeof(ObjectEditor.MainPage),
			//},
			//new ModuleInformation {
			//	Type = ModuleType.LevelCreator,
			//	Icon = FluentIconGlyph.FitPage,
			//	Title = "Level Creator",
			//	Description = "...",
			//	Page = typeof(LevelCreator.MainPage),
			//},
			//new ModuleInformation {
			//	Type = ModuleType.MapDesigner,
			//	Icon = FluentIconGlyph.MapPin,
			//	Title = "Map Designer",
			//	Description = "...",
			//	Page = typeof(MapDesigner.MainPage),
			//},
			//new ModuleInformation {
			//	Type = ModuleType.PackageBuilder,
			//	Icon = FluentIconGlyph.IOT,
			//	Title = "Package Builder",
			//	Description = "...",
			//	Page = typeof(PackageBuilder.MainPage),
			//},
		};
	}

	#endregion

}
