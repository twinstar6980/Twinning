namespace TwinStar.Script.Support.PopCap.PAM.Convert {

	// ------------------------------------------------

	export type Transform = [number, number, number, number, number, number];

	export const k_initial_transform: Transform = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0];

	export function mix_transform(
		source: Transform,
		change: Transform,
	): Transform {
		return [
			change[0] * source[0] + change[2] * source[1],
			change[1] * source[0] + change[3] * source[1],
			change[0] * source[2] + change[2] * source[3],
			change[1] * source[2] + change[3] * source[3],
			change[0] * source[4] + change[2] * source[5] + change[4],
			change[1] * source[4] + change[3] * source[5] + change[5],
		];
	}

	export function compute_standard_transform_from_variant(
		transform: Core.Tool.PopCap.PAM.Manifest.JS_N.VariantTransform,
	): Transform {
		let result: Transform;
		if (transform.length === 2) {
			result = [
				1.0, 0.0, 0.0, 1.0,
				transform[1 - 1],
				transform[2 - 1],
			];
		} else if (transform.length === 3) {
			let cos_value = Math.cos(transform[1 - 1]);
			let sin_value = Math.sin(transform[1 - 1]);
			result = [
				cos_value,
				sin_value,
				-sin_value,
				cos_value,
				transform[2 - 1],
				transform[3 - 1],
			];
		} else if (transform.length === 6) {
			result = [...transform];
		} else {
			assert_test(false, `invalid transform size`);
		}
		return result;
	}

	export function compute_variant_transform_from_standard(
		data: Transform,
	): Core.Tool.PopCap.PAM.Manifest.JS_N.VariantTransform {
		if (data[0] === data[3] && data[1] === -data[2]) {
			if (data[0] === 1.0 && data[1] === 0.0) {
				return [data[4], data[5]];
			}
			let acos_value = Math.acos(data[0]);
			let asin_value = Math.asin(data[1]);
			if (Math.abs(Math.abs(acos_value) - Math.abs(asin_value)) <= 1e-2) {
				return [asin_value, data[4], data[5]];
			}
		}
		return [...data];
	}

	// ------------------------------------------------

	export type Color = [number, number, number, number];

	export const k_initial_color: Color = [1.0, 1.0, 1.0, 1.0];

	// ------------------------------------------------

}