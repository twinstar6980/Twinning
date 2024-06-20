#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus.View.ResourceForwarder {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PresetConfiguration {
		public String                     Name     = default!;
		public Dictionary<String, Object> Argument = default!;
	}

	public enum FilterType {
		Any,
		File,
		Directory,
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record FilterConfiguration {
		public FilterType Type = default!;
		public String     Name = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record OptionConfiguration {
		public String                     Name      = default!;
		public String                     Icon      = default!;
		public FilterConfiguration        Filter    = default!;
		public Boolean                    Batchable = default!;
		public String?                    Method    = default!;
		public List<PresetConfiguration?> Preset    = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record OptionGroupConfiguration {
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<OptionConfiguration> Item = default!;
	}

}
