#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using FluentIconGlyph = Twinning.AssistantPlus.Control.FluentIconGlyph;

namespace Twinning.AssistantPlus {

	#region type

	public enum ModuleType {
		CoreModdingWorker,
		CoreCommandSender,
		CoreResourceShipper,
		PopcapAnimationViewer,
		PopcapReflectionDescriptor,
		PopcapPackageBuilder,
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
				Identifier = "core_modding_worker",
				Type = ModuleType.CoreModdingWorker,
				Name = "Modding Worker",
				Icon = FluentIconGlyph.ProvisioningPackage,
				MainPage = typeof(View.CoreModdingWorker.MainPage),
				SettingPanel = () => new View.CoreModdingWorker.SettingPanel() {
					Data = App.Setting.Data.CoreModdingWorker,
				},
				GenerateForwardOption = (resource) => ["-additional_argument", ..resource],
			},
			new () {
				Identifier = "core_command_sender",
				Type = ModuleType.CoreCommandSender,
				Name = "Command Sender",
				Icon = FluentIconGlyph.Send,
				MainPage = typeof(View.CoreCommandSender.MainPage),
				SettingPanel = () => new View.CoreCommandSender.SettingPanel() {
					Data = App.Setting.Data.CoreCommandSender,
				},
				GenerateForwardOption = (resource) => null,
			},
			new () {
				Identifier = "core_resource_shipper",
				Type = ModuleType.CoreResourceShipper,
				Name = "Resource Shipper",
				Icon = FluentIconGlyph.Share,
				MainPage = typeof(View.CoreResourceShipper.MainPage),
				SettingPanel = () => new View.CoreResourceShipper.SettingPanel() {
					Data = App.Setting.Data.CoreResourceShipper,
				},
				GenerateForwardOption = (resource) => ["-resource", ..resource],
			},
			new () {
				Identifier = "popcap_animation_viewer",
				Type = ModuleType.PopcapAnimationViewer,
				Name = "PopCap Animation Viewer",
				Icon = FluentIconGlyph.HomeGroup,
				MainPage = typeof(View.PopcapAnimationViewer.MainPage),
				SettingPanel = () => new View.PopcapAnimationViewer.SettingPanel() {
					Data = App.Setting.Data.PopcapAnimationViewer,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pam\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-animation_file", resource[0]],
			},
			new () {
				Identifier = "popcap_reflection_descriptor",
				Type = ModuleType.PopcapReflectionDescriptor,
				Name = "PopCap Reflection Descriptor",
				Icon = FluentIconGlyph.Library,
				MainPage = typeof(View.PopcapReflectionDescriptor.MainPage),
				SettingPanel = () => new View.PopcapReflectionDescriptor.SettingPanel() {
					Data = App.Setting.Data.PopcapReflectionDescriptor,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-descriptor_file", resource[0]],
			},
			new () {
				Identifier = "popcap_package_builder",
				Type = ModuleType.PopcapPackageBuilder,
				Name = "PopCap Package Builder",
				Icon = FluentIconGlyph.DialShape3,
				MainPage = typeof(View.PopcapPackageBuilder.MainPage),
				SettingPanel = () => new View.PopcapPackageBuilder.SettingPanel() {
					Data = App.Setting.Data.PopcapPackageBuilder,
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
