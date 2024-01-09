#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;
using Helper.Utility;
using Newtonsoft.Json;

namespace Helper.View.CommandSender {

	#region type

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class ArgumentConfiguration {
		public String        Id      = default!;
		public String        Name    = default!;
		public ArgumentType  Type    = default!;
		public List<Object>? Option  = default!;
		public Object?       Initial = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class MethodConfiguration {
		public String                      Id        = default!;
		public String                      Name      = default!;
		public String                      Icon      = default!;
		public List<ArgumentConfiguration> Argument  = default!;
		public List<String>?               Batchable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class MethodGroupConfiguration {
		public String                    Id   = default!;
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<MethodConfiguration> Item = default!;
	}

	#endregion

}
