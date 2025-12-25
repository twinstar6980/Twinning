#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreResourceShipper {

	public record PresetConfiguration {
		public String                     Name     = default!;
		public Dictionary<String, Object> Argument = default!;
	}

	public enum FilterType {
		Any,
		File,
		Directory,
	}

	public record FilterConfiguration {
		public FilterType Type = default!;
		public String     Name = default!;
	}

	public record OptionConfiguration {
		public String                     Name   = default!;
		public String                     Icon   = default!;
		public FilterConfiguration?       Filter = default!;
		public Boolean                    Batch  = default!;
		public String?                    Method = default!;
		public List<PresetConfiguration?> Preset = default!;
	}

	public record OptionGroupConfiguration {
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<OptionConfiguration> Item = default!;
	}

	public record Configuration {
		public List<OptionGroupConfiguration> Option = default!;
	}

}
