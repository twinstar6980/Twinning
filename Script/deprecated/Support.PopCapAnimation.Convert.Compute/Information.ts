/** PAM样式运算结果数据结构 */
namespace TwinKleS.Support.PopCapAnimation.Convert.Compute.Information {

	// ------------------------------------------------

	export type Image = {
		source: string;
		size: [bigint, bigint];
		transform: Transform;
	};

	export type Frame = {
		transform: Transform;
		color: Color;
	};

	export type Layer = {
		image: bigint;
		sprite: Array<bigint>;
		work_area: [bigint, bigint];
		frame: Array<Frame>;
	};

	export type Sprite = {
		name: string;
		frame_rate: number;
		frame_count: bigint;
		frame_label: Record<string, bigint>;
		frame_stop: Array<bigint>;
		layer: Record<string, Layer>;
	};

	export type Animation = {
		frame_rate: bigint;
		position: [number, number];
		size: [number, number];
		image: Array<Image>;
		sprite: Array<Sprite>;
		main_sprite: Sprite;
	};

	// ------------------------------------------------

}