namespace TwinStar.Script.PathUtility {

	// ------------------------------------------------

	export function regularize(
		target: string,
	): string {
		return target.replaceAll('\\', '/');
	}

	export function to_windows_style(
		target: string,
	): string {
		return target.replaceAll('\\', '/');
	}

	// ------------------------------------------------

	export function split(
		target: string,
	): Array<string> {
		return regularize(target).split('/');
	}

	export function catenate(
		target: Array<string>,
	): string {
		return regularize(target.join('/'));
	}

	// ------------------------------------------------

	export function parent(
		target: string,
	): null | string {
		let regular_target = regularize(target);
		let index = regular_target.lastIndexOf('/');
		return (index === -1) ? (null) : (regular_target.slice(0, index));
	}

	export function name(
		target: string,
	): string {
		let regular_target = regularize(target);
		let index = regular_target.lastIndexOf('/');
		return (index === -1) ? (regular_target) : (regular_target.slice(index + 1));
	}

	export function extension(
		target: string,
	): null | string {
		let regular_target = regularize(target);
		let index = regular_target.lastIndexOf('.');
		return (index === -1) ? (null) : (regular_target.slice(index + 1));
	}

	// ------------------------------------------------

	export type Tree = { [key: string]: null | Tree; };

	export function tree(
		target: Array<string>,
	): Tree {
		let tree: Tree = {};
		let list = target.map(split);
		for (let path of list) {
			let current = tree;
			for (let index in path) {
				let element = path[index];
				let is_name = parseInt(index) === path.length - 1;
				if (current[element] === undefined) {
					current[element] = is_name ? null : {};
				} else {
					assert_test(current[element] !== null && !is_name);
				}
				current = current[element]!;
			}
		}
		return tree;
	}

	// ------------------------------------------------
	// TODO

	export function is_android_fuse_object(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\//.test(target);
	}

	export function is_android_fuse_ext_data_object(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/Android\/data\//.test(target);
	}

	export function is_android_fuse_ext_obb_object(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/Android\/obb\//.test(target);
	}

	export function safe_rename(
		source: string,
		destination: string,
	): void {
		if (Shell.is_android && is_android_fuse_object(destination) && source.toLowerCase() === destination.toLowerCase()) {
			CoreX.FileSystem.rename(source, source + '!');
			CoreX.FileSystem.rename(source + '!', destination);
		} else {
			CoreX.FileSystem.rename(source, destination);
		}
		return;
	}

	// ------------------------------------------------

}