namespace Twinning.Script.Support.PvZ2.PackageProject.Parse {

	// ------------------------------------------------

	export function parse(
		project_directory: string,
		package_directory: string,
		package_name: string,
		package_version: typeof Kernel.Tool.PopCap.ResourceStreamBundle.Version.Value,
		conversion_setting: ConversionSetting,
	): void {
		check_version_file(project_directory);
		let find_item_ignore_case = <T>(list: Array<T>, key: string, value: string): null | T => {
			let value_lower = value.toLowerCase();
			for (let item of list) {
				let item_value = (item as any)[key] as string;
				if (item_value.toLowerCase() === value_lower) {
					return item;
				}
			}
			return null;
		};
		Console.information(`Loading setting ...`, []);
		let project_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(project_directory)) as ProjectSetting;
		let part_directory = PathUtility.generate_suffix_path(make_scope_child_path(project_directory, package_name), '~');
		let part_setting: PartSetting = {
			variable: [],
		};
		KernelX.JSON.write_fs_js(make_scope_setting_path(part_directory), part_setting);
		package_name = PathUtility.name(part_directory);
		let package_resource_directory = `${package_directory}/resource`;
		let package_definition = KernelX.JSON.read_fs_js<Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package>(`${package_directory}/definition.json`);
		let package_manifest_official: null | ResourceManifest.Package = null;
		let package_setting: PackageSetting = {
			name: package_name,
			part: [package_name],
			version: {
				number: package_version.number,
				extended_texture_information_for_pvz2_cn: package_version.extended_texture_information_for_pvz2_cn,
			},
			compression: {
				general: true,
				texture: true,
				filter: [],
			},
			manifest: {
				type: 'internal',
				suffix: '',
			},
			category: {
				resolution: [],
				locale: [],
			},
			conversion: conversion_setting,
			variable: [],
		};
		Console.information(`Extracting manifest ...`, []);
		do {
			let group_list = package_definition.group.filter((value) => (/^__MANIFESTGROUP__(.+)?$/i.test(value.id)));
			if (group_list.length !== 1) {
				break;
			}
			let group = group_list[0];
			if (group.subgroup.length !== 1) {
				break;
			}
			let subgroup = group.subgroup[0];
			if (subgroup.id !== group.id) {
				break;
			}
			let resource_list = subgroup.resource.filter((value) => (/^properties\/resources(.+)?\.(rton|newton)$/i.test(value.path)));
			if (resource_list.length === 0) {
				break;
			}
			package_setting.manifest.suffix = group.id.slice('__MANIFESTGROUP__'.length);
			let resource_path_list = resource_list.map((value) => (value.path.toLowerCase()));
			let resource_path: undefined | string;
			resource_path = resource_path_list.find((value) => (value.endsWith('.newton')));
			if (resource_path !== undefined) {
				package_setting.manifest.type = 'external_newton';
				let resource_data = KernelX.Storage.read_file(`${package_resource_directory}/${resource_path}`);
				let resource_data_stream = new ByteStreamView(resource_data.view().value);
				package_manifest_official = ResourceManifest.NewTypeObjectNotation.Decode.process(resource_data_stream);
				break;
			}
			resource_path = resource_path_list.find((value) => (value.endsWith('.rton')));
			if (resource_path !== undefined) {
				package_setting.manifest.type = 'external_rton_with_string_path';
				let resource_data = KernelX.Storage.read_file(`${package_resource_directory}/${resource_path}`);
				let resource_data_stream = Kernel.ByteStreamView.watch(resource_data.view());
				let resource_definition = Kernel.JSON.Value.default<Kernel.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>();
				Kernel.Tool.PopCap.ReflectionObjectNotation.Decode.process(resource_data_stream, resource_definition, Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value({ number: 1n, native_string_encoding_use_utf8: true }));
				package_manifest_official = resource_definition.value as any;
				break;
			}
		}
		while (false);
		assert_test(package_manifest_official !== null);
		Console.information(`Regularizing manifest ...`, []);
		let package_manifest = RegularResourceManifest.Convert.from_official(package_manifest_official);
		Console.information(`Converting resource ...`, []);
		for (let group_manifest of package_manifest.group) {
			let group_directory = make_scope_child_path(part_directory, group_manifest.id);
			let group_setting: GroupSetting = {
				variable: [],
			};
			let group_definition = find_item_ignore_case(package_definition.group, 'id', group_manifest.id);
			assert_test(group_definition !== null);
			KernelX.JSON.write_fs_js(make_scope_setting_path(group_directory), group_setting);
			for (let subgroup_manifest of group_manifest.subgroup) {
				let subgroup_definition = find_item_ignore_case(group_definition.subgroup, 'id', subgroup_manifest.id);
				assert_test(subgroup_definition !== null);
				{
					if (subgroup_definition.category.resolution !== null && !package_setting.category.resolution.includes(subgroup_definition.category.resolution)) {
						package_setting.category.resolution.push(subgroup_definition.category.resolution);
					}
				}
				if (package_version.number >= 3n) {
					assert_test(subgroup_definition.category.locale !== undefined);
					if (subgroup_definition.category.locale !== null && !package_setting.category.locale.includes(subgroup_definition.category.locale)) {
						package_setting.category.locale.push(subgroup_definition.category.locale);
					}
				}
				for (let resource_manifest of subgroup_manifest.resource) {
					let resource_directory = make_scope_child_path(group_directory, resource_manifest.id);
					let resource_setting: null | ResourceSetting = null;
					if (resource_manifest.additional.type === 'dummy') {
						resource_setting = {
							category: subgroup_manifest.category,
							type: 'dummy',
							property: {
							},
							variable: [],
						};
					}
					if (resource_manifest.additional.type === 'general') {
						let resource_definition = find_item_ignore_case(subgroup_definition.resource, 'path', resource_manifest.additional.value.path);
						assert_test(resource_definition !== null && resource_definition.additional.type === 'general');
						resource_setting = {
							category: subgroup_manifest.category,
							type: 'general',
							property: {
								path: resource_manifest.additional.value.path,
								type: resource_manifest.additional.value.type,
							},
							variable: [],
						};
						KernelX.Storage.copy(`${package_resource_directory}/${resource_definition.path}`, make_scope_child_path(resource_directory, 'source.bin'));
					}
					if (resource_manifest.additional.type === 'texture') {
						let resource_definition = find_item_ignore_case(subgroup_definition.resource, 'path', resource_manifest.additional.value.path + '.ptx');
						assert_test(resource_definition !== null && resource_definition.additional.type === 'texture');
						if (package_version.extended_texture_information_for_pvz2_cn >= 2n && resource_definition.additional.value.scale !== 100n) {
							Console.warning(`texture scale is not supported in package project, expected 100 but scale is ${resource_definition.additional.value.scale}`, [resource_definition.path]);
						}
						resource_setting = {
							category: subgroup_manifest.category,
							type: 'texture',
							property: {
								path: resource_manifest.additional.value.path,
								format: resource_definition.additional.value.format,
								pitch: resource_definition.additional.value.pitch,
								additional_byte_count: package_version.extended_texture_information_for_pvz2_cn < 1n ? 0n : resource_definition.additional.value.additional_byte_count,
								size: resource_definition.additional.value.size,
								sprite: resource_manifest.additional.value.sprite,
							},
							variable: [],
						};
						KernelX.Storage.copy(`${package_resource_directory}/${resource_definition.path}`, make_scope_child_path(resource_directory, 'source.ptx'));
					}
					assert_test(resource_setting !== null);
					KernelX.JSON.write_fs_js(make_scope_setting_path(resource_directory), resource_setting);
				}
			}
		}
		project_setting.package.push(package_setting);
		Console.information(`Saving setting ...`, []);
		KernelX.JSON.write_fs_js(make_scope_setting_path(project_directory), project_setting);
		return;
	}

	// ------------------------------------------------

}