#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using FluentIconGlyph = Twinning.AssistantPlus.Control.FluentIconGlyph;

namespace Twinning.AssistantPlus {

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
		public String                            Identifier            = default!;
		public ModuleType                        Type                  = default!;
		public String                            Name                  = default!;
		public String                            Icon                  = default!;
		public Type                              MainPage              = default!;
		public Func<UIElement>                   SettingPanel          = default!;
		public Func<List<String>, List<String>?> GenerateForwardOption = default!;
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

		private static readonly List<ModuleInformation> Information = [
			new () {
				Identifier = "modding_worker",
				Type = ModuleType.ModdingWorker,
				Name = "Modding Worker",
				Icon = FluentIconGlyph.ProvisioningPackage,
				MainPage = typeof(View.ModdingWorker.MainPage),
				SettingPanel = () => new View.ModdingWorker.SettingPanel() {
					Data = App.Setting.Data.ModdingWorker,
				},
				GenerateForwardOption = (resource) => ["-additional_argument", ..resource],
			},
			new () {
				Identifier = "command_sender",
				Type = ModuleType.CommandSender,
				Name = "Command Sender",
				Icon = FluentIconGlyph.Send,
				MainPage = typeof(View.CommandSender.MainPage),
				SettingPanel = () => new View.CommandSender.SettingPanel() {
					Data = App.Setting.Data.CommandSender,
				},
				GenerateForwardOption = (resource) => null,
			},
			new () {
				Identifier = "resource_shipper",
				Type = ModuleType.ResourceShipper,
				Name = "Resource Shipper",
				Icon = FluentIconGlyph.Share,
				MainPage = typeof(View.ResourceShipper.MainPage),
				SettingPanel = () => new View.ResourceShipper.SettingPanel() {
					Data = App.Setting.Data.ResourceShipper,
				},
				GenerateForwardOption = (resource) => ["-resource", ..resource],
			},
			new () {
				Identifier = "animation_viewer",
				Type = ModuleType.AnimationViewer,
				Name = "Animation Viewer",
				Icon = FluentIconGlyph.HomeGroup,
				MainPage = typeof(View.AnimationViewer.MainPage),
				SettingPanel = () => new View.AnimationViewer.SettingPanel() {
					Data = App.Setting.Data.AnimationViewer,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pam\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-animation_file", resource[0]],
			},
			new () {
				Identifier = "reflection_descriptor",
				Type = ModuleType.ReflectionDescriptor,
				Name = "Reflection Descriptor",
				Icon = FluentIconGlyph.Library,
				MainPage = typeof(View.ReflectionDescriptor.MainPage),
				SettingPanel = () => new View.ReflectionDescriptor.SettingPanel() {
					Data = App.Setting.Data.ReflectionDescriptor,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-descriptor_file", resource[0]],
			},
			new () {
				Identifier = "package_builder",
				Type = ModuleType.PackageBuilder,
				Name = "Package Builder",
				Icon = FluentIconGlyph.DialShape3,
				MainPage = typeof(View.PackageBuilder.MainPage),
				SettingPanel = () => new View.PackageBuilder.SettingPanel() {
					Data = App.Setting.Data.PackageBuilder,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pvz2_package_project)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistDirectory(resource[0]) ? null : ["-project_directory", resource[0]],
			},
		];

		public static ModuleInformation Query (
			ModuleType type
		) {
			return ModuleHelper.Information[type.CastPrimitive<Size>()];
		}

		public static ModuleInformation QueryByIdentifier (
			String identifier
		) {
			return ModuleHelper.Information.First((it) => it.Identifier == identifier);
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
				"application",
				..launcher.Command,
				"-launch",
				launcher.Title,
				ModuleHelper.Query(launcher.Type).Identifier,
				..launcher.Option,
			];
		}

	}

	#endregion

}
