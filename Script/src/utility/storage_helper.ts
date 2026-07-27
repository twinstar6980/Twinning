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

	export function resize_file(
		target: StoragePath,
		size: bigint,
	): void {
		return KernelX.Storage.resize_file(target, size);
	}

	// ----------------

	export function read_file(
		target: StoragePath,
		offset: bigint,
		data: Kernel.ByteListView,
	): void {
		return KernelX.Storage.read_file(target, offset, data);
	}

	export function write_file(
		target: StoragePath,
		offset: bigint,
		data: Kernel.ByteListView,
	): void {
		return KernelX.Storage.write_file(target, offset, data);
	}

	// ----------------

	export function read_file_data(
		target: StoragePath,
	): Kernel.ByteArray {
		let size = size_file(target);
		let data = Kernel.ByteArray.allocate(Kernel.Size.value(size));
		read_file(target, 0n, data.view());
		return data;
	}

	export function write_file_data(
		target: StoragePath,
		data: Kernel.ByteListView,
	): void {
		// TODO: dont create
		if (!exist_file(target)) {
			create_file(target);
		}
		resize_file(target, data.size().value);
		write_file(target, 0n, data);
		return;
	}

	export function read_file_text(
		target: StoragePath,
	): string {
		let data = read_file_data(target);
		let text = Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
		return text;
	}

	export function write_file_text(
		target: StoragePath,
		text: string,
	): void {
		let data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(text));
		write_file_data(target, data.view());
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
		type: 'user_home' | 'application_shared' | 'application_persistent' | 'application_temporary' | 'application_cache',
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
			Shell.assistant_reveal_storage_item(target);
		}
		return;
	}

	export function pick_storage_item(
		type: 'load_file' | 'load_directory' | 'save_file',
		multiply: boolean,
		location: null | StoragePath,
		name: null | string,
	): Array<StoragePath> {
		let target: Array<StoragePath> = undefined!;
		if (Shell.is_basic) {
			// unavailable, silently fail
			target = [];
		}
		if (Shell.is_assistant) {
			if (location === null || !exist_directory(location)) {
				location = query_storage_item('user_home')!;
			}
			if (name === null) {
				name = '';
			}
			let target_value = Shell.assistant_pick_storage_item(type, multiply, location, name).target;
			target = target_value.map((it) => new StoragePath(it));
		}
		return target;
	}

	// ----------------

	export function temporary(
		use_cache: boolean = false,
	): [StoragePath, Finalizer] {
		let parent = HomePath.temporary();
		if (use_cache && Shell.is_assistant) {
			parent = query_storage_item('application_cache')!;
		}
		let name = ConvertHelper.make_date_to_string(new Date(), 'YY-MM-DD_hh-mm-ss_SSS');
		let target = generate_suffix_path(parent.join(name), null);
		{
			create_directory(target);
		}
		let target_finalizer = new Finalizer(() => {
			remove(target);
		});
		return [target, target_finalizer];
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
		while (exist(result)) {
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

	// #endregion

}
