namespace Twinning.Script.Support.Wwise.Media.Decode {

	// #region utility

	export function decode_fs(
		ripe_file: StoragePath,
		raw_file: StoragePath,
	): Format {
		if (!KernelX.is_windows && !KernelX.is_linux && !KernelX.is_macintosh) {
			throw new Error(`unsupported system, this function only available for windows or linux or macintosh`);
		}
		let ripe_file_fallback_temporary: null | StoragePath = null;
		let ripe_file_fallback = ripe_file;
		if (ripe_file.extension()?.toLowerCase() !== 'wem') {
			ripe_file_fallback_temporary = HomePath.new_temporary(null, 'directory');
			ripe_file_fallback = ripe_file_fallback_temporary.join('sample.wem');
			StorageHelper.copy(ripe_file, ripe_file_fallback, false);
		}
		let raw_file_directory = raw_file.parent();
		if (raw_file_directory !== null && !StorageHelper.exist_directory(raw_file_directory)) {
			StorageHelper.create_directory(raw_file_directory);
		}
		let vgmstream_result = ProcessHelper.run_process(
			ProcessHelper.search_program_ensure('vgmstream-cli', true),
			[
				'-o',
				raw_file.emit(),
				ripe_file_fallback.emit(),
			],
			null,
		);
		if (ripe_file_fallback_temporary !== null) {
			StorageHelper.remove(ripe_file_fallback_temporary);
		}
		if (vgmstream_result.code !== 0n) {
			throw new Error(`execute failed by vgmstream-cli`);
		}
		let regex_result = /^encoding: (.+)$/m.exec(vgmstream_result.output);
		assert_test(regex_result !== null);
		let encoding = regex_result[1];
		let format: null | Format = null;
		if (encoding === '16-bit Little Endian PCM') {
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
