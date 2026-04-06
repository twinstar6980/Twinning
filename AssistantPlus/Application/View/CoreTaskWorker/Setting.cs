#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreTaskWorker {

	public record Setting {
		public StoragePath  Kernel          { get; set; } = default!;
		public StoragePath  Script          { get; set; } = default!;
		public List<String> Argument        { get; set; } = default!;
		public Boolean      AutomaticScroll { get; set; } = default!;
		public Boolean      ImmediateLaunch { get; set; } = default!;
		public String       MessageFont     { get; set; } = default!;
	}

}
