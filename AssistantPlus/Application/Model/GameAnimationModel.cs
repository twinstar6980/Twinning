#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus {

	public static class GameAnimationModel {

		#region type

		public record Rectangle {
			public Tuple<Floater, Floater> Position = default!;
			public Tuple<Floater, Floater> Size     = default!;
		}

		// ----------------

		public record LayerRemove {
			public Integer Index = default!;
		}

		public record LayerAppend {
			public Integer Index        = default!;
			public String? Name         = default!;
			public Integer Resource     = default!;
			public Boolean Sprite       = default!;
			public Boolean Additive     = default!;
			public Integer PreloadFrame = default!;
			public Floater TimeScale    = default!;
		}

		public record LayerChange {
			public Integer                                    Index             = default!;
			public List<Floater>                              Transform         = default!;
			public Tuple<Floater, Floater, Floater, Floater>? Color             = default!;
			public Integer?                                   SpriteFrameNumber = default!;
			public Rectangle?                                 SourceRectangle   = default!;
		}

		public record Frame {
			public String?                     Label   = default!;
			public Boolean                     Stop    = default!;
			public List<Tuple<String, String>> Command = default!;
			public List<LayerRemove>           Remove  = default!;
			public List<LayerAppend>           Append  = default!;
			public List<LayerChange>           Change  = default!;
		}

		// ----------------

		public record Sprite {
			public String?                  Name      = default!;
			public Floater?                 FrameRate = default!;
			public Tuple<Integer, Integer>? WorkArea  = default!;
			public List<Frame>              Frame     = default!;
		}

		public record Image {
			public String                   Name      = default!;
			public Tuple<Integer, Integer>? Size      = default!;
			public List<Floater>            Transform = default!;
		}

		public record Animation {
			public Integer                 FrameRate  = default!;
			public Tuple<Floater, Floater> Position   = default!;
			public Tuple<Floater, Floater> Size       = default!;
			public List<Image>             Image      = default!;
			public List<Sprite>            Sprite     = default!;
			public Sprite?                 MainSprite = default!;
		}

		#endregion

	}

}
