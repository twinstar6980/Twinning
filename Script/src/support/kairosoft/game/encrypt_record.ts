namespace Twinning.Script.Support.Kairosoft.Game.EncryptRecord {

	// #region utility

	export function process_fs(
		target_directory: StoragePath,
		key: Array<bigint>,
	): void {
		for (let item of StorageHelper.list_directory(target_directory, 1n, true, false, true, false).filter((value) => (/^\d{4,4}(_backup)?$/.test(value.name()!)))) {
			let item_file = target_directory.push(item);
			KernelX.Tool.Data.Encryption.Exor.encrypt_fs(item_file, item_file, key);
		}
		return;
	}

	// #endregion

}
