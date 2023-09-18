namespace TwinStar.Script.Support.PvZ2.ResourceManifest {

	// ------------------------------------------------

	export type ResourceType = RegularResourceManifest.ResourceType;

	export type GenericResourceInformation = {
	};

	export type AtlasImageResourceInformation = {
		atlas?: boolean;
		width: bigint;
		height: bigint;
	};

	export type SpriteImageResourceInformation = {
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

	export type SoundResourceInformation = {
		volume?: number,
		pan?: number,
	};

	export type PopAnimResourceInformation = {
		scale?: undefined,
		rasterizedScale?: undefined,
	};

	export type RenderEffectResourceInformation = {
		srcpath?: Array<string>,
	};

	export type ResourceInformation = GenericResourceInformation | AtlasImageResourceInformation | SpriteImageResourceInformation | SoundResourceInformation | PopAnimResourceInformation | RenderEffectResourceInformation;

	export type ResourceBase = {
		slot: bigint;
		id: string;
		path: Array<string> | string;
		type: ResourceType;
	};

	export type Resource = ResourceBase & ResourceInformation;

	// ------------------------------------------------

	export const GroupTypeE = [
		'composite',
		'simple',
	] as const;

	export type GroupType = typeof GroupTypeE[number];

	export type SimpleGroupResolution = RegularResourceManifest.SubgroupResolution;

	export type SimpleGroupLocale = RegularResourceManifest.SubgroupLocale;

	export type SimpleGroupInformation = {
		res?: SimpleGroupResolution;
		loc?: SimpleGroupLocale;
		parent?: string;
		resources: Array<Resource>;
	};

	export type CompositeGroupInformation = {
		subgroups: Array<{
			id: string;
			res?: SimpleGroupResolution;
			loc?: SimpleGroupLocale;
		}>;
	};

	export type GroupInformation = CompositeGroupInformation | SimpleGroupInformation;

	export type GroupBase = {
		id: string;
		type: GroupType;
	};

	export type Group = GroupBase & GroupInformation;

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