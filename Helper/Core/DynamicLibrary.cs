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
				version = (Interface.version)Marshal.GetDelegateForFunctionPointer(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.version), typeof(Interface.version)),
				execute = (Interface.execute)Marshal.GetDelegateForFunctionPointer(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.execute), typeof(Interface.execute)),
				prepare = (Interface.prepare)Marshal.GetDelegateForFunctionPointer(ExternalLibrary.Kernel32.GetProcAddress(this.mHandle, SymbolNameTable.prepare), typeof(Interface.prepare)),
			};
		}

		#endregion

		#region interface

		public override Interface.String* Version (
			Interface.Size** number
		) {
			return this.mSymbol.version(number);
		}

		public override Interface.String* Execute (
			Interface.Callback**   callback,
			Interface.String**     script,
			Interface.StringList** argument,
			Interface.String**     result
		) {
			return this.mSymbol.execute(callback, script, argument, result);
		}

		public override Interface.String* Prepare (
		) {
			return this.mSymbol.prepare();
		}

		#endregion

	}

}
