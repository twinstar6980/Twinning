namespace TwinStar.Script.Support.PvZ2.ResourceManifest {

	// ------------------------------------------------

	export type ResourceType = RegularResourceManifest.ResourceType;

	export type GenericResourceAdditional = {
	};

	export type AtlasImageResourceAdditional = {
		atlas?: boolean;
		width: bigint;
		height: bigint;
	};

	export type SpriteImageResourceAdditional = {
		parent: string;
		ax: bigint;
		ay: bigint;
		aw: bigint;
		ah: bigint;
		x?: bigint;
		y?: bigint;
		cols?: bigint;
		rows?: bigint;
		aflag?: bigint;
	};

	export type SoundResourceAdditional = {
		volume?: number,
		pan?: number,
	};

	export type PopAnimResourceAdditional = {
		scale?: undefined,
		rasterizedScale?: undefined,
	};

	export type RenderEffectResourceAdditional = {
		srcpath?: Array<string>,
	};

	export type ResourceAdditional = GenericResourceAdditional | AtlasImageResourceAdditional | SpriteImageResourceAdditional | SoundResourceAdditional | PopAnimResourceAdditional | RenderEffectResourceAdditional;

	export type ResourceBase = {
		slot: bigint;
		id: string;
		path: Array<string> | string;
		type: ResourceType;
	};

	export type Resource = ResourceBase & ResourceAdditional;

	// ------------------------------------------------

	export const GroupTypeE = [
		'composite',
		'simple',
	] as const;

	export type GroupType = typeof GroupTypeE[number];

	export type SimpleGroupResolution = RegularResourceManifest.SubgroupResolution;

	export type SimpleGroupLocale = RegularResourceManifest.SubgroupLocale;

	export type SimpleGroupAdditional = {
		res?: SimpleGroupResolution;
		loc?: SimpleGroupLocale;
		parent?: string;
		resources: Array<Resource>;
	};

	export type CompositeGroupAdditional = {
		subgroups: Array<{
			id: string;
			res?: SimpleGroupResolution;
			loc?: SimpleGroupLocale;
		}>;
	};

	export type GroupAdditional = CompositeGroupAdditional | SimpleGroupAdditional;

	export type GroupBase = {
		id: string;
		type: GroupType;
	};

	export type Group = GroupBase & GroupAdditional;

	// ------------------------------------------------

	export type Package = {
		version?: bigint;
		full_resources_digest?: string;
		compatible_resources_digest?: string;
		content_version?: bigint;
		temppath?: string;
		slot_count: bigint;
		groups: Array<Group>;
	};

	// ------------------------------------------------

}