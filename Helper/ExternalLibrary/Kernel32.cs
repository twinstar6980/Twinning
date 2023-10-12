#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.ExternalLibrary {

	public static class Kernel32 {

		[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
		public static extern IntPtr LoadLibrary (
			String lpLibFileName
		);

		[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
		public static extern Boolean FreeLibrary (
			IntPtr hLibModule
		);

		[DllImport("Kernel32.dll", CharSet = CharSet.Ansi)]
		public static extern IntPtr GetProcAddress (
			IntPtr hModule,
			String lpProcName
		);

		// ----------------

		[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
		public static extern IntPtr GetProcessHeap (
		);

		[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
		public static extern IntPtr HeapAlloc (
			IntPtr  hHeap,
			UInt32  dwFlags,
			UIntPtr dwBytes
		);

		[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
		public static extern Boolean HeapFree (
			IntPtr hHeap,
			UInt32 dwFlags,
			IntPtr lpMem
		);

	}

}
