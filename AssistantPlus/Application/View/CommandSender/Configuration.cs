#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.CommandSender {

	public record PresetConfiguration {
		public String                     Name     = default!;
		public Dictionary<String, Object> Argument = default!;
	}

	public enum ArgumentType {
		Boolean,
		Integer,
		Floater,
		Size,
		String,
		Path,
	}

	public record ArgumentConfiguration {
		public String        Id     = default!;
		public String        Name   = default!;
		public ArgumentType  Type   = default!;
		public List<Object>? Option = default!;
	}

	public record MethodConfiguration {
		public String                      Id        = default!;
		public String                      Name      = default!;
		public String                      Icon      = default!;
		public List<ArgumentConfiguration> Argument  = default!;
		public List<String>?               Batchable = default!;
		public List<PresetConfiguration?>  Preset    = default!;
	}

	public record MethodGroupConfiguration {
		public String                    Id   = default!;
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<MethodConfiguration> Item = default!;
	}

}
