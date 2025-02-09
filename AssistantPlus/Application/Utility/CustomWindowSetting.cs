#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public record CustomWindowPosition {
		public Boolean State = default!;
		public Integer X     = default!;
		public Integer Y     = default!;
	}

	public record CustomWindowSize {
		public Boolean State  = default!;
		public Integer Width  = default!;
		public Integer Height = default!;
	}

	// ----------------

	public record CustomWindowSetting {
		public CustomWindowPosition Position = default!;
		public CustomWindowSize     Size     = default!;
	}

}
