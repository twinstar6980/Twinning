#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using Newtonsoft.Json;

namespace AssistantPlus.Utility {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record CustomWindowPosition {
		public Boolean State = default!;
		public Integer X     = default!;
		public Integer Y     = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record CustomWindowSize {
		public Boolean State  = default!;
		public Integer Width  = default!;
		public Integer Height = default!;
	}

	// ----------------

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record CustomWindowSetting {
		public CustomWindowPosition Position = default!;
		public CustomWindowSize     Size     = default!;
	}

}
