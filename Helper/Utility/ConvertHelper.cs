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

		#endregion

		#region theme

		public static String ThemeToString (
			ElementTheme value
		) {
			return value switch {
				ElementTheme.Default => "System",
				ElementTheme.Light   => "Light",
				ElementTheme.Dark    => "Dark",
				_                    => throw new ArgumentOutOfRangeException(),
			};
		}

		#endregion

		#region string list

		public static String StringListToTextWithCr (
			List<String> value
		) {
			return String.Join('\r', value) + (value.Count != 0 && value.Last().Length == 0 ? "\r" : "");
		}

		public static List<String> StringListFromTextWithCr (
			String text
		) {
			var value = text.Split('\r').ToList();
			if (value.Count != 0 && value.Last().Length == 0) {
				value.RemoveAt(value.Count - 1);
			}
			return value;
		}

		#endregion

	}

}
