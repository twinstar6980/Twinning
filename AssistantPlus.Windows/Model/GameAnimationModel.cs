#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;

namespace AssistantPlus {

	public static class GameAnimationModel {

		#region type

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record Rectangle {
			public List<Floater> Position = default!;
			public List<Floater> Size     = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record LayerRemove {
			public Integer Index = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record LayerAppend {
			public Integer Index        = default!;
			public String? Name         = default!;
			public Integer Resource     = default!;
			public Boolean Sprite       = default!;
			public Boolean Additive     = default!;
			public Integer PreloadFrame = default!;
			public Floater TimeScale    = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record LayerChange {
			public Integer        Index             = default!;
			public List<Floater>  Transform         = default!;
			public List<Floater>? Color             = default!;
			public Integer?       SpriteFrameNumber = default!;
			public Rectangle?     SourceRectangle   = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record Frame {
			public String?            Label   = default!;
			public Boolean            Stop    = default!;
			public List<List<String>> Command = default!;
			public List<LayerRemove>  Remove  = default!;
			public List<LayerAppend>  Append  = default!;
			public List<LayerChange>  Change  = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record Sprite {
			public String?        Name      = default!;
			public Floater?       FrameRate = default!;
			public List<Integer>? WorkArea  = default!;
			public List<Frame>    Frame     = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record Image {
			public String         Name      = default!;
			public List<Integer>? Size      = default!;
			public List<Floater>  Transform = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record Animation {
			public Integer       FrameRate  = default!;
			public List<Floater> Position   = default!;
			public List<Floater> Size       = default!;
			public List<Image>   Image      = default!;
			public List<Sprite>  Sprite     = default!;
			public Sprite?       MainSprite = default!;
		}

		#endregion

	}

}
