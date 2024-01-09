namespace TwinStar.Script.Support.Kairosoft.Game.EncryptRecord {

	// ------------------------------------------------

	export function process_fs(
		target_directory: string,
		key: bigint,
	): void {
		let key_data = new ArrayBuffer(8);
		new BigUint64Array(key_data)[0] = key;
		let key_list = Array.from(new Uint8Array(key_data)).map(BigInt);
		for (let item_name of KernelX.FileSystem.list_file(target_directory, 1n).filter((value) => (/^\d{4,4}(_backup)?$/.test(value)))) {
			let item_file = `${target_directory}/${item_name}`;
			KernelX.Tool.Data.Encryption.XOR.encrypt_fs(item_file, item_file, key_list);
		}
		return;
	}

	// ------------------------------------------------

}