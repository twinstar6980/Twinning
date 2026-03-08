#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus {

	public static class PopcapAnimationModel {

		#region type

		public record Rectangle {
			public Tuple<Floater, Floater> Position { get; set; } = default!;
			public Tuple<Floater, Floater> Size     { get; set; } = default!;
		}

		// ----------------

		public record LayerRemove {
			public Integer Index { get; set; } = default!;
		}

		public record LayerAppend {
			public Integer Index        { get; set; } = default!;
			public String? Name         { get; set; } = default!;
			public Integer Resource     { get; set; } = default!;
			public Boolean Sprite       { get; set; } = default!;
			public Boolean Additive     { get; set; } = default!;
			public Integer PreloadFrame { get; set; } = default!;
			public Floater TimeScale    { get; set; } = default!;
		}

		public record LayerChange {
			public Integer                                    Index             { get; set; } = default!;
			public List<Floater>                              Transform         { get; set; } = default!;
			public Tuple<Floater, Floater, Floater, Floater>? Color             { get; set; } = default!;
			public Integer?                                   SpriteFrameNumber { get; set; } = default!;
			public Rectangle?                                 SourceRectangle   { get; set; } = default!;
		}

		public record Frame {
			public String?                     Label   { get; set; } = default!;
			public Boolean                     Stop    { get; set; } = default!;
			public List<Tuple<String, String>> Command { get; set; } = default!;
			public List<LayerRemove>           Remove  { get; set; } = default!;
			public List<LayerAppend>           Append  { get; set; } = default!;
			public List<LayerChange>           Change  { get; set; } = default!;
		}

		// ----------------

		public record Sprite {
			public String?                  Name      { get; set; } = default!;
			public Floater?                 FrameRate { get; set; } = default!;
			public Tuple<Integer, Integer>? WorkArea  { get; set; } = default!;
			public List<Frame>              Frame     { get; set; } = default!;
		}

		public record Image {
			public String                   Name      { get; set; } = default!;
			public Tuple<Integer, Integer>? Size      { get; set; } = default!;
			public List<Floater>            Transform { get; set; } = default!;
		}

		public record Animation {
			public Integer                 FrameRate  { get; set; } = default!;
			public Tuple<Floater, Floater> Position   { get; set; } = default!;
			public Tuple<Floater, Floater> Size       { get; set; } = default!;
			public List<Image>             Image      { get; set; } = default!;
			public List<Sprite>            Sprite     { get; set; } = default!;
			public Sprite?                 MainSprite { get; set; } = default!;
		}

		#endregion

	}

}
