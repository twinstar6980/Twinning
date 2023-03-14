namespace TwinStar.Script.Support.PvZ2.Image {

	// ------------------------------------------------

	export type SpriteInformation = {
		position: [bigint, bigint];
		size: [bigint, bigint];
	};

	export type AtlasInformation = {
		format: Support.PopCap.Texture.Encode.Format;
		size: [bigint, bigint];
		sprite: Record<string, SpriteInformation>;
	};

	export type Manifest = Record<string, AtlasInformation>;

	// ------------------------------------------------

	export function from_resource_manifest(
		resource_manifest: ResourceStreamBundle.ResourceManifest.Package,
	): Manifest {
		let manifest: Manifest = {};
		for (let group_id in resource_manifest.group) {
			let group = resource_manifest.group[group_id];
			for (let subgroup_id in group.subgroup) {
				let subgroup = group.subgroup[subgroup_id];
				for (let resource_id in subgroup.resource) {
					let resource = subgroup.resource[resource_id];
					if (resource.expand[0] === 'atlas') {
						// manifest[resource.path] = {
						// 	format: 'argb_8888',
						// };
					}
				}
			}
		}
		return manifest;
	}

	// ------------------------------------------------

}