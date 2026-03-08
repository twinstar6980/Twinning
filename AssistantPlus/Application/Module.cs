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
		public ModuleType                        Type                  { get; set; } = default!;
		public String                            Identifier            { get; set; } = default!;
		public String                            Name                  { get; set; } = default!;
		public String                            Icon                  { get; set; } = default!;
		public Type                              MainPage              { get; set; } = default!;
		public Func<UIElement>                   SettingPanel          { get; set; } = default!;
		public Func<List<String>, List<String>?> GenerateForwardOption { get; set; } = default!;
		public Tuple<Integer, Integer>           StandardSize          { get; set; } = default!;
	}

	#endregion

	#region launcher

	public record ModuleLauncherConfiguration {
		public String       Title   { get; set; } = default!;
		public ModuleType   Type    { get; set; } = default!;
		public List<String> Option  { get; set; } = default!;
		public List<String> Command { get; set; } = default!;
	}

	// ----------------

	public enum ModuleLauncherCategory {
		Module,
		Pinned,
		Recent,
	}

	public record ModuleLauncherSetting {
		public List<ModuleLauncherConfiguration> Module { get; set; } = default!;
		public List<ModuleLauncherConfiguration> Pinned { get; set; } = default!;
		public List<ModuleLauncherConfiguration> Recent { get; set; } = default!;
	}

	#endregion

	#region helper

	public static class ModuleHelper {

		private static readonly List<ModuleInformation> Information = [
			new () {
				Type = ModuleType.CoreTaskWorker,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.CoreTaskWorker),
				Name = "Task Worker",
				Icon = FluentIconGlyph.ProvisioningPackage,
				MainPage = typeof(View.CoreTaskWorker.MainPage),
				SettingPanel = () => new View.CoreTaskWorker.SettingPanel() {
					Data = App.Instance.Setting.Data.CoreTaskWorker,
				},
				GenerateForwardOption = (resource) => {
					return ["-additional_argument", ..resource];
				},
				StandardSize = new (480, 840),
			},
			new () {
				Type = ModuleType.CoreCommandSender,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.CoreCommandSender),
				Name = "Command Sender",
				Icon = FluentIconGlyph.Send,
				MainPage = typeof(View.CoreCommandSender.MainPage),
				SettingPanel = () => new View.CoreCommandSender.SettingPanel() {
					Data = App.Instance.Setting.Data.CoreCommandSender,
				},
				GenerateForwardOption = (resource) => {
					return null;
				},
				StandardSize = new (920, 840),
			},
			new () {
				Type = ModuleType.CoreResourceShipper,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.CoreResourceShipper),
				Name = "Resource Shipper",
				Icon = FluentIconGlyph.Share,
				MainPage = typeof(View.CoreResourceShipper.MainPage),
				SettingPanel = () => new View.CoreResourceShipper.SettingPanel() {
					Data = App.Instance.Setting.Data.CoreResourceShipper,
				},
				GenerateForwardOption = (resource) => {
					foreach (var resourceItem in resource) {
						if (!StorageHelper.Exist(resourceItem)) {
							return null;
						}
					}
					return ["-resource", ..resource];
				},
				StandardSize = new (480, 840),
			},
			new () {
				Type = ModuleType.PopcapAnimationViewer,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.PopcapAnimationViewer),
				Name = "PopCap Animation Viewer",
				Icon = FluentIconGlyph.HomeGroup,
				MainPage = typeof(View.PopcapAnimationViewer.MainPage),
				SettingPanel = () => new View.PopcapAnimationViewer.SettingPanel() {
					Data = App.Instance.Setting.Data.PopcapAnimationViewer,
				},
				GenerateForwardOption = (resource) => {
					if (resource.Count != 1) {
						return null;
					}
					var animationFile = null as String;
					if (StorageHelper.ExistFile(resource[0])) {
						animationFile = PopcapAnimationHelper.CheckAnimationFilePath(resource[0]);
					}
					if (StorageHelper.ExistDirectory(resource[0])) {
						animationFile = PopcapAnimationHelper.CheckAnimationDirectoryPath(resource[0]);
					}
					if (animationFile == null) {
						return null;
					}
					return ["-animation_file", animationFile];
				},
				StandardSize = new (1600, 840),
			},
			new () {
				Type = ModuleType.PopcapReflectionDescriptor,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.PopcapReflectionDescriptor),
				Name = "PopCap Reflection Descriptor",
				Icon = FluentIconGlyph.Library,
				MainPage = typeof(View.PopcapReflectionDescriptor.MainPage),
				SettingPanel = () => new View.PopcapReflectionDescriptor.SettingPanel() {
					Data = App.Instance.Setting.Data.PopcapReflectionDescriptor,
				},
				GenerateForwardOption = (resource) => {
					if (resource.Count != 1) {
						return null;
					}
					if (!StorageHelper.ExistFile(resource[0])) {
						return null;
					}
					if (!new Regex(@"(\.json)$", RegexOptions.IgnoreCase).IsMatch(resource[0])) {
						return null;
					}
					return ["-descriptor_file", resource[0]];
				},
				StandardSize = new (920, 840),
			},
			new () {
				Type = ModuleType.PopcapPackageBuilder,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.PopcapPackageBuilder),
				Name = "PopCap Package Builder",
				Icon = FluentIconGlyph.DialShape3,
				MainPage = typeof(View.PopcapPackageBuilder.MainPage),
				SettingPanel = () => new View.PopcapPackageBuilder.SettingPanel() {
					Data = App.Instance.Setting.Data.PopcapPackageBuilder,
				},
				GenerateForwardOption = (resource) => {
					if (resource.Count != 1) {
						return null;
					}
					if (!StorageHelper.ExistDirectory(resource[0])) {
						return null;
					}
					if (!new Regex(@"(\.pvz2_package_project)$", RegexOptions.IgnoreCase).IsMatch(resource[0])) {
						return null;
					}
					return ["-project_directory", resource[0]];
				},
				StandardSize = new (1440, 840),
			},
		];

		public static ModuleInformation Query(
			ModuleType type
		) {
			return ModuleHelper.Information[type.CastPrimitive<Size>()];
		}

		// ----------------

		public static Boolean CompareLauncher(
			ModuleLauncherConfiguration thix,
			ModuleLauncherConfiguration that
		) {
			return thix.Type.Equals(that.Type) && thix.Option.SequenceEqual(that.Option);
		}

		public static List<String> GenerateArgument(
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
