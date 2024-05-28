namespace Twinning.Script.Support.Atlas.PackAutomatic {

	// ------------------------------------------------

	export type Box = {
		w: number;
		h: number;
	};

	export type Rect = {
		x: number;
		y: number;
	} & Box;

	// ------------------------------------------------

	function box_is_fit(
		item: Box,
		container: Box,
	): boolean {
		return item.w <= container.w && item.h <= container.h;
	}

	function box_is_empty(
		t: Box,
	): boolean {
		return t.w === 0 || t.h === 0;
	}

	function box_compare_asc(
		a: Box,
		b: Box,
	): number {
		let a_area = a.w * a.h;
		let b_area = b.w * b.h;
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

	// ------------------------------------------------

	function box_area(
		t: Box,
	): number {
		return t.w * t.h;
	}

	function box_perimeter(
		t: Box,
	): number {
		return 2 * (t.w + t.h);
	}

	function box_bigger_side(
		t: Box,
	): number {
		return Math.max(t.w, t.h);
	}

	function box_smaller_side(
		t: Box,
	): number {
		return Math.min(t.w, t.h);
	}

	function box_width(
		t: Box,
	): number {
		return t.w;
	}

	function box_height(
		t: Box,
	): number {
		return t.h;
	}

	function box_pathological_multiplier(
		t: Box,
	): number {
		return box_bigger_side(t) / box_smaller_side(t) * box_area(t);
	}

	// ------------------------------------------------

	export function expander_exponent_of_2_generator(
		square: boolean,
	): (it: Box, step: number) => void {
		return (it, step) => {
			if (square) {
				it.w = Math.max(it.w, 1) * 2;
				it.h = Math.max(it.h, 1) * 2;
			}
			else {
				if (step % 2 === 0) {
					it.h = Math.max(it.h, 1) * 2;
				}
				else {
					it.w = Math.max(it.w, 1) * 2;
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
				it.w += expand_value;
				it.h += expand_value;
			}
			else {
				if (step % 2 === 0) {
					it.w += expand_value;
				}
				else {
					it.h += expand_value;
				}
			}
		}
	}

	// ------------------------------------------------

	export function pack(
		container: Box,
		item_map: Record<string, Box>,
		item_weighter: (a: Box) => number,
	): null | Record<string, Rect> {
		let space_list: Array<Rect> = [{ x: 0, y: 0, ...container }];
		let item_list = record_to_array(item_map, (key, value) => ({ name: key, box: item_map[key] })).sort((a, b) => (item_weighter(a.box) > item_weighter(b.box) ? -1 : +1));
		let result: Record<string, Rect> = {};
		for (let item of item_list) {
			let box = item.box;
			let space_index = space_list.findIndex((value) => (box_is_fit(box, value)));
			if (space_index === -1) {
				return null;
			}
			let space = space_list[space_index];
			space_list.splice(space_index, 1);
			result[item.name] = { x: space.x, y: space.y, ...box };
			let reserve_w = space.w - box.w;
			let reserve_h = space.h - box.h;
			let new_space = [
				{
					x: space.x + box.w,
					y: space.y,
					w: reserve_w,
					h: reserve_w > reserve_h ? space.h : box.h,
				},
				{
					x: space.x,
					y: space.y + box.h,
					w: reserve_w > reserve_h ? box.w : space.w,
					h: reserve_h,
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

	export function pack_automatic(
		item_map: Record<string, Box>,
		container_expander: (it: Box, step: number) => void,
		item_weighter: (a: Box) => number,
	): [Box, Record<string, Rect>] {
		let container: Box = { w: 0, h: 0 };
		let result: null | Record<string, Rect>;
		let expand_step = 0;
		while (true) {
			result = pack(container, item_map, item_weighter);
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
		item_weighter: Array<(a: Box) => number> = [
			box_area,
			box_perimeter,
			box_bigger_side,
			box_width,
			box_height,
			box_pathological_multiplier,
		],
	): [Box, Record<string, Rect>] {
		assert_test(item_weighter.length !== 0, `item_weighter list must be not empty`);
		let candidate = item_weighter.map((value) => (pack_automatic(item_map, container_expander, value)));
		let best = candidate[0];
		for (let e of candidate.slice(1)) {
			if (box_area(e[0]) < box_area(best[0])) {
				best = e;
			}
		}
		return best;
	}

	// ------------------------------------------------

}