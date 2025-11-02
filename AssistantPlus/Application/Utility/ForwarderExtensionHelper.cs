#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;

namespace Twinning.AssistantPlus.Utility {

	public static class ForwarderExtensionHelper {

		#region utility

		private static String StateFile {
			get {
				return $"{App.SharedDirectory}/forwarder";
			}
		}

		// ----------------

		public static Boolean Check (
		) {
			return StorageHelper.Exist(ForwarderExtensionHelper.StateFile);
		}

		public static void Toggle (
			Boolean state
		) {
			var exist = StorageHelper.Exist(ForwarderExtensionHelper.StateFile);
			if (!state && exist) {
				StorageHelper.Remove(ForwarderExtensionHelper.StateFile);
			}
			if (state && !exist) {
				StorageHelper.CreateFile(ForwarderExtensionHelper.StateFile);
			}
			return;
		}

		#endregion

	}

}
