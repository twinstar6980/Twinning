namespace Twinning.Script.StorageHelper {

	// #region basic

	export function exist(
		target: StoragePath,
	): boolean {
		return KernelX.Storage.exist(target);
	}

	export function copy(
		target: StoragePath,
		placement: StoragePath,
		follow_link: boolean,
	): void {
		return KernelX.Storage.copy(target, placement, follow_link);
	}

	export function rename(
		target: StoragePath,
		placement: StoragePath,
	): void {
		return KernelX.Storage.rename(target, placement);
	}

	export function remove(
		target: StoragePath,
	): void {
		return KernelX.Storage.remove(target);
	}

	// ----------------

	export function rename_secure(
		source: StoragePath,
		destination: StoragePath,
	): void {
		// TODO: if dest exist
		if (KernelX.is_android && AndroidHelper.fs_is_fuse_path(destination) && source.emit().toLowerCase() === destination.emit().toLowerCase()) {
			let temporary = source.parent()!.join(source.name()! + '!');
			KernelX.Storage.rename(source, temporary);
			KernelX.Storage.rename(temporary, destination);
		}
		else {
			KernelX.Storage.rename(source, destination);
		}
		return;
	}

	export function remove_if(
		target: StoragePath,
	): void {
		if (exist(target)) {
			remove(target);
		}
		return;
	}

	// #endregion

	// #region link

	export function exist_link(
		target: StoragePath,
	): boolean {
		return KernelX.Storage.exist_link(target);
	}

	export function create_link(
		target: StoragePath,
		referent: StoragePath,
		is_directory: boolean,
	): void {
		return KernelX.Storage.create_link(target, referent, is_directory);
	}

	// ----------------

	export function resolve_link(
		target: StoragePath,
	): string {
		return KernelX.Storage.resolve_link(target);
	}

	// #endregion

	// #region file

	export function exist_file(
		target: StoragePath,
	): boolean {
		return KernelX.Storage.exist_file(target);
	}

	export function create_file(
		target: StoragePath,
	): void {
		return KernelX.Storage.create_file(target);
	}

	// ----------------

	export function size_file(
		target: StoragePath,
	): bigint {
		return KernelX.Storage.size_file(target);
	}

	// ----------------

	export function read_file(
		target: StoragePath,
	): Kernel.ByteArray {
		return KernelX.Storage.read_file(target);
	}

	// TODO: dont create
	export function write_file(
		target: StoragePath,
		data: Kernel.ByteListView | Kernel.ByteArray | ArrayBuffer,
	): void {
		let data_view: Kernel.ByteListView;
		if (data instanceof Kernel.ByteListView) {
			data_view = data;
		}
		if (data instanceof Kernel.ByteArray) {
			data_view = data.view();
		}
		if (data instanceof ArrayBuffer) {
			data_view = Kernel.ByteListView.value(data);
		}
		let target_path = Kernel.Path.value(target.emit());
		if (!Kernel.Storage.exist_file(target_path).value) {
			Kernel.Storage.create_file(target_path);
		}
		return KernelX.Storage.write_file(target, data_view!);
	}

	// ----------------

	export function read_file_text(
		target: StoragePath,
	): string {
		let data = read_file(target);
		let text = Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
		return text;
	}

	export function write_file_text(
		target: StoragePath,
		text: string,
	): void {
		let data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(text));
		write_file(target, data);
		return;
	}

	// #endregion

	// #region directory

	export function exist_directory(
		target: StoragePath,
	): boolean {
		return KernelX.Storage.exist_directory(target);
	}

	export function create_directory(
		target: StoragePath,
	): void {
		return KernelX.Storage.create_directory(target);
	}

	// ----------------

	export function list_directory(
		target: StoragePath,
		depth: null | bigint,
		follow_link: boolean,
		allow_link: boolean,
		allow_file: boolean,
		allow_directory: boolean,
	): Array<StoragePath> {
		return KernelX.Storage.list_directory(target, depth, follow_link, allow_link, allow_file, allow_directory);
	}
	
	// #endregion

	// #region miscellaneous

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

	// ----------------

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

	// ----------------

	export type Tree = {[key: string]: null | Tree};

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

	// ----------------

	export function generate_suffix_path(
		path: StoragePath,
		infix: null | string,
	): StoragePath {
		infix = CheckHelper.not_null_or(infix, '.');
		let result = path;
		let suffix = 0;
		while (KernelX.Storage.exist(result)) {
			suffix += 1;
			result = path.parent()!.join(path.name()! + `${infix}${suffix}`);
		}
		return result;
	}

	// ----------------

	const k_dangerous_path_rule = [
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
	];

	export function is_dangerous(
		target: string,
	): boolean {
		return k_dangerous_path_rule.every((value) => (!value.test(target)));
	}

	// #endregion

}
