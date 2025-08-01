#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct Message {
		public IntegerU8* data;
		public IntegerUN  size;
	}

	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct Executor {
		public delegate* unmanaged<Executor*, Executor*, Message*, Message*, Message*, void> invoke;
		public delegate* unmanaged<Executor*, Executor*, Message*, Message*, Message*, void> clear;
	}

}
