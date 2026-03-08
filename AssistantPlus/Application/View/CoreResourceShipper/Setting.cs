#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreResourceShipper {

	public record Setting {
		public Boolean ParallelForward { get; set; } = default!;
		public Boolean EnableFilter    { get; set; } = default!;
		public Boolean EnableBatch     { get; set; } = default!;
	}

}
