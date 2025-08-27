#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	public unsafe class Library {

		#region structor

		private Win32.Foundation.HMODULE? mHandle;

		private Win32.Foundation.FARPROC? mSymbol;

		// ----------------

		public Library (
		) {
			this.mHandle = null;
			this.mSymbol = null;
			return;
		}

		#endregion

		#region access

		public Boolean State (
		) {
			return this.mHandle != null;
		}

		// ----------------

		public void Open (
			String path
		) {
			GF.AssertTest(!this.State());
			var handle = Win32.PInvoke.LoadLibrary($"{path}.");
			GF.AssertTest(!handle.IsNull);
			var symbol = default(Win32.Foundation.FARPROC);
			try {
				symbol = Win32.PInvoke.GetProcAddress(handle, "_ZN8Twinning6Kernel9Interface7serviceE");
				GF.AssertTest(!symbol.IsNull);
			}
			catch (Exception) {
				Win32.PInvoke.FreeLibrary(handle);
				throw;
			}
			this.mHandle = handle;
			this.mSymbol = symbol;
			(*(Service*)this.mSymbol.AsNotNull().Value).initialize();
			return;
		}

		public void Close (
		) {
			GF.AssertTest(this.State());
			(*(Service*)this.mSymbol.AsNotNull().Value).finalize();
			this.mSymbol = null;
			Win32.PInvoke.FreeLibrary(this.mHandle.AsNotNull());
			this.mHandle = null;
			return;
		}

		// ----------------

		public ref Service Symbol (
		) {
			GF.AssertTest(this.State());
			return ref *(Service*)this.mSymbol.AsNotNull().Value;
		}

		#endregion

	}

}
