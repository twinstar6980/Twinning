#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus.View.CommandSender {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record ArgumentConfiguration {
		public String        Id      = default!;
		public String        Name    = default!;
		public ArgumentType  Type    = default!;
		public List<Object>? Option  = default!;
		public Object?       Initial = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record MethodConfiguration {
		public String                      Id        = default!;
		public String                      Name      = default!;
		public String                      Icon      = default!;
		public List<ArgumentConfiguration> Argument  = default!;
		public List<String>?               Batchable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record MethodGroupConfiguration {
		public String                    Id   = default!;
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<MethodConfiguration> Item = default!;
	}

}
