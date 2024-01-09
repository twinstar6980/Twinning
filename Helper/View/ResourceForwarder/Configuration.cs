#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;
using Helper.Utility;
using Newtonsoft.Json;

namespace Helper.View.ResourceForwarder {

	#region type

	public enum OptionFilterFileObjectType {
		Any,
		File,
		Directory,
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class OptionFilterConfiguration {
		public OptionFilterFileObjectType Type = default!;
		public String                     Name = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class OptionPresetConfiguration {
		public String                     Name     = default!;
		public Dictionary<String, Object> Argument = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class OptionConfiguration {
		public String                           Name      = default!;
		public String                           Icon      = default!;
		public OptionFilterConfiguration        Filter    = default!;
		public Boolean                          Batchable = default!;
		public String?                          Method    = default!;
		public List<OptionPresetConfiguration?> Preset    = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class OptionGroupConfiguration {
		public String                    Name = default!;
		public String                    Icon = default!;
		public List<OptionConfiguration> Item = default!;
	}

	#endregion

}
