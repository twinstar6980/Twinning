#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using FluentIconGlyph = Helper.Control.FluentIconGlyph;

namespace Helper.View {

	#region type

	public enum ModuleType {
		ModuleLauncher,
		ModdingWorker,
		ResourceForwarder,
		CommandSender,
		AnimationViewer,
		PackageBuilder,
	}

	#endregion

	#region information

	public class ModuleInformation {
		public required ModuleType Type;
		public required String     Icon;
		public required String     Name;
		public required Type       Page;
	}

	// ----------------

	public static class ModuleInformationConstant {

		public static readonly List<ModuleInformation> Value = [
			new () {
				Type = ModuleType.ModuleLauncher,
				Icon = FluentIconGlyph.OEM,
				Name = "Module Launcher",
				Page = typeof(ModuleLauncher.MainPage),
			},
			new () {
				Type = ModuleType.ModdingWorker,
				Icon = FluentIconGlyph.ProvisioningPackage,
				Name = "Modding Worker",
				Page = typeof(ModdingWorker.MainPage),
			},
			new () {
				Type = ModuleType.ResourceForwarder,
				Icon = FluentIconGlyph.Share,
				Name = "Resource Forwarder",
				Page = typeof(ResourceForwarder.MainPage),
			},
			new () {
				Type = ModuleType.CommandSender,
				Icon = FluentIconGlyph.Send,
				Name = "Command Sender",
				Page = typeof(CommandSender.MainPage),
			},
			new () {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Name = "Animation Viewer",
				Page = typeof(AnimationViewer.MainPage),
			},
			new () {
				Type = ModuleType.PackageBuilder,
				Icon = FluentIconGlyph.DialShape3,
				Name = "Package Builder",
				Page = typeof(PackageBuilder.MainPage),
			},
		];

		public static ModuleInformation Query (
			ModuleType type
		) {
			return ModuleInformationConstant.Value[(Size)type];
		}

	}

	#endregion

}
