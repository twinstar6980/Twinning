namespace Twinning.Script.Support.PvZ2.PackageProject.Link {

	// ------------------------------------------------

	export function link(
		project_directory: string,
		target_package: null | Array<string>,
		remake_manifest: boolean,
		buffer_size: bigint,
	): void {
		check_version_file(project_directory);
		let buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
		let project_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(project_directory)) as ProjectSetting;
		for (let package_setting of project_setting.package) {
			if (target_package !== null && !target_package.includes(package_setting.name)) {
				continue;
			}
			Console.information(`Linking package ...`, [`${package_setting.name}`]);
			if (remake_manifest) {
				Console.information(`Loading state ...`, []);
				let package_state = KernelX.JSON.read_fs_js(make_build_package_state_path(project_directory, package_setting.name)) as PackageState;
				Console.information(`Remaking manifest ...`, []);
				let package_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package = {
					group: [],
				};
				let package_manifest: RegularResourceManifest.Package = {
					group: [],
				};
				let package_compression_none = {
					general: false,
					texture: false,
				};
				let package_compression_default = {
					general: package_setting.compression.general,
					texture: package_setting.compression.texture,
				};
				let part_name_list = list_scope_child_name(`${project_directory}`, []);
				for (let part_name in package_state.part) {
					if (!part_name_list.includes(part_name) || !package_setting.part.includes(part_name)) {
						continue;
					}
					let part_state = package_state.part[part_name];
					let group_name_list = list_scope_child_name(`${project_directory}/${part_name}`, []);
					for (let group_name in part_state.group) {
						if (!group_name_list.includes(group_name)) {
							continue;
						}
						let group_state = part_state.group[group_name];
						let group_definition = package_definition.group.find((value) => (value.id === group_state.id));
						if (group_definition === undefined) {
							group_definition = {
								id: group_state.id,
								composite: true,
								subgroup: [],
							};
							package_definition.group.push(group_definition);
						}
						let group_manifest = package_manifest.group.find((value) => (value.id === group_state.id));
						if (group_manifest === undefined) {
							group_manifest = {
								id: group_state.id,
								composite: true,
								subgroup: [],
							};
							package_manifest.group.push(group_manifest);
						}
						let resource_name_list = list_scope_child_name(`${project_directory}/${part_name}/${group_name}`, []);
						for (let resource_name in group_state.resource) {
							if (!resource_name_list.includes(resource_name)) {
								continue;
							}
							let resource_state = group_state.resource[resource_name];
							for (let resource_state_item of resource_state) {
								if (resource_state_item.category.resolution !== null && !package_setting.category.resolution.includes(resource_state_item.category.resolution)) {
									continue;
								}
								if (resource_state_item.category.locale !== null && !package_setting.category.locale.includes(resource_state_item.category.locale)) {
									continue;
								}
								let subgroup_name = make_subgroup_name(group_state.id, resource_state_item.category);
								let subgroup_definition = group_definition.subgroup.find((value) => (value.id === subgroup_name));
								if (subgroup_definition === undefined) {
									subgroup_definition = {
										id: subgroup_name,
										category: {
											resolution: resource_state_item.category.resolution,
											...(package_setting.version.number < 3n ? {} : { locale: resource_state_item.category.locale }),
										},
										compression: package_setting.compression.filter.includes(group_state.id) ? package_compression_none : package_compression_default,
										resource: [],
									};
									group_definition.subgroup.push(subgroup_definition);
								}
								if (resource_state_item.definition !== null) {
									subgroup_definition.resource.push(resource_state_item.definition);
								}
								let subgroup_manifest = group_manifest.subgroup.find((value) => (value.id === subgroup_name));
								if (subgroup_manifest === undefined) {
									subgroup_manifest = {
										id: subgroup_name,
										category: resource_state_item.category,
										resource: [],
									};
									group_manifest.subgroup.push(subgroup_manifest);
								}
								subgroup_manifest.resource.push(resource_state_item.manifest);
							}
						}
					}
				}
				let package_bundle_directory = make_build_package_bundle_path(project_directory, package_setting.name);
				if (package_setting.manifest.type === 'internal') {
					throw new Error('not implemented');
				}
				if (package_setting.manifest.type === 'external_rton_with_array_path' || package_setting.manifest.type === 'external_rton_with_string_path' || package_setting.manifest.type === 'external_newton') {
					KernelX.JSON.write_fs_js(`${package_bundle_directory}/manifest.json`, null);
					let manifest_group_name = `__ManifestGroup__${package_setting.manifest.suffix}`;
					let manifesr_resource_path = `properties/resources${package_setting.manifest.suffix}`;
					if (package_setting.manifest.type === 'external_rton_with_array_path' || package_setting.manifest.type === 'external_rton_with_string_path') {
						manifesr_resource_path += '.rton';
						let manifest = RegularResourceManifest.Convert.to_official(package_manifest, package_setting.manifest.type === 'external_rton_with_array_path');
						let version_c = Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value({ number: 1n, native_string_encoding_use_utf8: true });
						let stream = Kernel.ByteStreamView.watch(buffer.view());
						Kernel.Tool.PopCap.ReflectionObjectNotation.Encode.process(stream, Kernel.JSON.Value.value(manifest as any), Kernel.Boolean.value(true), Kernel.Boolean.value(true), version_c);
						KernelX.Storage.write_file(`${package_bundle_directory}/resource/${manifesr_resource_path}`, stream.stream_view());
					}
					if (package_setting.manifest.type === 'external_newton') {
						manifesr_resource_path += '.newton';
						let manifest = RegularResourceManifest.Convert.to_official(package_manifest, false);
						let stream = new ByteStreamView(buffer.view().value);
						ResourceManifest.NewTypeObjectNotation.Encode.process(stream, manifest);
						KernelX.Storage.write_file(`${package_bundle_directory}/resource/${manifesr_resource_path}`, stream.sub(0, stream.p()));
					}
					KernelX.Storage.remove(`${package_bundle_directory}/packet/${manifest_group_name}.rsg`);
					package_definition.group.push({
						id: manifest_group_name,
						composite: false,
						subgroup: [
							{
								id: manifest_group_name,
								category: {
									resolution: null,
									...(package_setting.version.number < 3n ? {} : { locale: null }),
								},
								compression: package_compression_none,
								resource: [
									{
										path: manifesr_resource_path,
										additional: {
											type: 'general',
											value: {
											},
										},
									},
								],
							},
						],
					});
				}
				KernelX.JSON.write_fs_js(`${package_bundle_directory}/definition.json`, package_definition);
			}
			Console.information(`Packing bundle ...`, []);
			let data_file = make_build_package_data_path(project_directory, package_setting.name);
			let bundle_directory = make_build_package_bundle_path(project_directory, package_setting.name);
			let definition_file = `${bundle_directory}/definition.json`;
			let manifest_file = `${bundle_directory}/manifest.json`;
			let resource_directory = `${bundle_directory}/resource`;
			let packet_file = `${bundle_directory}/packet/{1}.rsg`;
			KernelX.Tool.PopCap.ResourceStreamBundle.pack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, packet_file, package_setting.version, buffer.view());
		}
		return;
	}

	// ------------------------------------------------

}