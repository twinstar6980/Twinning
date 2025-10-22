#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CommandSender {

	public record Setting {
		public String  MethodConfiguration = default!;
		public Boolean ParallelForward     = default!;
	}

}
