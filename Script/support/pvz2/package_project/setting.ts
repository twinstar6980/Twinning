namespace Twinning.Script.Support.Pvz2.PackageProject {

	// #region type

	export type Variable = {
		name: string;
		value: string;
	};

	export type ConversionSetting = {
		rton: Array<{
			name: string;
			version: typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value;
			key: null | string;
		}>;
		ptx: Array<{
			name: string;
			format: Support.Popcap.Texture.Encoding.Format;
			index: bigint;
		}>;
		pam: Array<{
			name: string;
			version: typeof Kernel.Tool.Popcap.Animation.Version.Value;
		}>;
		wem: Array<{
			name: string;
			format: Support.Wwise.Media.Format;
		}>;
	};

	// ----------------

	export type ResourceCategory = {
		resolution: null | bigint;
		locale: null | string;
	};

	const ResourceTypeX = [
		'dummy',
		'general',
		'texture',
		'special_rton',
		'special_ptx',
		'special_pam',
		'special_wem',
	] as const;

	export type ResourceType = typeof ResourceTypeX[number];

	export const ResourceTypeE = ResourceTypeX as unknown as ResourceType[];

	export type DummyResourceProperty = {
	};

	export type GeneralResourceProperty = {
		path: string;
		type: RegularResourceManifest.ResourceType;
	};

	export type TextureResourcePropertySpriteProperty = {
		identifier: string;
		path: string;
		position: [bigint, bigint];
		size: [bigint, bigint];
		offset: [bigint, bigint];
		separate: [bigint, bigint];
	};

	export type TextureResourceProperty = {
		path: string;
		format: bigint;
		pitch: bigint;
		additional_byte_count: bigint;
		size: [bigint, bigint];
		sprite: Array<TextureResourcePropertySpriteProperty>;
	};

	export type SpecialResourcePropertyBase = {
		conversion: string;
	};

	export type SpecialRtonResourceProperty = SpecialResourcePropertyBase & {
		path: string;
	};

	export type SpecialPtxResourcePropertySpriteProperty = {
		source: string;
		identifier: string;
		path: string;
		offset: [bigint, bigint];
		separate: [bigint, bigint];
	};

	export type SpecialPtxResourceProperty = SpecialResourcePropertyBase & {
		path: string;
		resolution: bigint;
		sprite: Array<SpecialPtxResourcePropertySpriteProperty>;
	};

	export type SpecialPamResourceProperty = SpecialResourcePropertyBase & {
		path: string;
	};

	export type SpecialPopfxPesourceProperty = SpecialResourcePropertyBase & {
		path: string;
	};

	export type SpecialWemResourceProperty = SpecialResourcePropertyBase & {
		path: string;
	};

	export type SpecialBnkResourceProperty = SpecialResourcePropertyBase & {
		path: string;
	};

	export type ResourceProperty = DummyResourceProperty | GeneralResourceProperty | TextureResourceProperty | SpecialRtonResourceProperty | SpecialPtxResourceProperty | SpecialPamResourceProperty | SpecialPopfxPesourceProperty | SpecialWemResourceProperty | SpecialBnkResourceProperty;

	// ----------------

	const ManifestTypeX = [
		'internal',
		'external_rton_with_array_path',
		'external_rton_with_string_path',
		'external_newton',
	] as const;

	export type ManifestType = typeof ManifestTypeX[number];

	export const ManifestTypeE = ManifestTypeX as unknown as ManifestType[];

	// ----------------

	export type ResourceSetting = {
		category: ResourceCategory;
		type: ResourceType;
		property: ResourceProperty;
		variable: Array<Variable>;
	};

	export type GroupSetting = {
		variable: Array<Variable>;
	};

	export type PartSetting = {
		variable: Array<Variable>;
	};

	export type PackageSetting = {
		name: string;
		part: Array<string>;
		version: {
			number: 1n | 3n | 4n;
			extended_texture_information_for_pvz2cn: 0n | 1n | 2n | 3n;
		};
		compression: {
			general: boolean;
			texture: boolean;
			filter: Array<string>;
		};
		manifest: {
			type: ManifestType;
			suffix: string;
		};
		category: {
			resolution: Array<bigint>;
			locale: Array<string>;
		};
		conversion: ConversionSetting;
		variable: Array<Variable>;
	};

	export type ProjectSetting = {
		package: Array<PackageSetting>;
	};

	// #endregion

}
