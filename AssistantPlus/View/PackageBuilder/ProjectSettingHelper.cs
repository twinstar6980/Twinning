#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public static class ProjectSettingHelper {

		#region check

		public static Integer? ParseResolutionString (
			String   text,
			Integer? fallback
		) {
			var result = default(Integer?);
			if (text.Length != 0) {
				if (text.All(Character.IsAsciiDigit) && Integer.TryParse(text, out var resultValue)) {
					if (resultValue != 0) {
						result = resultValue;
					}
				}
				else {
					result = fallback;
				}
			}
			return result;
		}

		public static String? ParseLocaleString (
			String  text,
			String? fallback
		) {
			var result = default(String?);
			if (text.Length != 0) {
				if (text.Length == 4 && text.All(Character.IsAsciiLetter)) {
					result = text.ToUpperInvariant();
				}
				else {
					result = fallback;
				}
			}
			return result;
		}

		#endregion

	}

}
