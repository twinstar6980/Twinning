#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	public unsafe class Library {

		#region structor

		private IntPtr? mHandle;

		private Service* mSymbol;

		// ----------------

		public Library (
		) {
			this.mHandle = null;
			this.mSymbol = null;
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
			var handle = PlatformInvoke.Kernel32.LoadLibrary($"{path}.");
			var symbol = default(Service*);
			try {
				symbol = (Service*)PlatformInvoke.Kernel32.GetProcAddress(handle, "?service@Interface@Kernel@Twinning@@3UService@123@A");
			}
			catch (Exception) {
				PlatformInvoke.Kernel32.FreeLibrary(handle);
				throw;
			}
			this.mHandle = handle;
			this.mSymbol = symbol;
			this.mSymbol->initialize();
			return;
		}

		public void Close (
		) {
			GF.AssertTest(this.State());
			this.mSymbol->finalize();
			this.mSymbol = null;
			PlatformInvoke.Kernel32.FreeLibrary(this.mHandle.AsNotNull());
			this.mHandle = null;
			return;
		}

		// ----------------

		public Service Symbol (
		) {
			GF.AssertTest(this.State());
			return *this.mSymbol;
		}

		#endregion

	}

}
