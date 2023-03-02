#pragma warning disable 0, 8981,
// ReSharper disable InconsistentNaming

using Helper;

namespace Helper.Core {

	public static unsafe class Interface {

		#region interface

		[StructLayout(LayoutKind.Sequential)]
		public struct Size {
			public IntegerU64 value;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct Character {
			public IntegerU8 value;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct String {
			public Character* data;
			public Size       size;
			public Size       capacity;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct StringList {
			public String* data;
			public Size    size;
			public Size    capacity;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct Callback {
			public delegate* <StringList*, StringList*> value;
		}

		// ----------------

		public delegate Size* version (
		);

		public delegate String* execute (
			Callback*   callback,
			String*     script,
			StringList* argument
		);

		public delegate String* prepare (
		);

		#endregion

	}

}
