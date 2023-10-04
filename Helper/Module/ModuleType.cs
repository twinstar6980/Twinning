#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.CustomControl;

namespace Helper.Module {

	#region type

	public enum ModuleType {
		ApplicationSetting,
		ModdingWorker,
		ResourceForwarder,
		CommandSender,
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
				Type = ModuleType.ApplicationSetting,
				Icon = FluentIconGlyph.OEM,
				Title = "Application Setting",
				Page = typeof(ApplicationSetting.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.ModdingWorker,
				Icon = FluentIconGlyph.ProvisioningPackage,
				Title = "Modding Worker",
				Page = typeof(ModdingWorker.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.ResourceForwarder,
				Icon = FluentIconGlyph.Share,
				Title = "Resource Forwarder",
				Page = typeof(ResourceForwarder.MainPage),
			},
			new ModuleInformation {
				Type = ModuleType.CommandSender,
				Icon = FluentIconGlyph.Send,
				Title = "Command Sender",
				Page = typeof(CommandSender.MainPage),
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
