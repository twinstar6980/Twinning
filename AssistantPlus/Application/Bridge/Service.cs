#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.Bridge {

	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct Service {
		public Executor*                 executor;
		public delegate* unmanaged<void> initialize;
		public delegate* unmanaged<void> finalize;
	}

}
