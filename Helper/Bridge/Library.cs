#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Bridge {

	public unsafe class Library {

		#region structor

		private SymbolTable mSymbol;
		private IntPtr      mHandle;

		// ----------------

		~Library (
		) {
			ExternalLibrary.Kernel32.FreeLibrary(this.mHandle);
		}

		// ----------------

		public Library (
			String path
		) {
			this.mHandle = ExternalLibrary.Kernel32.LoadLibrary($"{path}.");
			this.mSymbol = new SymbolTable() {
				version = Marshal.GetDelegateForFunctionPointer<Interface.version>(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.version)) ?? throw new NullReferenceException(),
				execute = Marshal.GetDelegateForFunctionPointer<Interface.execute>(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.execute)) ?? throw new NullReferenceException(),
				prepare = Marshal.GetDelegateForFunctionPointer<Interface.prepare>(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.prepare)) ?? throw new NullReferenceException(),
			};
		}

		#endregion

		#region interface

		public Interface.String* Version (
			Interface.Size** number
		) {
			return this.mSymbol.version(number);
		}

		public Interface.String* Execute (
			Interface.Callback**   callback,
			Interface.String**     script,
			Interface.StringList** argument,
			Interface.String**     result
		) {
			return this.mSymbol.execute(callback, script, argument, result);
		}

		public Interface.String* Prepare (
		) {
			return this.mSymbol.prepare();
		}

		#endregion

	}

}
