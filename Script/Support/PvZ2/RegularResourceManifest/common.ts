namespace TwinStar.Script.Support.PvZ2.RegularResourceManifest {

	// ------------------------------------------------

	export const TextureFormatE = [
		'abgr_8888',
		'argb_8888',
		'rgb_etc1_a_8',
		'rgba_pvrtc4',
		'rgb_pvrtc4_a_8',
	] as const;

	export type TextureFormat = typeof TextureFormatE[number];

	// ------------------------------------------------

	export const ResourceTypeE = [
		'File',
		'Image',
		'PopAnim',
		'SoundBank',
		'DecodedSoundBank',
		'RenderEffect',
		'PrimeFont',
	] as const;

	export type ResourceType = typeof ResourceTypeE[number];

	export type GenericResourceAdditional = {
	};

	export type AtlasSpriteResource = {
		path: string;
		position: [bigint, bigint];
		size: [bigint, bigint];
		offset: [bigint, bigint];
		layout: [bigint, bigint];
	};

	export type AtlasResourceAdditional = {
		size: [bigint, bigint];
		sprite: Record<string, AtlasSpriteResource>;
	};

	export type ResourceAdditional = {
		type: 'generic';
		value: GenericResourceAdditional;
	} | {
		type: 'atlas';
		value: AtlasResourceAdditional;
	};

	export type ResourceBase = {
		path: string;
		type: ResourceType;
	};

	export type Resource = ResourceBase & {
		additional: ResourceAdditional;
	};

	// ------------------------------------------------

	export type SubgroupResolution = bigint;

	export type SubgroupLocale = string;

	export type SubgroupCategory = [null | SubgroupResolution, null | SubgroupLocale];

	export type Subgroup = {
		category: SubgroupCategory;
		resource: Record<string, Resource>;
	};

	// ------------------------------------------------

	export type Group = {
		composite: boolean;
		subgroup: Record<string, Subgroup>;
	};

	// ------------------------------------------------

	export type Package = {
		group: Record<string, Group>;
	};

	// ------------------------------------------------

}