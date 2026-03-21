namespace Twinning.Script.HomePath {

	// #region utility

	let g_location: string = undefined!;

	export function of(
		format: string,
	): string {
		return format.replaceAll(/^~(?=([/]|$))/g, g_location);
	}

	// ----------------

	export function script(
	): string {
		return of(`~/script`);
	}

	export function workspace(
	): string {
		return of(`~/workspace`);
	}

	export function temporary(
	): string {
		return of(`~/temporary`);
	}

	// ----------------

	export function new_temporary(
		name: null | string,
		create: null | 'file' | 'directory',
	): string {
		let temporary_name = name !== null ? name : ConvertHelper.make_date_to_string_simple(new Date());
		let temporary_path = StorageHelper.generate_suffix_path(`${temporary()}/${temporary_name}`, null);
		if (create === 'file') {
			KernelX.Storage.create_file(temporary_path);
		}
		if (create === 'directory') {
			KernelX.Storage.create_directory(temporary_path);
		}
		return temporary_path;
	}

	// ----------------

	export function initialize(
		location: string,
	): void {
		g_location = location;
		KernelX.Storage.create_directory(workspace());
		KernelX.Storage.create_directory(temporary());
		KernelX.Process.set_workspace(workspace());
		return;
	}

	// #endregion

}
