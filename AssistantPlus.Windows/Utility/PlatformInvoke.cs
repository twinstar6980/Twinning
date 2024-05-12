#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public static class PlatformInvoke {

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

		public static class User32 {

			[DllImport("User32.dll", CharSet = CharSet.Unicode)]
			public static extern Boolean ShowWindow (
				IntPtr hWnd,
				Int32  nCmdShow
			);

			[DllImport("User32.dll", CharSet = CharSet.Unicode)]
			public static extern Boolean SetForegroundWindow (
				IntPtr hWnd
			);

		}

	}

}
