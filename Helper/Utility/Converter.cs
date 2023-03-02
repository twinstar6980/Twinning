#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class Converter {

		#region opacity

		public static Floating BooleanToFloatingOfOpacity (
			Boolean state
		) {
			return state ? 1.0 : 0.5;
		}

		#endregion

	}

}
