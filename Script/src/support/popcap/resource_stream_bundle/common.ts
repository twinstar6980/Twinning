namespace Twinning.Script.Support.Popcap.ResourceStreamBundle {

	// #region layout mode

	const LayoutModeX = [
		'group',
		'subgroup',
		'resource',
	] as const;

	export type LayoutMode = typeof LayoutModeX[number];

	export const LayoutModeE = LayoutModeX as unknown as LayoutMode[];

	// ----------------

	export function make_package_relative_path(
		layout_mode: LayoutMode,
	): {
		resource_directory: StoragePath;
		packet_file: StoragePath;
		packet_definition_file: StoragePath;
	} {
		let result: ReturnType<typeof make_package_relative_path>;
		switch (layout_mode) {
			case 'group': {
				result = {
					resource_directory: new StoragePath('./group/{0}/{1}/resource'),
					packet_file: new StoragePath('./group/{0}/{1}/packet.rsg'),
					packet_definition_file: new StoragePath('./group/{0}/{1}/definition.json'),
				};
				break;
			}
			case 'subgroup': {
				result = {
					resource_directory: new StoragePath('./subgroup/{1}/resource'),
					packet_file: new StoragePath('./subgroup/{1}/packet.rsg'),
					packet_definition_file: new StoragePath('./subgroup/{1}/definition.json'),
				};
				break;
			}
			case 'resource': {
				result = {
					resource_directory: new StoragePath('./resource'),
					packet_file: new StoragePath('./packet/{1}.rsg'),
					packet_definition_file: new StoragePath('./packet/{1}.json'),
				};
				break;
			}
		}
		return result;
	}

	// #endregion

}
