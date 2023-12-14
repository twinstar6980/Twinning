namespace TwinStar.Script.Support.Wwise.Media.Decode {

	// ------------------------------------------------

	export function decode_fs(
		ripe_file: string,
		raw_file: string,
	): void {
		assert_test(Shell.is_windows || Shell.is_linux || Shell.is_macintosh, `unsupported system, this function only avaliable for windows or linux or macintosh`);
		let vgmstream_program_file = ProcessHelper.search_path('vgmstream-cli');
		assert_test(vgmstream_program_file !== null, `can not found 'vgmstream-cli' program from PATH environment`);
		let program_result: ProcessHelper.ExecuteResult;
		let ripe_file_fallback_temporary: null | string = null;
		let ripe_file_fallback = ripe_file;
		if (!ripe_file.endsWith('.wem')) {
			ripe_file_fallback_temporary = Home.new_temporary(null, 'directory');
			ripe_file_fallback = `${ripe_file_fallback_temporary}/sample.wem`;
			KernelX.FileSystem.copy(ripe_file, ripe_file_fallback);
		}
		let raw_file_directory = PathUtility.parent(raw_file);
		if (raw_file_directory !== null) {
			KernelX.FileSystem.create_directory(raw_file_directory);
		}
		program_result = ProcessHelper.execute(
			vgmstream_program_file,
			[
				'-o',
				raw_file,
				ripe_file_fallback,
			],
			KernelX.Process.list_environment_variable(),
		);
		if (ripe_file_fallback_temporary !== null) {
			KernelX.FileSystem.remove(ripe_file_fallback);
		}
		assert_test(program_result.code === 0n, `execute failed by vgmstream`);
		return;
	}

	// ------------------------------------------------

}
