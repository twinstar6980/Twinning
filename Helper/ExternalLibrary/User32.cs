#pragma warning disable 0,
// ReSharper disable

namespace Helper.ExternalLibrary {

	public static class User32 {

		#region symbol

		[DllImport("User32.dll")]
		public static extern Boolean ShowWindow (
			IntPtr hWnd,
			Int32  nCmdShow
		);

		[DllImport("User32.dll")]
		public static extern Boolean SetForegroundWindow (
			IntPtr hWnd
		);

		#endregion

	}

}
