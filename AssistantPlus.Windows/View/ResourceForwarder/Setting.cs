#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus.View.ResourceForwarder {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record Setting {
		public String  OptionConfiguration = default!;
		public Boolean ParallelExecute     = default!;
		public Boolean EnableFilter        = default!;
		public Boolean EnableBatch         = default!;
		public Boolean RemainInput         = default!;
	}

}
