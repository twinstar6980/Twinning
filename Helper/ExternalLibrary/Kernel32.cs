#pragma warning disable 0,
// ReSharper disable

namespace Helper.ExternalLibrary {

	public static class Kernel32 {

		#region library

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

		#endregion

	}

}
