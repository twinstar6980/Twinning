#pragma warning disable 0,
// ReSharper disable InconsistentNaming

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct Service {
		public Executor*                 executor;
		public delegate* unmanaged<void> initialize;
		public delegate* unmanaged<void> finalize;
	}

}
