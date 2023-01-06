/** PvZ2数据包中原生的资源清单 */
namespace TwinStar.Support.PvZ2.RSB.OfficialResourceManifest {

	// ------------------------------------------------

	export type GenericResourceInformation = {
	};

	export type BaseImageResourceInformation = {
		x?: bigint;
		y?: bigint;
		rows?: bigint;
		cols?: bigint;
		// ???
		anim?: 'none' | 'once' | 'loop' | 'pingpong';
		framedelay?: bigint;
		begindelay?: bigint;
		enddelay?: bigint;
		perframedelay?: Array<bigint>;
		framemap?: Array<bigint>;
	};

	export type AtlasImageResourceInformation = BaseImageResourceInformation & {
		atlas?: boolean;
		width: bigint;
		height: bigint;
	};

	export type SpriteImageResourceInformation = BaseImageResourceInformation & {
		parent: string;
		ax: bigint;
		ay: bigint;
		aw: bigint;
		ah: bigint;
		aflag?: bigint;
	};

	export type SoundResourceInformation = {
		volume?: number,
		pan?: number,
	};

	export type RenderEffectResourceInformation = {
		srcpath?: Array<string>,
	};

	export type ResourceType = ResourceManifest.ResourceType;

	export type ResourceBase = {
		slot: bigint;
		id: string;
		path: Array<string>;
		type: ResourceType;
	};

	export type Resource = ResourceBase & (GenericResourceInformation | AtlasImageResourceInformation | SpriteImageResourceInformation);

	// ------------------------------------------------

	export type SimpleGroupResolution = ResourceManifest.SubgroupResolution;

	export type SimpleGroupLocale = ResourceManifest.SubgroupLocale;

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

	export type GroupInformation = (
		({ type: 'composite'; } & CompositeGroupInformation) |
		({ type: 'simple'; } & SimpleGroupInformation)
	);

	export const GroupTypeE = [
		'composite',
		'simple',
	] as const;

	export type GroupType = typeof GroupTypeE[number];

	export type GroupBase = {
		id: string;
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