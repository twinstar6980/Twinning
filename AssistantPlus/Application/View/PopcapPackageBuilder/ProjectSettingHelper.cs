#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.PopcapPackageBuilder {

	public static class ProjectSettingHelper {

		#region version

		private static readonly Integer Version = 3;

		// ----------------

		public static async Task<Boolean> CheckVersionFile (
			String projectDirectory
		) {
			var result = true;
			try {
				var versionFile = $"{projectDirectory}/version.txt";
				var versionText = await StorageHelper.ReadFileText(versionFile);
				AssertTest(versionText == ProjectSettingHelper.Version.ToString());
			}
			catch (Exception) {
				result = false;
			}
			return result;
		}

		#endregion

		#region category

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
			var result = null as String;
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
