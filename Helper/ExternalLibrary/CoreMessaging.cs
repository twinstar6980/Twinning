#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.ExternalLibrary {

	public static class CoreMessaging {

		#region library

		[StructLayout(LayoutKind.Sequential)]
		public struct DispatcherQueueOptions {
			internal Int32 dwSize;
			internal Int32 threadType;
			internal Int32 apartmentType;
		}

		// ----------------

		[DllImport("CoreMessaging.dll")]
		public static extern Int32 CreateDispatcherQueueController (
			[In] DispatcherQueueOptions options,
			[In] [Out] [MarshalAs(UnmanagedType.IUnknown)]
			ref Object dispatcherQueueController
		);

		#endregion

	}

}
