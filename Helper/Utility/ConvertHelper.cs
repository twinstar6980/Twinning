#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class ConvertHelper {

		#region boolean

		public static Floater BooleanToFloaterOfOpacityVisibility (
			Boolean value
		) {
			return value switch {
				false => 0.0,
				true  => 1.0,
			};
		}

		public static Floater BooleanToFloaterOfOpacityEnabled (
			Boolean value
		) {
			return value switch {
				false => 0.5,
				true  => 1.0,
			};
		}

		public static String BooleanToConfirmationStringLower (
			Boolean value
		) {
			return value switch {
				false => "no",
				true  => "yes",
			};
		}

		public static String BooleanToSwitchString (
			Boolean value
		) {
			return value switch {
				false => "Off",
				true  => "On",
			};
		}

		#endregion

		#region theme

		public static String ThemeToString (
			ElementTheme value
		) {
			return value switch {
				ElementTheme.Default => "System",
				ElementTheme.Light   => "Light",
				ElementTheme.Dark    => "Dark",
				_                    => throw new ArgumentOutOfRangeException(nameof(value), value, null),
			};
		}

		#endregion

	}

}
