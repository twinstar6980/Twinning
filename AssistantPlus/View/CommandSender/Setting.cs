#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus.View.CommandSender {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record Setting {
		public String  MethodConfiguration = default!;
		public Boolean ParallelForward     = default!;
	}

}
