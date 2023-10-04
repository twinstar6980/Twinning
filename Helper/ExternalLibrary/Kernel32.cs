#pragma warning disable 0,
// ReSharper disable

namespace Helper.ExternalLibrary {

	public static unsafe class Kernel32 {

		#region symbol

		[DllImport("Kernel32.dll")]
		public static extern IntPtr LoadLibrary (
			String lpLibFileName
		);

		[DllImport("Kernel32.dll")]
		public static extern Boolean FreeLibrary (
			IntPtr hLibModule
		);

		[DllImport("Kernel32.dll")]
		public static extern IntPtr GetProcAddress (
			IntPtr hModule,
			String lpProcName
		);

		// ----------------

		[DllImport("Kernel32.dll")]
		public static extern IntPtr GetProcessHeap (
		);

		[DllImport("Kernel32.dll")]
		public static extern void* HeapAlloc (
			IntPtr  hHeap,
			Int32   dwFlags,
			UIntPtr dwBytes
		);

		[DllImport("Kernel32.dll")]
		public static extern Boolean HeapFree (
			IntPtr hHeap,
			Int32  dwFlags,
			void*  lpMem
		);

		#endregion

	}

}
