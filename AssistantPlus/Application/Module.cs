#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using FluentIconGlyph = Twinning.AssistantPlus.Control.FluentIconGlyph;

namespace Twinning.AssistantPlus {

	#region type

	public enum ModuleType {
		CoreTaskWorker,
		CoreCommandSender,
		CoreResourceShipper,
		PopcapAnimationViewer,
		PopcapReflectionDescriptor,
		PopcapPackageBuilder,
	}

	// ----------------

	public record ModuleInformation {
		public ModuleType                        Type                  = default!;
		public String                            Identifier            = default!;
		public String                            Name                  = default!;
		public String                            Icon                  = default!;
		public Type                              MainPage              = default!;
		public Func<UIElement>                   SettingPanel          = default!;
		public Func<List<String>, List<String>?> GenerateForwardOption = default!;
		public Tuple<Integer, Integer>           StandardSize          = default!;
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
				Type = ModuleType.CoreTaskWorker,
				Identifier = "core_task_worker",
				Name = "Task Worker",
				Icon = FluentIconGlyph.ProvisioningPackage,
				MainPage = typeof(View.CoreTaskWorker.MainPage),
				SettingPanel = () => new View.CoreTaskWorker.SettingPanel() {
					Data = App.Setting.Data.CoreTaskWorker,
				},
				GenerateForwardOption = (resource) => ["-additional_argument", ..resource],
				StandardSize = new (480, 840),
			},
			new () {
				Type = ModuleType.CoreCommandSender,
				Identifier = "core_command_sender",
				Name = "Command Sender",
				Icon = FluentIconGlyph.Send,
				MainPage = typeof(View.CoreCommandSender.MainPage),
				SettingPanel = () => new View.CoreCommandSender.SettingPanel() {
					Data = App.Setting.Data.CoreCommandSender,
				},
				GenerateForwardOption = (resource) => null,
				StandardSize = new (920, 840),
			},
			new () {
				Type = ModuleType.CoreResourceShipper,
				Identifier = "core_resource_shipper",
				Name = "Resource Shipper",
				Icon = FluentIconGlyph.Share,
				MainPage = typeof(View.CoreResourceShipper.MainPage),
				SettingPanel = () => new View.CoreResourceShipper.SettingPanel() {
					Data = App.Setting.Data.CoreResourceShipper,
				},
				GenerateForwardOption = (resource) => ["-resource", ..resource],
				StandardSize = new (480, 840),
			},
			new () {
				Type = ModuleType.PopcapAnimationViewer,
				Identifier = "popcap_animation_viewer",
				Name = "PopCap Animation Viewer",
				Icon = FluentIconGlyph.HomeGroup,
				MainPage = typeof(View.PopcapAnimationViewer.MainPage),
				SettingPanel = () => new View.PopcapAnimationViewer.SettingPanel() {
					Data = App.Setting.Data.PopcapAnimationViewer,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pam\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-animation_file", resource[0]],
				StandardSize = new (1600, 840),
			},
			new () {
				Type = ModuleType.PopcapReflectionDescriptor,
				Identifier = "popcap_reflection_descriptor",
				Name = "PopCap Reflection Descriptor",
				Icon = FluentIconGlyph.Library,
				MainPage = typeof(View.PopcapReflectionDescriptor.MainPage),
				SettingPanel = () => new View.PopcapReflectionDescriptor.SettingPanel() {
					Data = App.Setting.Data.PopcapReflectionDescriptor,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-descriptor_file", resource[0]],
				StandardSize = new (920, 840),
			},
			new () {
				Type = ModuleType.PopcapPackageBuilder,
				Identifier = "popcap_package_builder",
				Name = "PopCap Package Builder",
				Icon = FluentIconGlyph.DialShape3,
				MainPage = typeof(View.PopcapPackageBuilder.MainPage),
				SettingPanel = () => new View.PopcapPackageBuilder.SettingPanel() {
					Data = App.Setting.Data.PopcapPackageBuilder,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pvz2_package_project)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistDirectory(resource[0]) ? null : ["-project_directory", resource[0]],
				StandardSize = new (1440, 840),
			},
		];

		public static ModuleInformation Query (
			ModuleType type
		) {
			return ModuleHelper.Information[type.CastPrimitive<Size>()];
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
