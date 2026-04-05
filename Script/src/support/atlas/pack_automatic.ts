namespace Twinning.Script.Support.Atlas.PackAutomatic {

	// #region common

	export type Box = {
		width: number;
		height: number;
	};

	export type Rect = {
		x: number;
		y: number;
	} & Box;

	// ----------------

	function box_is_fit(
		item: Box,
		container: Box,
	): boolean {
		return item.width <= container.width && item.height <= container.height;
	}

	function box_is_empty(
		t: Box,
	): boolean {
		return t.width === 0 || t.height === 0;
	}

	function box_compare_asc(
		a: Box,
		b: Box,
	): number {
		let a_area = a.width * a.height;
		let b_area = b.width * b.height;
		if (a_area === b_area) {
			return 0;
		}
		else if (a_area < b_area) {
			return -1;
		}
		else {
			return +1;
		}
	}

	// ----------------

	function box_area(
		t: Box,
	): number {
		return t.width * t.height;
	}

	function box_perimeter(
		t: Box,
	): number {
		return 2 * (t.width + t.height);
	}

	function box_bigger_side(
		t: Box,
	): number {
		return Math.max(t.width, t.height);
	}

	function box_smaller_side(
		t: Box,
	): number {
		return Math.min(t.width, t.height);
	}

	function box_width(
		t: Box,
	): number {
		return t.width;
	}

	function box_height(
		t: Box,
	): number {
		return t.height;
	}

	function box_pathological_multiplier(
		t: Box,
	): number {
		return box_bigger_side(t) / box_smaller_side(t) * box_area(t);
	}

	// ----------------

	export function expander_exponent_of_2_generator(
		square: boolean,
	): (it: Box, step: number) => void {
		return (it, step) => {
			if (square) {
				it.width = Math.max(it.width, 1) * 2;
				it.height = Math.max(it.height, 1) * 2;
			}
			else {
				if (step % 2 === 0) {
					it.height = Math.max(it.height, 1) * 2;
				}
				else {
					it.width = Math.max(it.width, 1) * 2;
				}
			}
		}
	}

	export function expander_fixed_generator(
		square: boolean,
		expand_value: number,
	): (it: Box, step: number) => void {
		return (it, step) => {
			if (square) {
				it.width += expand_value;
				it.height += expand_value;
			}
			else {
				if (step % 2 === 0) {
					it.width += expand_value;
				}
				else {
					it.height += expand_value;
				}
			}
		}
	}

	// #endregion

	// #region utility

	export function pack(
		item_map: Record<string, Box>,
		container: Box,
		item_weighter: (it: Box) => number,
	): null | Record<string, Rect> {
		let space_list: Array<Rect> = [{x: 0, y: 0, ...container}];
		let item_list = ConvertHelper.record_to_array(item_map, (key, value) => ({name: key, box: item_map[key]})).sort((a, b) => (item_weighter(a.box) > item_weighter(b.box) ? -1 : +1));
		let result: Record<string, Rect> = {};
		for (let item of item_list) {
			let box = item.box;
			let space_index = space_list.findIndex((value) => (box_is_fit(box, value)));
			if (space_index === -1) {
				return null;
			}
			let space = space_list[space_index];
			space_list.splice(space_index, 1);
			result[item.name] = {x: space.x, y: space.y, ...box};
			let reserve_width = space.width - box.width;
			let reserve_height = space.height - box.height;
			let new_space = [
				{
					x: space.x + box.width,
					y: space.y,
					width: reserve_width,
					height: reserve_width > reserve_height ? space.height : box.height,
				},
				{
					x: space.x,
					y: space.y + box.height,
					width: reserve_width > reserve_height ? box.width : space.width,
					height: reserve_height,
				},
			];
			if (!box_is_empty(new_space[0])) {
				space_list.push(new_space[0]);
			}
			if (!box_is_empty(new_space[1])) {
				space_list.push(new_space[1]);
			}
			space_list = space_list.sort(box_compare_asc);
		}
		return result;
	}

	// ----------------

	export function pack_automatic(
		item_map: Record<string, Box>,
		container_expander: (it: Box, step: number) => void,
		item_weighter: (it: Box) => number,
	): [Box, Record<string, Rect>] {
		let container: Box = {width: 0, height: 0};
		let result: null | Record<string, Rect>;
		let expand_step = 0;
		while (true) {
			result = pack(item_map, container, item_weighter);
			if (result !== null) {
				break;
			}
			container_expander(container, expand_step);
			expand_step++;
		}
		return [container, result];
	}

	export function pack_automatic_best(
		item_map: Record<string, Box>,
		container_expander: (it: Box, step: number) => void,
		item_weighter: Array<(it: Box) => number> = [
			box_area,
			box_perimeter,
			box_bigger_side,
			box_width,
			box_height,
			box_pathological_multiplier,
		],
	): [Box, Record<string, Rect>] {
		assert_test(item_weighter.length !== 0);
		let candidate = item_weighter.map((value) => (pack_automatic(item_map, container_expander, value)));
		let best = candidate[0];
		for (let e of candidate.slice(1)) {
			if (box_area(e[0]) < box_area(best[0])) {
				best = e;
			}
		}
		return best;
	}

	// #endregion

}
