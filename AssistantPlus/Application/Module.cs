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
		public ModuleType                                   Type                  { get; set; } = default!;
		public String                                       Identifier            { get; set; } = default!;
		public String                                       Name                  { get; set; } = default!;
		public String                                       Icon                  { get; set; } = default!;
		public Func<Type>                                   GetMainPage           { get; set; } = default!;
		public Func<UIElement>                              BuildSettingPanel     { get; set; } = default!;
		public Func<List<StoragePath>, Task<List<String>?>> GenerateForwardOption { get; set; } = default!;
		public Tuple<Integer, Integer>                      StandardSize          { get; set; } = default!;
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
				GetMainPage = () => typeof(View.CoreTaskWorker.MainPage),
				BuildSettingPanel = () => new View.CoreTaskWorker.SettingPanel() {
					Data = App.Instance.Setting.Data.CoreTaskWorker,
				},
				GenerateForwardOption = async (resource) => {
					return ["-additional_argument", ..resource.Select((it) => it.Emit())];
				},
				StandardSize = new (480, 840),
			},
			new () {
				Type = ModuleType.CoreCommandSender,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.CoreCommandSender),
				Name = "Command Sender",
				Icon = FluentIconGlyph.Send,
				GetMainPage = () => typeof(View.CoreCommandSender.MainPage),
				BuildSettingPanel = () => new View.CoreCommandSender.SettingPanel() {
					Data = App.Instance.Setting.Data.CoreCommandSender,
				},
				GenerateForwardOption = async (resource) => {
					return null;
				},
				StandardSize = new (920, 840),
			},
			new () {
				Type = ModuleType.CoreResourceShipper,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.CoreResourceShipper),
				Name = "Resource Shipper",
				Icon = FluentIconGlyph.Share,
				GetMainPage = () => typeof(View.CoreResourceShipper.MainPage),
				BuildSettingPanel = () => new View.CoreResourceShipper.SettingPanel() {
					Data = App.Instance.Setting.Data.CoreResourceShipper,
				},
				GenerateForwardOption = async (resource) => {
					foreach (var resourceItem in resource) {
						if (!await StorageHelper.Exist(resourceItem)) {
							return null;
						}
					}
					return ["-resource", ..resource.Select((it) => it.Emit())];
				},
				StandardSize = new (480, 840),
			},
			new () {
				Type = ModuleType.PopcapAnimationViewer,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.PopcapAnimationViewer),
				Name = "PopCap Animation Viewer",
				Icon = FluentIconGlyph.HomeGroup,
				GetMainPage = () => typeof(View.PopcapAnimationViewer.MainPage),
				BuildSettingPanel = () => new View.PopcapAnimationViewer.SettingPanel() {
					Data = App.Instance.Setting.Data.PopcapAnimationViewer,
				},
				GenerateForwardOption = async (resource) => {
					if (resource.Count != 1) {
						return null;
					}
					var animationFile = null as StoragePath;
					if (await StorageHelper.ExistFile(resource.First())) {
						animationFile = await PopcapAnimationHelper.CheckAnimationFilePath(resource.First());
					}
					if (await StorageHelper.ExistDirectory(resource.First())) {
						animationFile = await PopcapAnimationHelper.CheckAnimationDirectoryPath(resource.First());
					}
					if (animationFile == null) {
						return null;
					}
					return ["-animation_file", animationFile.Emit()];
				},
				StandardSize = new (1600, 840),
			},
			new () {
				Type = ModuleType.PopcapReflectionDescriptor,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.PopcapReflectionDescriptor),
				Name = "PopCap Reflection Descriptor",
				Icon = FluentIconGlyph.Library,
				GetMainPage = () => typeof(View.PopcapReflectionDescriptor.MainPage),
				BuildSettingPanel = () => new View.PopcapReflectionDescriptor.SettingPanel() {
					Data = App.Instance.Setting.Data.PopcapReflectionDescriptor,
				},
				GenerateForwardOption = async (resource) => {
					if (resource.Count != 1) {
						return null;
					}
					if (!await StorageHelper.ExistFile(resource.First())) {
						return null;
					}
					if (!new Regex(@"(\.json)$", RegexOptions.IgnoreCase).IsMatch(resource.First().Name().AsNotNull())) {
						return null;
					}
					return ["-descriptor_file", resource.First().Emit()];
				},
				StandardSize = new (920, 840),
			},
			new () {
				Type = ModuleType.PopcapPackageBuilder,
				Identifier = ConvertHelper.MakeEnumerationToStringOfSnakeCase(ModuleType.PopcapPackageBuilder),
				Name = "PopCap Package Builder",
				Icon = FluentIconGlyph.DialShape3,
				GetMainPage = () => typeof(View.PopcapPackageBuilder.MainPage),
				BuildSettingPanel = () => new View.PopcapPackageBuilder.SettingPanel() {
					Data = App.Instance.Setting.Data.PopcapPackageBuilder,
				},
				GenerateForwardOption = async (resource) => {
					if (resource.Count != 1) {
						return null;
					}
					if (!await StorageHelper.ExistDirectory(resource.First())) {
						return null;
					}
					if (!new Regex(@"(\.pvz2_package_project)$", RegexOptions.IgnoreCase).IsMatch(resource.First().Name().AsNotNull())) {
						return null;
					}
					return ["-project_directory", resource.First().Emit()];
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
