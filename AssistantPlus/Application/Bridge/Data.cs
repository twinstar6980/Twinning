#pragma warning disable 0, 8981,
// ReSharper disable InconsistentNaming

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct Message {
		public IntegerU8*    data;
		public IntegerNative size;
	}

	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct Executor {
		public delegate* unmanaged<Executor*, Executor*, Message*, Message*, Message*, void> invoke;
		public delegate* unmanaged<Executor*, Executor*, Message*, Message*, Message*, void> clear;
	}

}
