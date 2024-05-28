namespace Twinning.Script.Support.PvZ2.ResourceManifest.NewTypeObjectNotation {

	// ------------------------------------------------

	export const GroupTypeEnumeration = [
		{ index: 1n, value: 'composite' },
		{ index: 2n, value: 'simple' },
	] as const;

	export const ResourceTypeEnumeration = [
		{ index: 1n, value: 'Image' },
		{ index: 2n, value: 'PopAnim' },
		{ index: 3n, value: 'SoundBank' },
		{ index: 4n, value: 'File' },
		{ index: 5n, value: 'PrimeFont' },
		{ index: 6n, value: 'RenderEffect' },
		{ index: 7n, value: 'DecodedSoundBank' },
	] as const;

	// ------------------------------------------------

}