#pragma warning disable 0,

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreCommandSender {

	public record PresetConfiguration {
		public String                     Name     { get; set; } = default!;
		public Dictionary<String, Object> Argument { get; set; } = default!;
	}

	public enum ArgumentType {
		Boolean,
		Integer,
		Floater,
		String,
		Size,
		Path,
	}

	public record ArgumentConfiguration {
		public String        Identifier { get; set; } = default!;
		public String        Name       { get; set; } = default!;
		public ArgumentType  Type       { get; set; } = default!;
		public List<Object>? Option     { get; set; } = default!;
	}

	public record MethodConfiguration {
		public String                      Identifier { get; set; } = default!;
		public String                      Name       { get; set; } = default!;
		public String                      Icon       { get; set; } = default!;
		public List<ArgumentConfiguration> Argument   { get; set; } = default!;
		public List<String>?               Batch      { get; set; } = default!;
		public List<PresetConfiguration?>  Preset     { get; set; } = default!;
	}

	public record MethodGroupConfiguration {
		public String                    Identifier { get; set; } = default!;
		public String                    Name       { get; set; } = default!;
		public String                    Icon       { get; set; } = default!;
		public List<MethodConfiguration> Item       { get; set; } = default!;
	}

	public record Configuration {
		public List<MethodGroupConfiguration> Method { get; set; } = default!;
	}

}
