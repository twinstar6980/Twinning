#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.ExternalLibrary {

	public static class Shell32 {

		[DllImport("Shell32.dll", CharSet = CharSet.Unicode)]
		public static extern IntPtr CommandLineToArgv (
			String    lpCmdLine,
			ref Int32 pNumArgs
		);

	}

}
