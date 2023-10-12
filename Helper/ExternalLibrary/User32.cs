#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.ExternalLibrary {

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
