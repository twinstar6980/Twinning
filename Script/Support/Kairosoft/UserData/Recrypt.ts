namespace TwinStar.Script.Support.Kairosoft.UserData.Recrypt {

	// ------------------------------------------------

	export function process_fs(
		old_directory: string,
		new_directory: string,
		old_key: bigint,
		new_key: bigint,
	): void {
		let key_data = new ArrayBuffer(8);
		new BigUint64Array(key_data)[0] = old_key ^ new_key;
		let key_list = Array.from(new Uint8Array(key_data)).map((e) => (BigInt(e)));
		let item_list = KernelX.FileSystem.list_file(old_directory).filter((value) => (/^\d{4,4}$/.test(value)));
		for (let item of item_list) {
			KernelX.Tool.Data.Encryption.XOR.encrypt_fs(`${old_directory}/${item}`, `${new_directory}/${item}`, key_list);
		}
		return;
	}

	// ------------------------------------------------

}