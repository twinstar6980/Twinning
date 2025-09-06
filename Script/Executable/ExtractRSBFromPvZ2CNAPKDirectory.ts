namespace Twinning.Script.Executable.ExtractRSBFromPvZ2CNAPKDirectory {

	// #region run

	export function run(
	): void {
		Console.information(los('executable.extract_rsb_from_pvz2_cn_apk_directory:provide_input_directory'), [
		]);
		let input_directory = Console.path('directory', 'input', null, null);
		Console.information(los('executable.extract_rsb_from_pvz2_cn_apk_directory:provide_output_directory'), [
		]);
		let output_directory = Console.path('directory', 'output', null, null);
		Console.information(los('executable.extract_rsb_from_pvz2_cn_apk_directory:extract_start'), [
			los('executable.extract_rsb_from_pvz2_cn_apk_directory:input_directory', input_directory),
			los('executable.extract_rsb_from_pvz2_cn_apk_directory:output_directory', output_directory),
		]);
		Executor.execute_typical_batch_task(input_directory, ['file', /.+\.rsb\.smf$/i], (item, temporary) => {
			let input_file = `${input_directory}/${item}`;
			let output_file = `${output_directory}/${item.slice(0, -4)}`;
			KernelX.Tool.PopCap.ZLib.uncompress_fs(input_file, output_file, 15n, { variant_64: false });
		});
		Console.success(los('executable.extract_rsb_from_pvz2_cn_apk_directory:extract_finish'), [
		]);
		return;
	}

	// #endregion

}

Twinning.Script.Executable.ExtractRSBFromPvZ2CNAPKDirectory.run();