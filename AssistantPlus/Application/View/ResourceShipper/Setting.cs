#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.ResourceShipper {

	public record Setting {
		public String  OptionConfiguration = default!;
		public Boolean ParallelForward     = default!;
		public Boolean EnableFilter        = default!;
		public Boolean EnableBatch         = default!;
	}

}
