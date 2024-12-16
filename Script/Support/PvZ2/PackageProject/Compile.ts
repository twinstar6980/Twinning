namespace Twinning.Script.Support.PvZ2.PackageProject.Compile {

	// ------------------------------------------------

	function compile_resource(
		project_directory: string,
		part_name: string,
		group_name: string,
		resource_name: string,
		package_setting: PackageSetting,
		variable_list: Array<Variable>,
		resource_state: ResourceState,
		group_id: string,
		buffer: Kernel.ByteArray,
	): void {
		Console.information(`Compiling ...`, [`${package_setting.name}/${part_name}/${group_name}/${resource_name}`]);
		let resource_directory = make_scope_root_path(project_directory, part_name, group_name, resource_name);
		let resource_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(resource_directory)) as ResourceSetting;
		if (resource_setting.category.resolution !== null && !package_setting.category.resolution.includes(resource_setting.category.resolution)) {
			return;
		}
		if (resource_setting.category.locale !== null && !package_setting.category.locale.includes(resource_setting.category.locale)) {
			return;
		}
		variable_list = merge_variable_list(variable_list, resource_setting.variable);
		let runtime_variable = {
			group: {
				name: '#group',
				value: group_id,
			},
			resolution: {
				name: '#resolution',
				value: not_undefined_or(resource_setting.category.resolution?.toString(), ''),
			},
			locale: {
				name: '#locale',
				value: not_undefined_or(resource_setting.category.locale?.toString(), ''),
			},
		};
		variable_list.unshift(runtime_variable.group, runtime_variable.resolution, runtime_variable.locale);
		let resource_id = parse_variable_string(resource_name, variable_list).toUpperCase();
		switch (resource_setting.type) {
			case 'dummy': {
				let resource_property = resource_setting.property as DummyResourceProperty;
				{
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_setting.category));
					resource_state.push({
						category: resource_setting.category,
						definition: null,
						manifest: {
							id: resource_id,
							additional: {
								type: 'dummy',
								value: {
								},
							},
						},
					});
				}
				break;
			}
			case 'general': {
				let resource_property = resource_setting.property as GeneralResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.bin');
				{
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_setting.category));
					let resource_path = parse_variable_string(resource_property.path, variable_list);
					let package_resource_file = make_build_package_bundle_resource_path(project_directory, package_setting.name, resource_path);
					KernelX.Storage.remove(package_resource_file);
					if (KernelX.is_windows || KernelX.is_linux || KernelX.is_macintosh) {
						KernelX.Storage.create_link(package_resource_file, source_file, false);
					}
					else {
						KernelX.Storage.copy(source_file, package_resource_file);
					}
					resource_state.push({
						category: resource_setting.category,
						definition: {
							path: resource_path,
							additional: {
								type: 'general',
								value: {
								},
							},
						},
						manifest: {
							id: resource_id,
							additional: {
								type: 'general',
								value: {
									path: resource_path,
									type: resource_property.type,
								},
							},
						},
					});
				}
				break;
			}
			case 'texture': {
				let resource_property = resource_setting.property as TextureResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.ptx');
				{
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_setting.category));
					let resource_path = parse_variable_string(resource_property.path, variable_list) + '.ptx';
					let package_resource_file = make_build_package_bundle_resource_path(project_directory, package_setting.name, resource_path);
					KernelX.Storage.remove(package_resource_file);
					if (KernelX.is_windows || KernelX.is_linux || KernelX.is_macintosh) {
						KernelX.Storage.create_link(package_resource_file, source_file, false);
					}
					else {
						KernelX.Storage.copy(source_file, package_resource_file);
					}
					let resource_manifest_additional_sprite: Array<TextureResourcePropertySpriteProperty> = [];
					for (let sprite_resource_property of resource_property.sprite) {
						resource_manifest_additional_sprite.push({
							id: parse_variable_string(sprite_resource_property.id, variable_list),
							path: parse_variable_string(sprite_resource_property.path, variable_list),
							position: sprite_resource_property.position,
							size: sprite_resource_property.size,
							offset: sprite_resource_property.offset,
							separate: sprite_resource_property.separate,
						});
					}
					resource_state.push({
						category: resource_setting.category,
						definition: {
							path: resource_path,
							additional: {
								type: 'texture',
								value: {
									size: resource_property.size,
									format: resource_property.format,
									pitch: resource_property.pitch,
									...(package_setting.version.extended_texture_information_for_pvz2_cn < 1n ? {} : { additional_byte_count: resource_property.additional_byte_count }),
									...(package_setting.version.extended_texture_information_for_pvz2_cn < 2n ? {} : { scale: 100n }),
								},
							},
						},
						manifest: {
							id: resource_id,
							additional: {
								type: 'texture',
								value: {
									path: resource_path.slice(0, -4),
									size: resource_property.size,
									sprite: resource_manifest_additional_sprite,
								},
							},
						},
					});
				}
				break;
			}
			case 'special_rton': {
				let resource_property = resource_setting.property as SpecialRTONResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.json');
				let conversion_setting = find_conversion_setting_strict(package_setting.conversion, 'rton', resource_property.conversion);
				{
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_setting.category));
					let resource_path = parse_variable_string(resource_property.path, variable_list) + '.rton';
					let package_resource_file = make_build_package_bundle_resource_path(project_directory, package_setting.name, resource_path);
					KernelX.Storage.remove(package_resource_file);
					KernelX.Tool.PopCap.ReflectionObjectNotation.encode_cipher_fs(package_resource_file, source_file, true, true, conversion_setting.version, conversion_setting.key, buffer.view());
					resource_state.push({
						category: resource_setting.category,
						definition: {
							path: resource_path,
							additional: {
								type: 'general',
								value: {
								},
							},
						},
						manifest: {
							id: resource_id,
							additional: {
								type: 'general',
								value: {
									path: resource_path,
									type: 'File',
								},
							},
						},
					});
				}
				break;
			}
			case 'special_ptx': {
				let resource_property = resource_setting.property as SpecialPTXResourceProperty;
				let source_directory = make_scope_child_path(resource_directory, 'source.sprite');
				let conversion_setting = find_conversion_setting_strict(package_setting.conversion, 'ptx', resource_property.conversion);
				let source_list: Record<string, [Kernel.Image.Image, Kernel.Image.ImageView]> = {};
				for (let sprite_resource_property of resource_property.sprite) {
					let source_file = make_scope_child_path(source_directory, sprite_resource_property.source + '.png');
					let source = KernelX.Image.File.PNG.read_fs_of(source_file);
					let source_view = source.view();
					source_list[sprite_resource_property.source] = [source, source_view];
				}
				for (let resource_resolution of resource_setting.category.resolution !== null ? [resource_setting.category.resolution] : package_setting.category.resolution) {
					let resource_category = {
						resolution: resource_resolution,
						locale: resource_setting.category.locale,
					};
					runtime_variable.resolution.value = resource_resolution.toString();
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_category));
					let resource_path = parse_variable_string(resource_property.path, variable_list) + '.ptx';
					let package_resource_file = make_build_package_bundle_resource_path(project_directory, package_setting.name, resource_path);
					KernelX.Storage.remove(package_resource_file);
					let sprite_resource_manifest_list: Array<TextureResourcePropertySpriteProperty> = [];
					let sprite_list: Record<string, [null | Kernel.Image.Image, Kernel.Image.ImageView]>;
					if (resource_resolution === resource_property.resolution) {
						sprite_list = source_list;
					}
					else {
						let scale_rate = Number(resource_resolution) / Number(resource_property.resolution);
						sprite_list = {};
						for (let sprite_name in source_list) {
							let sprite_view = source_list[sprite_name][1];
							let current_sprite_size: [bigint, bigint] = [
								BigInt(Math.ceil(Number(sprite_view.size().value[0]) * scale_rate)),
								BigInt(Math.ceil(Number(sprite_view.size().value[1]) * scale_rate)),
							];
							let current_sprite = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(current_sprite_size));
							let current_sprite_view = current_sprite.view();
							Kernel.Tool.Texture.Transformation.Scale.process(sprite_view, current_sprite_view);
						}
					}
					let sprite_item_map = record_transform(sprite_list, (key, value) => ([key, { w: Number(value[1].size().value[0]), h: Number(value[1].size().value[1]) }]));
					let [atlas_box, sprite_rect_list] = Support.Atlas.PackAutomatic.pack_automatic_best(sprite_item_map, Support.Atlas.PackAutomatic.expander_exponent_of_2_generator(false));
					let atlas_size = PopCap.Texture.Encoding.compute_padded_image_size([BigInt(atlas_box.w), BigInt(atlas_box.h)], conversion_setting.format);
					let atlas = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(atlas_size));
					let atlas_view = atlas.view();
					for (let sprite_resource_property of resource_property.sprite) {
						let sprite_rect = sprite_rect_list[sprite_resource_property.source];
						let sprite_resource_manifest: TextureResourcePropertySpriteProperty = {
							id: parse_variable_string(sprite_resource_property.id, variable_list),
							path: parse_variable_string(sprite_resource_property.path, variable_list),
							position: [BigInt(sprite_rect.x), BigInt(sprite_rect.y)],
							size: [BigInt(sprite_rect.w), BigInt(sprite_rect.h)],
							offset: sprite_resource_property.offset,
							separate: sprite_resource_property.separate,
						};
						sprite_resource_manifest_list.push(sprite_resource_manifest);
						let sprite_view = atlas_view.sub(Kernel.Image.ImagePosition.value(sprite_resource_manifest.position), Kernel.Image.ImageSize.value(sprite_resource_manifest.size));
						sprite_view.draw(sprite_list[sprite_resource_property.source][1]);
					}
					let texture_encode_option: PopCap.Texture.Encoding.EncodeOption = {
						rgb_etc1_a_palette: null,
					};
					if (conversion_setting.format === 'rgb_etc1_a_palette') {
						texture_encode_option.rgb_etc1_a_palette = {
							palette: KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.evaluate_palette(atlas_view),
						};
					}
					let resource_data_size = Support.PopCap.Texture.Encoding.compute_data_size(atlas_size, conversion_setting.format, texture_encode_option);
					if (buffer.size().value < resource_data_size) {
						buffer.allocate(Kernel.Size.value(resource_data_size));
					}
					let resource_data = Kernel.ByteStreamView.watch(buffer.view());
					Support.PopCap.Texture.Encoding.encode(atlas_view, resource_data, conversion_setting.format, texture_encode_option);
					KernelX.Storage.write_file(package_resource_file, resource_data.stream_view());
					resource_state.push({
						category: resource_category,
						definition: {
							path: resource_path,
							additional: {
								type: 'texture',
								value: {
									size: atlas_size,
									format: conversion_setting.index,
									pitch: atlas_size[0] * PopCap.Texture.Encoding.get_bpp_for_pitch(conversion_setting.format) / 8n,
									...(package_setting.version.extended_texture_information_for_pvz2_cn < 1n ? {} : { additional_byte_count: conversion_setting.format !== 'rgb_etc1_a_palette' ? 0n : KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.compute_data_size_with_palette(atlas_size, texture_encode_option.rgb_etc1_a_palette!.palette.length) }),
									...(package_setting.version.extended_texture_information_for_pvz2_cn < 2n ? {} : { scale: 100n }),
								},
							},
						},
						manifest: {
							id: resource_id,
							additional: {
								type: 'texture',
								value: {
									path: resource_path.slice(0, -4),
									size: atlas_size,
									sprite: sprite_resource_manifest_list,
								},
							},
						},
					});
				}
				break;
			}
			case 'special_pam': {
				let resource_property = resource_setting.property as SpecialWEMResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.json');
				let conversion_setting = find_conversion_setting_strict(package_setting.conversion, 'pam', resource_property.conversion);
				{
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_setting.category));
					let resource_path = parse_variable_string(resource_property.path, variable_list) + '.pam';
					let package_resource_file = make_build_package_bundle_resource_path(project_directory, package_setting.name, resource_path);
					KernelX.Storage.remove(package_resource_file);
					KernelX.Tool.PopCap.Animation.encode_fs(package_resource_file, source_file, conversion_setting.version, buffer.view());
					resource_state.push({
						category: resource_setting.category,
						definition: {
							path: resource_path,
							additional: {
								type: 'general',
								value: {
								},
							},
						},
						manifest: {
							id: resource_id,
							additional: {
								type: 'general',
								value: {
									path: resource_path,
									type: 'PopAnim',
								},
							},
						},
					});
				}
				break;
			}
			case 'special_wem': {
				let resource_property = resource_setting.property as SpecialWEMResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.wav');
				let conversion_setting = find_conversion_setting_strict(package_setting.conversion, 'wem', resource_property.conversion);
				{
					KernelX.Storage.remove(make_build_package_bundle_packet_path(project_directory, package_setting.name, group_id, resource_setting.category));
					let resource_path = parse_variable_string(resource_property.path, variable_list) + '.wem';
					let package_resource_file = make_build_package_bundle_resource_path(project_directory, package_setting.name, resource_path);
					KernelX.Storage.remove(package_resource_file);
					Support.Wwise.Media.Encode.encode_fs(source_file, package_resource_file, conversion_setting.format);
					resource_state.push({
						category: resource_setting.category,
						definition: {
							path: resource_path,
							additional: {
								type: 'general',
								value: {
								},
							},
						},
						manifest: {
							id: resource_id,
							additional: {
								type: 'general',
								value: {
									path: resource_path,
									type: 'File',
								},
							},
						},
					});
				}
				break;
			}
			default: {
				throw new Error('unknown resource type');
			}
		}
		return;
	}

	function compile_group(
		project_directory: string,
		part_name: string,
		group_name: string,
		resource_scope: Scope0 | Scope1,
		package_setting: PackageSetting,
		variable_list: Array<Variable>,
		group_state: GroupState,
		buffer: Kernel.ByteArray,
	): void {
		let group_directory = make_scope_root_path(project_directory, part_name, group_name);
		let group_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(group_directory)) as GroupSetting;
		variable_list = merge_variable_list(variable_list, group_setting.variable);
		group_state.id = parse_variable_string(group_name, variable_list);
		for (let resource_name of list_scope_child_name(group_directory, resource_scope)) {
			compile_resource(project_directory, part_name, group_name, resource_name, package_setting, variable_list, query_state_of_resource(group_state, resource_name), group_state.id, buffer);
		}
		return;
	}

	function compile_part(
		project_directory: string,
		part_name: string,
		group_scope: Scope0 | Scope1 | Scope2,
		package_setting: PackageSetting,
		variable_list: Array<Variable>,
		part_state: PartState,
		buffer: Kernel.ByteArray,
	): void {
		let part_directory = make_scope_root_path(project_directory, part_name);
		let part_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(part_directory)) as PartSetting;
		variable_list = merge_variable_list(variable_list, part_setting.variable);
		for (let group_name of list_scope_child_name(part_directory, group_scope)) {
			compile_group(project_directory, part_name, group_name, group_scope.slice(1) as any, package_setting, variable_list, query_state_of_group(part_state, group_name), buffer);
		}
		return;
	}

	function compile_project(
		project_directory: string,
		part_scope: Scope0 | Scope1 | Scope2 | Scope3,
		package_setting: PackageSetting,
		variable_list: Array<Variable>,
		package_state: PackageState,
		buffer: Kernel.ByteArray,
	): void {
		variable_list = merge_variable_list(variable_list, package_setting.variable);
		for (let part_name of list_scope_child_name(project_directory, part_scope)) {
			if (!package_setting.part.includes(part_name)) {
				continue;
			}
			compile_part(project_directory, part_name, part_scope.slice(1) as any, package_setting, variable_list, query_state_of_part(package_state, part_name), buffer);
		}
		return;
	}

	// ------------------------------------------------

	export function compile(
		project_directory: string,
		target_scope: null | Array<Scope>,
		target_package: null | Array<string>,
		buffer_size: bigint,
	): void {
		check_version_file(project_directory);
		let buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
		let project_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(project_directory)) as ProjectSetting;
		if (target_scope === null) {
			target_scope = [[]];
		}
		for (let package_setting of project_setting.package) {
			if (target_package !== null && !target_package.includes(package_setting.name)) {
				continue;
			}
			Console.information(`Loading state ...`, []);
			let package_state_file = make_build_package_state_path(project_directory, package_setting.name);
			let package_state: PackageState;
			try {
				package_state = KernelX.JSON.read_fs_js(package_state_file) as PackageState;
			}
			catch (e) {
				package_state = {
					part: {},
				};
			}
			for (let scope of target_scope) {
				compile_project(project_directory, scope, package_setting, [], package_state, buffer);
			}
			Console.information(`Saving state ...`, []);
			KernelX.JSON.write_fs_js(package_state_file, package_state, true, true, true, true);
		}
		return;
	}

	// ------------------------------------------------

}