#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;

namespace Helper.Module.PackageBuilder {

	public static class ProjectSettingHelper {

		#region check

		public static Integer? ParseResolutionString (
			String text
		) {
			var result = default(Integer?);
			if (text.Length != 0 && text.All(Character.IsAsciiDigit)) {
				if (Integer.TryParse(text, out var resultValue)) {
					result = resultValue;
				}
				if (result == 0) {
					result = null;
				}
			}
			return result;
		}

		public static String? ParseLocaleString (
			String text
		) {
			var result = default(String?);
			if (text.Length == 4 && text.All(Character.IsAsciiLetter)) {
				result = text.ToUpperInvariant();
			}
			return result;
		}

		#endregion

	}

}
