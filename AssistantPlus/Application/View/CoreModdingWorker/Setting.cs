#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreModdingWorker {

	public record Setting {
		public String       Kernel          = default!;
		public String       Script          = default!;
		public List<String> Argument        = default!;
		public Boolean      AutomaticScroll = default!;
		public Boolean      ImmediateLaunch = default!;
		public String       MessageFont     = default!;
	}

}
