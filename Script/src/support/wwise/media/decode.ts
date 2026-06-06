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
			ripe_file_fallback_temporary = StorageHelper.temporary('directory');
			ripe_file_fallback = ripe_file_fallback_temporary.join('sample.wem');
			StorageHelper.copy(ripe_file, ripe_file_fallback, false);
		}
		let raw_file_directory = raw_file.parent();
		if (raw_file_directory !== null && !StorageHelper.exist_directory(raw_file_directory)) {
			StorageHelper.create_directory(raw_file_directory);
		}
		let encoding_name = ExternalHelper.run_vgmstream_decode(raw_file, ripe_file_fallback);
		if (ripe_file_fallback_temporary !== null) {
			StorageHelper.remove(ripe_file_fallback_temporary);
		}
		let format: null | Format = null;
		if (encoding_name === '16-bit Little Endian PCM') {
			format = 'pcm';
		}
		if (encoding_name === 'Platinum 4-bit ADPCM') {
			format = 'adpcm';
		}
		if (encoding_name === 'Audiokinetic Wwise 4-bit IMA ADPCM') {
			format = 'adpcm';
		}
		if (encoding_name === 'Custom Vorbis') {
			format = 'vorbis';
		}
		if (encoding_name === 'AAC (Advanced Audio Coding)') {
			format = 'aac';
		}
		if (encoding_name === 'libopus Opus') {
			format = 'wemopus'; // or opus
		}
		if (format === null) {
			throw new Error(`unknown wem format '${encoding_name}'`);
		}
		return format;
	}

	// #endregion

}
