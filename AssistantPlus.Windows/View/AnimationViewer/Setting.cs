#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus.View.AnimationViewer {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record Setting {
		public Boolean ImmediateSelect    = default!;
		public Boolean AutomaticPlay      = default!;
		public Boolean RepeatPlay         = default!;
		public Boolean RemainFrameRate    = default!;
		public Boolean ShowSpriteBoundary = default!;
		public String  ImageFilterRule    = default!;
		public String  SpriteFilterRule   = default!;
	}

}
