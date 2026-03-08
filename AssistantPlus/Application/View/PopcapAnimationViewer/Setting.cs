#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.PopcapAnimationViewer {

	public record Setting {
		public Boolean ImmediateSelect  { get; set; } = default!;
		public Boolean AutomaticPlay    { get; set; } = default!;
		public Boolean RepeatPlay       { get; set; } = default!;
		public Boolean KeepSpeed        { get; set; } = default!;
		public Boolean ShowBoundary     { get; set; } = default!;
		public String  ImageFilterRule  { get; set; } = default!;
		public String  SpriteFilterRule { get; set; } = default!;
	}

}
