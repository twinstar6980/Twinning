#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class ConvertHelper {

		#region opacity

		public static Floater BooleanToFloaterOfOpacity (
			Boolean state
		) {
			return !state ? 0.5 : 1.0;
		}

		#endregion

		#region switch

		public static String BooleanToSwitchString (
			Boolean state
		) {
			return !state ? "Off" : "On";
		}

		#endregion

	}

}
