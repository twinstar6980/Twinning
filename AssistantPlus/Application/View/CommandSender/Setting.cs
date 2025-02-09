#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.CommandSender {

	public record Setting {
		public String  MethodConfiguration = default!;
		public Boolean ParallelForward     = default!;
	}

}
