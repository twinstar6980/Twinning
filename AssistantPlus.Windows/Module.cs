#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus {

	#region type

	public enum ModuleType {
		ModdingWorker,
		CommandSender,
		ResourceForwarder,
		ReflectionDescriptor,
		AnimationViewer,
		PackageBuilder,
	}

	// ----------------

	public record ModuleInformation {
		public ModuleType Type         = default!;
		public String     Icon         = default!;
		public String     Name         = default!;
		public Type       MainPage     = default!;
		public Type       SettingPanel = default!;
	}

	#endregion

	#region launcher

	[JsonObject(ItemRequired = Required.AllowNull)]
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

	[JsonObject(ItemRequired = Required.AllowNull)]
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
				Type = ModuleType.ResourceForwarder,
				Icon = FluentIconGlyph.Share,
				Name = "Resource Forwarder",
				MainPage = typeof(View.ResourceForwarder.MainPage),
				SettingPanel = typeof(View.ResourceForwarder.SettingPanel),
			},
			new () {
				Type = ModuleType.ReflectionDescriptor,
				Icon = FluentIconGlyph.Library,
				Name = "Reflection Descriptor",
				MainPage = typeof(View.ReflectionDescriptor.MainPage),
				SettingPanel = typeof(View.ReflectionDescriptor.SettingPanel),
			},
			new () {
				Type = ModuleType.AnimationViewer,
				Icon = FluentIconGlyph.HomeGroup,
				Name = "Animation Viewer",
				MainPage = typeof(View.AnimationViewer.MainPage),
				SettingPanel = typeof(View.AnimationViewer.SettingPanel),
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
				..launcher.Command,
				"-InitialTab",
				launcher.Title,
				launcher.Type.ToString(),
				..launcher.Option,
			];
		}

	}

	#endregion

}
