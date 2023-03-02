#pragma warning disable 0,
// ReSharper disable

using Helper;
using ExternalLibrary = Helper.ExternalLibrary;

namespace Helper.Utility {

	public class WindowsSystemDispatcherQueueHelper {

		#region utility

		private Object? mDispatcherQueueController = null;

		// ----------------

		public void EnsureWindowsSystemDispatcherQueueController () {
			if (Windows.System.DispatcherQueue.GetForCurrentThread() != null) {
				return;
			}
			if (this.mDispatcherQueueController == null) {
				var options = new ExternalLibrary.CoreMessaging.DispatcherQueueOptions {
					dwSize = Marshal.SizeOf(typeof(ExternalLibrary.CoreMessaging.DispatcherQueueOptions)),
					threadType = 2, // DQTYPE_THREAD_CURRENT
					apartmentType = 2, // DQTAT_COM_STA
				};
				ExternalLibrary.CoreMessaging.CreateDispatcherQueueController(options, ref this.mDispatcherQueueController!);
			}
			return;
		}

		#endregion

	}

}
