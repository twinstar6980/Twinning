namespace Twinning.Script.HomePath {

	// #region utility

	let g_location: string = undefined!;

	export function of(
		format: string,
	): StoragePath {
		return new StoragePath(format.replaceAll(/^~(?=([/]|$))/g, g_location));
	}

	// ----------------

	export function script(
	): StoragePath {
		return of(`~/script`);
	}

	export function workspace(
	): StoragePath {
		return of(`~/workspace`);
	}

	export function temporary(
	): StoragePath {
		return of(`~/temporary`);
	}

	// ----------------

	export function initialize(
		location: string,
	): void {
		g_location = location;
		if (!StorageHelper.exist_directory(workspace())) {
			StorageHelper.create_directory(workspace());
		}
		if (!StorageHelper.exist_directory(temporary())) {
			StorageHelper.create_directory(temporary());
		}
		KernelX.Process.set_workspace(workspace());
		return;
	}

	// #endregion

}
