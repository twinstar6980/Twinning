#pragma warning disable 0,
// ReSharper disable InconsistentNaming

using AssistantPlus;

namespace AssistantPlus.Utility {

	public static class PlatformInvoke {

		public static class Kernel32 {

			[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
			public static extern UInt32 GetLongPathName (
				String        lpszShortPath,
				StringBuilder lpszLongPath,
				UInt32        cchBuffer
			);

			// ----------------

			[DllImport("Kernel32.dll", CharSet = CharSet.None)]
			public static extern IntPtr GetProcessHeap (
			);

			[DllImport("Kernel32.dll", CharSet = CharSet.None)]
			public static extern IntPtr HeapAlloc (
				IntPtr  hHeap,
				UInt32  dwFlags,
				UIntPtr dwBytes
			);

			[DllImport("Kernel32.dll", CharSet = CharSet.None)]
			public static extern Boolean HeapFree (
				IntPtr hHeap,
				UInt32 dwFlags,
				IntPtr lpMem
			);

			// ----------------

			[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
			public static extern IntPtr AddDllDirectory (
				String NewDirectory
			);

			[DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
			public static extern IntPtr LoadLibrary (
				String lpLibFileName
			);

			[DllImport("Kernel32.dll", CharSet = CharSet.None)]
			public static extern Boolean FreeLibrary (
				IntPtr hLibModule
			);

			[DllImport("Kernel32.dll", CharSet = CharSet.None)]
			public static extern IntPtr GetProcAddress (
				IntPtr hModule,
				String lpProcName
			);

		}

		public static class Shell32 {

			[DllImport("Shell32.dll", CharSet = CharSet.Unicode)]
			public static extern IntPtr ShellExecute (
				IntPtr  hwnd,
				String? lpOperation,
				String  lpFile,
				String? lpParameters,
				String? lpDirectory,
				Int32   nShowCmd
			);

		}

		public static class User32 {

			public const Int32 SW_SHOWNORMAL = 1;

			public const Int32 SW_RESTORE = 9;

			[DllImport("User32.dll", CharSet = CharSet.None)]
			public static extern Boolean ShowWindow (
				IntPtr hWnd,
				Int32  nCmdShow
			);

			[DllImport("User32.dll", CharSet = CharSet.None)]
			public static extern Boolean SetForegroundWindow (
				IntPtr hWnd
			);

		}

	}

}
