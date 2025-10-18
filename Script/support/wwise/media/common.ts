namespace Twinning.Script.Support.Wwise.Media {

	// #region type

	const FormatX = [
		'pcm',
		'adpcm',
		'vorbis',
		'aac',
		'opus',
		'wemopus',
	] as const;

	export type Format = typeof FormatX[number];

	export const FormatE = FormatX as unknown as Format[];

	// #endregion

}
