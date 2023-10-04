#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Bridge {

	public unsafe class Library {

		#region structor

		private IntPtr? mHandle = null;

		private SymbolTable? mSymbol = null;

		#endregion

		#region state

		public Boolean State (
		) {
			return this.mHandle != null;
		}

		public void Open (
			String path
		) {
			Debug.Assert(!this.State());
			var handle = ExternalLibrary.Kernel32.LoadLibrary($"{path}.");
			var symbol = new SymbolTable() {
				version = null!,
				execute = null!,
				prepare = null!,
			};
			try {
				symbol.version = Marshal.GetDelegateForFunctionPointer<Interface.version>(ExternalLibrary.Kernel32.GetProcAddress(handle, SymbolNameTable.version));
				symbol.execute = Marshal.GetDelegateForFunctionPointer<Interface.execute>(ExternalLibrary.Kernel32.GetProcAddress(handle, SymbolNameTable.execute));
				symbol.prepare = Marshal.GetDelegateForFunctionPointer<Interface.prepare>(ExternalLibrary.Kernel32.GetProcAddress(handle, SymbolNameTable.prepare));
			} catch (Exception) {
				ExternalLibrary.Kernel32.FreeLibrary(handle);
				throw;
			}
			this.mHandle = handle;
			this.mSymbol = symbol;
			return;
		}

		public void Close (
		) {
			Debug.Assert(this.State());
			ExternalLibrary.Kernel32.FreeLibrary(this.mHandle!.Value);
			this.mHandle = null;
			this.mSymbol = null;
			return;
		}

		#endregion

		#region interface

		public Interface.String* Version (
			Interface.Size** number
		) {
			Debug.Assert(this.State());
			return this.mSymbol!.version(number);
		}

		public Interface.String* Execute (
			Interface.Callback**   callback,
			Interface.String**     script,
			Interface.StringList** argument,
			Interface.String**     result
		) {
			Debug.Assert(this.State());
			return this.mSymbol!.execute(callback, script, argument, result);
		}

		public Interface.String* Prepare (
		) {
			Debug.Assert(this.State());
			return this.mSymbol!.prepare();
		}

		#endregion

	}

}
