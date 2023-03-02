#pragma warning disable 0,
// ReSharper disable

using Helper;
using ExternalLibrary = Helper.ExternalLibrary;

namespace Helper.Core {

	public unsafe class DynamicLibrary : Library {

		#region structor

		private SymbolTable mSymbol;
		private IntPtr      mHandle;

		// ----------------

		~DynamicLibrary (
		) {
			ExternalLibrary.Kernel32.FreeLibrary(this.mHandle);
		}

		// ----------------

		public DynamicLibrary (
			String path
		) {
			this.mHandle = ExternalLibrary.Kernel32.LoadLibrary($"{path}.");
			this.mSymbol = new SymbolTable() {
				version = Marshal.GetDelegateForFunctionPointer(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.version), typeof(Interface.version)) as Interface.version ?? throw new Exception($"can not found symbol : {SymbolNameTable.version}"),
				execute = Marshal.GetDelegateForFunctionPointer(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.execute), typeof(Interface.execute)) as Interface.execute ?? throw new Exception($"can not found symbol : {SymbolNameTable.execute}"),
				prepare = Marshal.GetDelegateForFunctionPointer(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.prepare), typeof(Interface.prepare)) as Interface.prepare ?? throw new Exception($"can not found symbol : {SymbolNameTable.prepare}"),
			};
		}

		#endregion

		#region interface

		public override Interface.Size* Version (
		) {
			return this.mSymbol.version();
		}

		public override Interface.String* Execute (
			Interface.Callback*   callback,
			Interface.String*     script,
			Interface.StringList* argument
		) {
			return this.mSymbol.execute(callback, script, argument);
		}

		public override Interface.String* Prepare (
		) {
			return this.mSymbol.prepare();
		}

		#endregion

	}

}
