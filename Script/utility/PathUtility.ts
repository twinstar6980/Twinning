namespace Twinning.Script.PathUtility {

	// ------------------------------------------------

	export function regularize(
		target: string,
	): string {
		return to_posix_style(target);
	}

	export function to_posix_style(
		target: string,
	): string {
		return target.replaceAll('\\', '/');
	}

	export function to_windows_style(
		target: string,
	): string {
		return target.replaceAll('/', '\\');
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
		target = regularize(target);
		let index = target.lastIndexOf('/');
		return index === -1 ? null : target.slice(0, index);
	}

	export function name(
		target: string,
	): string {
		target = regularize(target);
		let index = target.lastIndexOf('/');
		return index === -1 ? target : target.slice(index + 1);
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
				let is_name = Number(index) === path.length - 1;
				if (current[element] === undefined) {
					current[element] = is_name ? null : {};
				}
				else {
					assert_test(current[element] !== null && !is_name);
				}
				current = current[element]!;
			}
		}
		return tree;
	}

	// ------------------------------------------------

	const dangerous_path_rule = [
		/^\/((storage\/emulated\/[0-9]+)|(sdcard))\//,
		/^\/$/,
		/^\/(home)$/,
		/^\/(Windows)/,
		/^\/(Windows.old)/,
		/^\/(Documents and Settings)$/,
		/^\/(PerfLogs)$/,
		/^\/(Program Files)$/,
		/^\/(Program Files (x86))$/,
		/^\/(ProgramData)$/,
		/^\/(Recovery)$/,
		/^\/(System Volume Information)$/,
		/^\/(Users)$/,
		/^\/(appverifUI.dll)$/,
		/^\/(hiberfil.sys)$/,
		/^\/(swapfile.sys)$/,
		/^\/(pagefile.sys)$/,
		/^\/(vfcompat.dll)$/,
		/^\/()$/,
		/^\/()$/,
		/^\/()$/,
		/^\/()$/,
	];

	export function is_dangerous(
		target: string,
	): boolean {
		return dangerous_path_rule.every((value) => (!value.test(target)));
	}

	export function rename_secure(
		source: string,
		destination: string,
	): void {
		if (KernelX.is_android && AndroidHelper.fs_is_fuse_path(destination) && source.toLowerCase() === destination.toLowerCase()) {
			KernelX.Storage.rename(source, source + '!');
			KernelX.Storage.rename(source + '!', destination);
		}
		else {
			KernelX.Storage.rename(source, destination);
		}
		return;
	}

	export function generate_suffix_path(
		path: string,
		infix: string = '.',
	): string {
		let result = path;
		let suffix = 0;
		while (KernelX.Storage.exist(result)) {
			suffix += 1;
			result = `${path}${infix}${suffix}`;
		}
		return result;
	}

	// ------------------------------------------------

}