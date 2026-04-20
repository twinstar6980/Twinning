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

	export function rename_case(
		source: StoragePath,
		destination: StoragePath,
	): void {
		// TODO: if dest exist
		let temporary = source.parent()!.join(source.name()! + '!');
		KernelX.Storage.rename(source, temporary);
		KernelX.Storage.rename(temporary, destination);
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

	// #region shell

	export function query_storage_item(
		type: 'user_home' | 'application_shared' | 'application_cache',
	): null | StoragePath {
		let target: null | StoragePath = undefined!;
		if (Shell.is_basic) {
			// unavailable, silently fail
			target = null;
		}
		if (Shell.is_assistant) {
			let target_value = Shell.assistant_query_storage_item(type).target;
			target = new StoragePath(target_value);
		}
		return target;
	}

	export function reveal_storage_item(
		target: StoragePath,
	): void {
		if (Shell.is_basic) {
			// unavailable, silently fail
		}
		if (Shell.is_assistant) {
			Shell.assistant_reveal_storage_item(target.emit());
		}
		return;
	}

	export function pick_storage_item(
		type: 'load_file' | 'load_directory' | 'save_file',
		location: null | StoragePath,
		name: null | string,
	): null | StoragePath {
		let target: null | StoragePath = undefined!;
		if (Shell.is_basic) {
			// unavailable, silently fail
			target = null;
		}
		if (Shell.is_assistant) {
			let target_value = Shell.assistant_pick_storage_item(type, location === null ? '' : location.emit(), name === null ? '' : name).target;
			if (target_value !== '') {
				target = new StoragePath(target_value);
			}
		}
		return target;
	}

	// ----------------

	export function temporary(
		create: null | 'file' | 'directory',
	): StoragePath {
		let temporary_parent = HomePath.temporary();
		if (Shell.is_assistant) {
			temporary_parent = StorageHelper.query_storage_item('application_cache')!;
		}
		let temporary_name = ConvertHelper.make_date_to_string_simple(new Date());
		let temporary_path = StorageHelper.generate_suffix_path(temporary_parent.join(temporary_name), null);
		if (create === 'file') {
			StorageHelper.create_file(temporary_path);
		}
		if (create === 'directory') {
			StorageHelper.create_directory(temporary_path);
		}
		return temporary_path;
	}

	// #endregion

	// #region miscellaneous

	export function generate_suffix_path(
		path: StoragePath,
		infix: null | string,
	): StoragePath {
		infix = CheckHelper.not_null_or(infix, '.');
		let result = path;
		let suffix = 0;
		while (StorageHelper.exist(result)) {
			suffix += 1;
			result = path.parent()!.join(path.name()! + `${infix}${suffix}`);
		}
		return result;
	}

	// ----------------

	export type PathSegmentTree = {[key: string]: null | PathSegmentTree};

	export function resolve_segment_tree(
		target: Array<StoragePath>,
	): PathSegmentTree {
		let tree: PathSegmentTree = {};
		let list = target.map((it) => it.segment());
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

	// TODO: safty
	export function is_dangerous(
		target: string,
	): boolean {
		return k_dangerous_path_rule.every((value) => (!value.test(target)));
	}

	// #endregion

}
