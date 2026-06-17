namespace Twinning.Script.Support.Wwise.Media.Decode {

	// #region utility

	export function decode_fs(
		ripe_file: StoragePath,
		raw_file: StoragePath,
	): Format {
		let vgmstream_encoding = ExternalHelper.run_vgmstream_decode(raw_file, ripe_file, 'wem');
		let format: null | Format = null;
		if (vgmstream_encoding === '16-bit Little Endian PCM') {
			format = 'pcm';
		}
		if (vgmstream_encoding === 'Platinum 4-bit ADPCM') {
			format = 'adpcm';
		}
		if (vgmstream_encoding === 'Audiokinetic Wwise 4-bit IMA ADPCM') {
			format = 'adpcm';
		}
		if (vgmstream_encoding === 'Custom Vorbis') {
			format = 'vorbis';
		}
		if (vgmstream_encoding === 'AAC (Advanced Audio Coding)') {
			format = 'aac';
		}
		if (vgmstream_encoding === 'libopus Opus') {
			format = 'wemopus'; // or opus
		}
		if (format === null) {
			throw new Error(`unknown wem format '${vgmstream_encoding}'`);
		}
		return format;
	}

	// #endregion

}
