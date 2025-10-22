#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;

namespace Twinning.AssistantPlus.Utility {

	public static class ForwarderExtensionHelper {

		#region utility

		private static String StateFile {
			get {
				return $"{App.SharedDirectory}/Forwarder";
			}
		}

		// ----------------

		public static Boolean Check (
		) {
			return StorageHelper.Exist(ForwarderExtensionHelper.StateFile);
		}

		public static void Toggle (
		) {
			if (!StorageHelper.Exist(ForwarderExtensionHelper.StateFile)) {
				StorageHelper.CreateFile(ForwarderExtensionHelper.StateFile);
			}
			else {
				StorageHelper.Remove(ForwarderExtensionHelper.StateFile);
			}
			return;
		}

		#endregion

	}

}
