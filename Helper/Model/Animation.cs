#pragma warning disable 0,
// ReSharper disable InconsistentNaming

using Helper;

namespace Helper.Model {

	#region type

	public record Rectangle {
		public required List<Floating> position;
		public required List<Floating> size;
	}

	public record LayerRemove {
		public required Integer index;
	}

	public record LayerAppend {
		public required Integer  index;
		public required String?  name;
		public required Integer  resource;
		public required Boolean  sprite;
		public required Boolean  additive;
		public required Integer  preload_frame;
		public required Floating time_scale;
	}

	public record LayerChange {
		public required Integer         index;
		public required List<Floating>  transform;
		public required List<Floating>? color;
		public required Integer?        sprite_frame_number;
		public required Rectangle?      source_rectangle;
	}

	public record Frame {
		public required String?            label;
		public required Boolean            stop;
		public required List<List<String>> command;
		public required List<LayerRemove>  remove;
		public required List<LayerAppend>  append;
		public required List<LayerChange>  change;
	}

	public record Sprite {
		public required String        name;
		public required Floating      frame_rate;
		public required List<Integer> work_area;
		public required List<Frame>   frame;
	}

	public record Image {
		public required String         name;
		public required List<Integer>  size;
		public required List<Floating> transform;
	}

	public record Animation {
		public required Integer        frame_rate;
		public required List<Floating> position;
		public required List<Floating> size;
		public required List<Image>    image;
		public required List<Sprite>   sprite;
		public required Sprite?        main_sprite;
	}

	#endregion

}
