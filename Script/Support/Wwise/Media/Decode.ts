namespace Twinning.Script.Support.Wwise.Media.Decode {

	// #region utility

	export function decode_fs(
		ripe_file: string,
		raw_file: string,
	): Format {
		if (!KernelX.is_windows && !KernelX.is_linux && !KernelX.is_macintosh) {
			throw new Error(`unsupported system, this function only avaliable for windows or linux or macintosh`);
		}
		let vgmstream_program_path = ProcessHelper.search_program_ensure('vgmstream-cli');
		let ripe_file_fallback_temporary: null | string = null;
		let ripe_file_fallback = ripe_file;
		if (!ripe_file.endsWith('.wem')) {
			ripe_file_fallback_temporary = HomePath.new_temporary(null, 'directory');
			ripe_file_fallback = `${ripe_file_fallback_temporary}/sample.wem`;
			KernelX.Storage.copy(ripe_file, ripe_file_fallback);
		}
		let raw_file_directory = PathUtility.parent(raw_file);
		if (raw_file_directory !== null) {
			KernelX.Storage.create_directory(raw_file_directory);
		}
		let vgmstream_program_result = ProcessHelper.spawn_child(
			vgmstream_program_path,
			[
				'-o',
				raw_file,
				ripe_file_fallback,
			],
			KernelX.Process.list_environment_variable(),
		);
		if (ripe_file_fallback_temporary !== null) {
			KernelX.Storage.remove(ripe_file_fallback_temporary);
		}
		if (vgmstream_program_result.code !== 0n) {
			throw new Error(`execute failed by vgmstream-cli`);
		}
		let regex_result = /^encoding: (.+)$/m.exec(vgmstream_program_result.output);
		assert_test(regex_result !== null);
		let encoding = regex_result[1];
		let format: null | Format = null;
		if (encoding === 'Little Endian 16-bit PCM') {
			format = 'pcm';
		}
		if (encoding === 'Platinum 4-bit ADPCM') {
			format = 'adpcm';
		}
		if (encoding === 'Audiokinetic Wwise 4-bit IMA ADPCM') {
			format = 'adpcm';
		}
		if (encoding === 'Custom Vorbis') {
			format = 'vorbis';
		}
		if (encoding === 'AAC (Advanced Audio Coding)') {
			format = 'aac';
		}
		if (encoding === 'libopus Opus') {
			format = 'wemopus'; // or opus
		}
		if (format === null) {
			throw new Error(`unknown wem format '${encoding}'`);
		}
		return format;
	}

	// #endregion

}
