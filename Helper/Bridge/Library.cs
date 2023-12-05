#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Bridge {

	public unsafe class Library {

		#region structor

		private IntPtr? mHandle = null;

		private SymbolTable? mSymbol = null;

		// ----------------

		public Library (
		) {
		}

		#endregion

		#region state

		public Boolean State (
		) {
			return this.mHandle is not null;
		}

		public void Open (
			String path
		) {
			GF.AssertTest(!this.State());
			var handle = ExternalLibrary.Kernel32.LoadLibrary($"{path}.");
			var symbol = new SymbolTable() {
				execute = null!,
			};
			try {
				symbol.execute = Marshal.GetDelegateForFunctionPointer<Interface.execute>(ExternalLibrary.Kernel32.GetProcAddress(handle, SymbolNameTable.execute));
			}
			catch (Exception) {
				ExternalLibrary.Kernel32.FreeLibrary(handle);
				throw;
			}
			this.mHandle = handle;
			this.mSymbol = symbol;
			return;
		}

		public void Close (
		) {
			GF.AssertTest(this.State());
			var result = ExternalLibrary.Kernel32.FreeLibrary(this.mHandle.AsNotNull());
			GF.AssertTest(result);
			this.mHandle = null;
			this.mSymbol = null;
			return;
		}

		#endregion

		#region interface

		public Interface.String* Execute (
			Interface.Callback**   callback,
			Interface.String**     script,
			Interface.StringList** argument,
			Interface.String**     result
		) {
			GF.AssertTest(this.State());
			return this.mSymbol.AsNotNull().execute(callback, script, argument, result);
		}

		#endregion

	}

}
