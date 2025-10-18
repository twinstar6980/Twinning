namespace Twinning.Script.Support.Kairosoft.Game.EncryptRecord {

	// #region utility

	export function process_fs(
		target_directory: string,
		key: Array<bigint>,
	): void {
		for (let item_name of KernelX.Storage.list_directory(target_directory, 1n, true, false).filter((value) => (/^\d{4,4}(_backup)?$/.test(value)))) {
			let item_file = `${target_directory}/${item_name}`;
			KernelX.Tool.Data.Encryption.EXOR.encrypt_fs(item_file, item_file, key);
		}
		return;
	}

	// #endregion

}