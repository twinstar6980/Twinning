#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus {

	#region type

	public enum ModuleType {
		ModdingWorker,
		CommandSender,
		ResourceShipper,
		AnimationViewer,
		ReflectionDescriptor,
		PackageBuilder,
	}

	// ----------------

	public record ModuleInformation {
		public ModuleType Type     = default!;
		public String     Icon     = default!;
		public String     Name     = default!;
		public Type       MainPage = default!;
		[DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicParameterlessConstructor | DynamicallyAccessedMemberTypes.PublicProperties)]
		public Type SettingPanel = default!;
	}

	#endregion

	#region launcher

	public record ModuleLauncherConfiguration {
		public String       Title   = default!;
		public ModuleType   Type    = default!;
		public List<String> Option  = default!;
		public List<String> Command = default!;
	}

	// ----------------

	public enum ModuleLauncherCategory {
		Module,
		Pinned,
		Recent,
	}

	public record ModuleLauncherSetting {
		public List<ModuleLauncherConfiguration> Module = default!;
		public List<ModuleLauncherConfiguration> Pinned = default!;
		public List<ModuleLauncherConfiguration> Recent = default!;
	}

	#endregion

	#region helper

	public static class ModuleHelper {

		public static readonly List<ModuleInformation> Information = [
			new () {
				Type = ModuleType.ModdingWorker,
				Icon = FluentIconGlyph.ProvisioningPackage,
				Name = "Modding Worker",
				MainPage = typeof(View.ModdingWorker.MainPage),
				SettingPanel = typeof(View.ModdingWorker.SettingPanel),
			},
			new () {
				Type = ModuleType.CommandSender,
				Icon = FluentIconGlyph.Send,
				Name = "Command Sender",
				MainPage = typeof(View.CommandSender.MainPage),
				SettingPanel = typeof(View.CommandSender.SettingPanel),
			},
			new () {
				Type = ModuleType.ResourceShipper,
				Icon = FluentIconGlyph.Share,
				Name = "Resource Shipper",
				MainPage = typeof(View.ResourceShipper.MainPage),
				SettingPanel = typeof(View.ResourceShipper.SettingPanel),
			},
			new () {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Name = "Animation Viewer",
				MainPage = typeof(View.AnimationViewer.MainPage),
				SettingPanel = typeof(View.AnimationViewer.SettingPanel),
			},
			new () {
				Type = ModuleType.ReflectionDescriptor,
				Icon = FluentIconGlyph.Library,
				Name = "Reflection Descriptor",
				MainPage = typeof(View.ReflectionDescriptor.MainPage),
				SettingPanel = typeof(View.ReflectionDescriptor.SettingPanel),
			},
			new () {
				Type = ModuleType.PackageBuilder,
				Icon = FluentIconGlyph.DialShape3,
				Name = "Package Builder",
				MainPage = typeof(View.PackageBuilder.MainPage),
				SettingPanel = typeof(View.PackageBuilder.SettingPanel),
			},
		];

		public static ModuleInformation Query (
			ModuleType type
		) {
			return ModuleHelper.Information[(Size)type];
		}

		// ----------------

		public static Boolean CompareLauncher (
			ModuleLauncherConfiguration thix,
			ModuleLauncherConfiguration that
		) {
			return thix.Type.Equals(that.Type) && thix.Option.SequenceEqual(that.Option);
		}

		public static List<String> GenerateArgument (
			ModuleLauncherConfiguration launcher
		) {
			return [
				"Application",
				..launcher.Command,
				"-Launch",
				launcher.Title,
				launcher.Type.ToString(),
				..launcher.Option,
			];
		}

	}

	#endregion

}
