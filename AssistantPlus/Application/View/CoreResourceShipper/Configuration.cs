#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreResourceShipper {

	public record PresetConfiguration {
		public String                     Name     { get; set; } = default!;
		public Dictionary<String, Object> Argument { get; set; } = default!;
	}

	public enum FilterType {
		Any,
		File,
		Directory,
	}

	public record FilterConfiguration {
		public FilterType Type { get; set; } = default!;
		public String     Name { get; set; } = default!;
	}

	public record OptionConfiguration {
		public String                     Name   { get; set; } = default!;
		public String                     Icon   { get; set; } = default!;
		public FilterConfiguration?       Filter { get; set; } = default!;
		public Boolean                    Batch  { get; set; } = default!;
		public String?                    Method { get; set; } = default!;
		public List<PresetConfiguration?> Preset { get; set; } = default!;
	}

	public record OptionGroupConfiguration {
		public String                    Name { get; set; } = default!;
		public String                    Icon { get; set; } = default!;
		public List<OptionConfiguration> Item { get; set; } = default!;
	}

	public record Configuration {
		public List<OptionGroupConfiguration> Option { get; set; } = default!;
	}

}
