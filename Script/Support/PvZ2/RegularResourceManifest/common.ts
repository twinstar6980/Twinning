namespace Twinning.Script.Support.PvZ2.RegularResourceManifest {

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

	export type DummyResourceAdditional = {
	};

	export type GeneralResourceAdditional = {
		path: string;
		type: ResourceType;
	};

	export type TextureResourceAdditionalSprite = {
		id: string;
		path: string;
		position: [bigint, bigint];
		size: [bigint, bigint];
		offset: [bigint, bigint];
		separate: [bigint, bigint];
	};

	export type TextureResourceAdditional = {
		path: string;
		size: [bigint, bigint];
		sprite: Array<TextureResourceAdditionalSprite>;
	};

	export type ResourceAdditional = {
		type: 'dummy';
		value: DummyResourceAdditional;
	} | {
		type: 'general';
		value: GeneralResourceAdditional;
	} | {
		type: 'texture';
		value: TextureResourceAdditional;
	};

	export type ResourceBase = {
		id: string;
	};

	export type Resource = ResourceBase & {
		additional: ResourceAdditional;
	};

	// ------------------------------------------------

	export type SubgroupCategory = {
		resolution: null | bigint;
		locale: null | string;
	};

	export type Subgroup = {
		id: string;
		category: SubgroupCategory;
		resource: Array<Resource>;
	};

	// ------------------------------------------------

	export type Group = {
		id: string;
		composite: boolean;
		subgroup: Array<Subgroup>;
	};

	// ------------------------------------------------

	export type Package = {
		group: Array<Group>;
	};

	// ------------------------------------------------

}