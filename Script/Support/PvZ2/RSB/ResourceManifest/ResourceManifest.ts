namespace TwinStar.Script.Support.PvZ2.RSB.ResourceManifest {

	// ------------------------------------------------

	export const TextureFormatE = [
		'rgba_8888',
		'argb_8888',
		'rgb_etc1_a_8',
		'rgba_pvrtc4',
		'rgb_pvrtc4_a_8',
	] as const;

	export type TextureFormat = typeof TextureFormatE[number];

	// ------------------------------------------------

	export type GenericResourceInformation = {
	};

	export type SpriteImageResource = {
		path: string;
		position: [bigint, bigint];
		size: [bigint, bigint];
		offset: [bigint, bigint];
	};

	export type AtlasImageResourceInformation = {
		size: [bigint, bigint];
		//format: TextureFormat;
		sprite: Record<string, SpriteImageResource>;
	};

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

	export type ResourceBase = {
		path: string;
		type: ResourceType;
	};

	export type Resource = ResourceBase & {
		expand: ['generic', GenericResourceInformation] | ['atlas', AtlasImageResourceInformation];
	};

	// ------------------------------------------------

	export type SubgroupResolution = bigint;

	export type SubgroupLocale = string;

	export type SubgroupCategory = [null | SubgroupResolution, null | SubgroupLocale];

	export type Subgroup = {
		category: SubgroupCategory;
		resource: Record<string, Resource>;
		//resource_data_sectionn_store_mode: Core.Tool.PopCap.RSB.Information.JS_N.ResourceDataSectionStoreMode;
	};

	// ------------------------------------------------

	export type Group = {
		composite: boolean;
		subgroup: Record<string, Subgroup>;
	};

	// ------------------------------------------------

	export type Package = {
		//name: null | string;
		//expand_texture_information: boolean;
		group: Record<string, Group>;
	};

	// ------------------------------------------------

}