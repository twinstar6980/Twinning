#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus.View.ResourceForwarder {

	public enum OptionFilterFileObjectType {
		Any,
		File,
		Directory,
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record OptionFilterConfiguration {
		public OptionFilterFileObjectType Type = default!;
		public String                     Name = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record OptionPresetConfiguration {
		public String                     Name     = default!;
		public Dictionary<String, Object> Argument = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record OptionConfiguration {
		public String                           Name      = default!;
		public String                           Icon      = default!;
		public OptionFilterConfiguration        Filter    = default!;
		public Boolean                          Batchable = default!;
		public String?                          Method    = default!;
		public List<OptionPresetConfiguration?> Preset    = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record OptionGroupConfiguration {
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<OptionConfiguration> Item = default!;
	}

}
