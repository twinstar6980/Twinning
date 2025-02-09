#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public enum CustomThemeMode {
		System,
		Light,
		Dark,
	}

	public record CustomThemeColor {
		public Boolean State      = default!;
		public Integer LightRed   = default!;
		public Integer LightGreen = default!;
		public Integer LightBlue  = default!;
		public Integer DarkRed    = default!;
		public Integer DarkGreen  = default!;
		public Integer DarkBlue   = default!;
	}

	public enum CustomThemeBackdrop {
		Solid,
		MicaBase,
		MicaAlt,
		AcrylicDesktop,
	}

	// ----------------

	public record CustomThemeSetting {
		public CustomThemeMode     Mode     = default!;
		public CustomThemeColor    Color    = default!;
		public CustomThemeBackdrop Backdrop = default!;
	}

}
