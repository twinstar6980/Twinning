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
		public ModuleType                        Type                  = default!;
		public String                            Icon                  = default!;
		public String                            Name                  = default!;
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
				Type = ModuleType.ModdingWorker,
				Icon = FluentIconGlyph.ProvisioningPackage,
				Name = "Modding Worker",
				MainPage = typeof(View.ModdingWorker.MainPage),
				SettingPanel = () => new View.ModdingWorker.SettingPanel() {
					Data = App.Setting.Data.ModdingWorker,
				},
				GenerateForwardOption = (resource) => ["-AdditionalArgument", ..resource],
			},
			new () {
				Type = ModuleType.CommandSender,
				Icon = FluentIconGlyph.Send,
				Name = "Command Sender",
				MainPage = typeof(View.CommandSender.MainPage),
				SettingPanel = () => new View.CommandSender.SettingPanel() {
					Data = App.Setting.Data.CommandSender,
				},
				GenerateForwardOption = (resource) => null,
			},
			new () {
				Type = ModuleType.ResourceShipper,
				Icon = FluentIconGlyph.Share,
				Name = "Resource Shipper",
				MainPage = typeof(View.ResourceShipper.MainPage),
				SettingPanel = () => new View.ResourceShipper.SettingPanel() {
					Data = App.Setting.Data.ResourceShipper,
				},
				GenerateForwardOption = (resource) => ["-Resource", ..resource],
			},
			new () {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Name = "Animation Viewer",
				MainPage = typeof(View.AnimationViewer.MainPage),
				SettingPanel = () => new View.AnimationViewer.SettingPanel() {
					Data = App.Setting.Data.AnimationViewer,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pam\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-AnimationFile", resource[0]],
			},
			new () {
				Type = ModuleType.ReflectionDescriptor,
				Icon = FluentIconGlyph.Library,
				Name = "Reflection Descriptor",
				MainPage = typeof(View.ReflectionDescriptor.MainPage),
				SettingPanel = () => new View.ReflectionDescriptor.SettingPanel() {
					Data = App.Setting.Data.ReflectionDescriptor,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistFile(resource[0]) ? null : ["-DescriptorFile", resource[0]],
			},
			new () {
				Type = ModuleType.PackageBuilder,
				Icon = FluentIconGlyph.DialShape3,
				Name = "Package Builder",
				MainPage = typeof(View.PackageBuilder.MainPage),
				SettingPanel = () => new View.PackageBuilder.SettingPanel() {
					Data = App.Setting.Data.PackageBuilder,
				},
				GenerateForwardOption = (resource) => resource.Count != 1 || !new Regex(@"(\.pvz2_package_project)$", RegexOptions.IgnoreCase).IsMatch(resource[0]) || !StorageHelper.ExistDirectory(resource[0]) ? null : ["-ProjectDirectory", resource[0]],
			},
		];

		public static ModuleInformation Query (
			ModuleType type
		) {
			return ModuleHelper.Information[type.AsCast<Size>()];
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
