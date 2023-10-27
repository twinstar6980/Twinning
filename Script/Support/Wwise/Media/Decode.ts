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
		let raw_file_directory = PathUtility.parent(raw_file);
		if (raw_file_directory !== null) {
			KernelX.FileSystem.create_directory(raw_file_directory);
		}
		program_result = ProcessHelper.execute(
			vgmstream_program_file,
			[
				'-o',
				raw_file,
				ripe_file,
			],
			KernelX.Process.list_environment_variable(),
		);
		assert_test(program_result.code === 0n, `execute failed by vgmstream`);
		return;
	}

	// ------------------------------------------------

}
