#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;
using Newtonsoft.Json;

namespace Helper {

	public static class AnimationModel {

		#region type

		[JsonObject(ItemRequired = Required.Default)]
		public record Rectangle {
			public required List<Floater> Position;
			public required List<Floater> Size;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.Default)]
		public record LayerRemove {
			public required Integer Index;
		}

		[JsonObject(ItemRequired = Required.Default)]
		public record LayerAppend {
			public required Integer Index;
			public required String? Name;
			public required Integer Resource;
			public required Boolean Sprite;
			public required Boolean Additive;
			public required Integer PreloadFrame;
			public required Floater TimeScale;
		}

		[JsonObject(ItemRequired = Required.Default)]
		public record LayerChange {
			public required Integer        Index;
			public required List<Floater>  Transform;
			public required List<Floater>? Color;
			public required Integer?       SpriteFrameNumber;
			public required Rectangle?     SourceRectangle;
		}

		[JsonObject(ItemRequired = Required.Default)]
		public record Frame {
			public required String?            Label;
			public required Boolean            Stop;
			public required List<List<String>> Command;
			public required List<LayerRemove>  Remove;
			public required List<LayerAppend>  Append;
			public required List<LayerChange>  Change;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.Default)]
		public record Sprite {
			public required String?        Name;
			public required Floater?       FrameRate;
			public required List<Integer>? WorkArea;
			public required List<Frame>    Frame;
		}

		[JsonObject(ItemRequired = Required.Default)]
		public record Image {
			public required String         Name;
			public required List<Integer>? Size;
			public required List<Floater>  Transform;
		}

		[JsonObject(ItemRequired = Required.Default)]
		public record Animation {
			public required Integer       FrameRate;
			public required List<Floater> Position;
			public required List<Floater> Size;
			public required List<Image>   Image;
			public required List<Sprite>  Sprite;
			public required Sprite?       MainSprite;
		}

		#endregion

	}

}
