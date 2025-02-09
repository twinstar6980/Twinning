#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.AnimationViewer {

	public record Setting {
		public Boolean ImmediateSelect  = default!;
		public Boolean AutomaticPlay    = default!;
		public Boolean RepeatPlay       = default!;
		public Boolean KeepSpeed        = default!;
		public Boolean ShowBoundary     = default!;
		public String  ImageFilterRule  = default!;
		public String  SpriteFilterRule = default!;
	}

}
