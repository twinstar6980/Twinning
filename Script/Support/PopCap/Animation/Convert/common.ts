namespace Twinning.Script.Support.PopCap.Animation.Convert {

	// ------------------------------------------------

	export function parse_image_file_name(
		value: string,
	): string {
		let result = value;
		let index_a_1 = result.indexOf('(');
		let index_a_2 = result.indexOf(')');
		if (index_a_1 !== -1 || index_a_2 !== -1) {
			assert_test(index_a_1 !== -1 && index_a_2 !== -1 && index_a_1 < index_a_2);
			result = result.slice(0, index_a_1) + result.slice(index_a_2 + 1);
		}
		let index_b_1 = result.indexOf('$');
		if (index_b_1 !== -1) {
			assert_test(index_a_1 === -1 && index_a_2 === -1);
			result = result.slice(index_b_1 + 1);
		}
		let index_c_1 = result.indexOf('[');
		let index_c_2 = result.indexOf(']');
		if (index_c_1 !== -1 || index_c_2 !== -1) {
			assert_test(index_c_1 !== -1 && index_c_2 !== -1 && index_c_1 < index_c_2);
			result = result.slice(0, index_c_1) + result.slice(index_c_2 + 1);
		}
		let index_d_1 = result.indexOf('|');
		if (index_d_1 !== -1) {
			result = result.slice(0, index_d_1);
		}
		return result;
	}

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

	export function convert_transform_from_standard_to_rotate(
		data: Transform,
	): Kernel.Tool.PopCap.Animation.Definition.JS_N.RotateTranslateTransform {
		assert_test(data[0] === data[3] && data[1] === -data[2]);
		let cos = data[0];
		let sin = data[1];
		let radian = Math.atan(sin / cos) + Math.PI * (cos >= 0 ? 0 : sin >= 0 ? +1 : -1);
		assert_test(number_is_equal(Math.sin(radian), sin, 1e-2) && number_is_equal(Math.cos(radian), cos, 1e-2));
		return [radian, data[4], data[5]];
	}

	export function convert_transform_from_rotate_to_standard(
		data: Kernel.Tool.PopCap.Animation.Definition.JS_N.RotateTranslateTransform,
	): Transform {
		let cos = Math.cos(data[0]);
		let sin = Math.sin(data[0]);
		return [
			cos,
			sin,
			-sin,
			cos,
			data[1],
			data[2],
		];
	}

	export function convert_transform_from_standard_to_variant(
		data: Transform,
	): Kernel.Tool.PopCap.Animation.Definition.JS_N.VariantTransform {
		if (data[0] === data[3] && data[1] === -data[2]) {
			if (data[0] === 1.0 && data[1] === 0.0) {
				return [data[4], data[5]];
			}
			let cos = data[0];
			let sin = data[1];
			let radian = Math.atan(sin / cos) + Math.PI * (cos >= 0 ? 0 : sin >= 0 ? +1 : -1);
			if (number_is_equal(Math.sin(radian), sin, 1e-2) && number_is_equal(Math.cos(radian), cos, 1e-2)) {
				return [radian, data[4], data[5]];
			}
		}
		return [...data];
	}

	export function convert_transform_from_variant_to_standard(
		transform: Kernel.Tool.PopCap.Animation.Definition.JS_N.VariantTransform,
	): Transform {
		let result: Transform;
		if (transform.length === 2) {
			result = [
				1.0, 0.0, 0.0, 1.0,
				transform[0],
				transform[1],
			];
		}
		else if (transform.length === 3) {
			let cos = Math.cos(transform[0]);
			let sin = Math.sin(transform[0]);
			result = [
				cos,
				sin,
				-sin,
				cos,
				transform[1],
				transform[2],
			];
		}
		else if (transform.length === 6) {
			result = [...transform];
		}
		else {
			assert_fail(`invalid transform size`);
		}
		return result;
	}

	// ------------------------------------------------

	export type Color = [number, number, number, number];

	export const k_initial_color: Color = [1.0, 1.0, 1.0, 1.0];

	// ------------------------------------------------

}