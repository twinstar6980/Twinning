namespace Twinning.Script.Support.PvZ2.PackageProject.Transpile {

	// ------------------------------------------------

	export type Option = {
		generalize_rton: boolean;
		generalize_ptx: boolean;
		generalize_pam: boolean;
		generalize_wem: boolean;
		specialize_rton: boolean;
		specialize_ptx: boolean;
		specialize_pam: boolean;
		specialize_wem: boolean;
	};

	function transpile_resource(
		project_directory: string,
		part_name: string,
		group_name: string,
		resource_name: string,
		conversion_setting_map: ConversionSetting,
		option: Option,
		buffer: Kernel.ByteArray,
	): void {
		let resource_directory = make_scope_root_path(project_directory, part_name, group_name, resource_name);
		let resource_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(resource_directory)) as ResourceSetting;
		let resource_type_old = resource_setting.type;
		switch (resource_setting.type) {
			case 'dummy': {
				let resource_property = resource_setting.property as DummyResourceProperty;
				break;
			}
			case 'general': {
				let resource_property = resource_setting.property as GeneralResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.bin');
				if (option.specialize_rton && resource_property.path.toLowerCase().endsWith('.rton')) {
					Console.warning(`Transpiling by specialize_rton ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = conversion_setting_map.rton.find((value) => (true));
					assert_test(conversion_setting !== undefined);
					let new_source_file = make_scope_child_path(resource_directory, 'source.json');
					KernelX.Tool.PopCap.ReflectionObjectNotation.decode_cipher_fs(source_file, new_source_file, conversion_setting.version, conversion_setting.key);
					KernelX.Storage.remove(source_file);
					resource_setting.type = 'special_rton';
					resource_setting.property = {
						conversion: conversion_setting.name,
						path: resource_property.path.slice(0, -5),
					} as SpecialRTONResourceProperty;
				}
				if (option.specialize_pam && resource_property.path.toLowerCase().endsWith('.pam')) {
					Console.warning(`Transpiling by specialize_pam ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = conversion_setting_map.pam.find((value) => (true));
					assert_test(conversion_setting !== undefined);
					let new_source_file = make_scope_child_path(resource_directory, 'source.json');
					KernelX.Tool.PopCap.Animation.decode_fs(source_file, new_source_file, conversion_setting.version);
					KernelX.Storage.remove(source_file);
					resource_setting.type = 'special_pam';
					resource_setting.property = {
						conversion: conversion_setting.name,
						path: resource_property.path.slice(0, -4),
					} as SpecialPAMResourceProperty;
				}
				if (option.specialize_wem && resource_property.path.toLowerCase().endsWith('.wem')) {
					Console.warning(`Transpiling by specialize_wem ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let new_source_file = make_scope_child_path(resource_directory, 'source.wav');
					let format = Support.Wwise.Media.Decode.decode_fs(source_file, new_source_file);
					KernelX.Storage.remove(source_file);
					let conversion_setting = conversion_setting_map.wem.find((value) => (value.format === format));
					if (conversion_setting === undefined) {
						Console.warning(`could not find conversion setting for wem format ${format}`, []);
					}
					resource_setting.type = 'special_wem';
					resource_setting.property = {
						conversion: conversion_setting === undefined ? '' : conversion_setting.name,
						path: resource_property.path.slice(0, -4),
					} as SpecialWEMResourceProperty;
				}
				break;
			}
			case 'texture': {
				let resource_property = resource_setting.property as TextureResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.ptx');
				if (option.specialize_ptx) {
					Console.warning(`Transpiling by specialize_ptx ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = conversion_setting_map.ptx.find((value) => (value.index === resource_property.format));
					assert_test(conversion_setting !== undefined, `unknown texture format : ${resource_property.format}`);
					let new_source_data_directory = make_scope_child_path(resource_directory, 'source.sprite');
					let new_sprite_property_list: Array<SpecialPTXResourcePropertySpriteProperty> = [];
					let data = KernelX.Storage.read_file(source_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(resource_property.size));
					let image_view = image.view();
					Support.PopCap.Texture.Encoding.decode(data_stream, image_view, conversion_setting.format);
					for (let source_sprite of resource_property.sprite) {
						let sprite_image_view = image_view.sub(Kernel.Image.ImagePosition.value(source_sprite.position), Kernel.Image.ImageSize.value(source_sprite.size));
						KernelX.Image.File.PNG.write_fs(make_scope_child_path(new_source_data_directory, source_sprite.path + '.png'), sprite_image_view);
						new_sprite_property_list.push({
							source: source_sprite.path,
							id: source_sprite.id,
							path: source_sprite.path,
							offset: source_sprite.offset,
							separate: source_sprite.separate,
						});
					}
					KernelX.Storage.remove(source_file);
					resource_setting.type = 'special_ptx';
					resource_setting.property = {
						conversion: conversion_setting.name,
						path: resource_property.path,
						resolution: not_null_or(resource_setting.category.resolution, 0n),
						sprite: new_sprite_property_list,
					} as SpecialPTXResourceProperty;
				}
				break;
			}
			case 'special_rton': {
				let resource_property = resource_setting.property as SpecialRTONResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.json');
				if (option.generalize_rton) {
					Console.warning(`Transpiling by generalize_rton ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = find_conversion_setting_strict(conversion_setting_map, 'rton', resource_property.conversion);
					let new_source_file = make_scope_child_path(resource_directory, 'source.bin');
					if (conversion_setting.key === null) {
						KernelX.Tool.PopCap.ReflectionObjectNotation.encode_fs(new_source_file, source_file, true, true, conversion_setting.version, buffer.view());
					}
					else {
						KernelX.Tool.PopCap.ReflectionObjectNotation.encode_cipher_fs(new_source_file, source_file, true, true, conversion_setting.version, conversion_setting.key, buffer.view());
					}
					KernelX.Storage.remove(source_file);
					resource_setting.type = 'general';
					resource_setting.property = {
						path: resource_property.path + '.rton',
						type: 'File',
					} as GeneralResourceProperty;
				}
				break;
			}
			case 'special_ptx': {
				let resource_property = resource_setting.property as SpecialPTXResourceProperty;
				let source_directory = make_scope_child_path(resource_directory, 'source.sprite');
				if (option.generalize_ptx) {
					Console.warning(`Transpiling by generalize_ptx ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = find_conversion_setting_strict(conversion_setting_map, 'ptx', resource_property.conversion);
					let new_source_file = make_scope_child_path(resource_directory, 'source.ptx');
					let new_sprite_property_list: Array<TextureResourcePropertySpriteProperty> = [];
					let source_list: Record<string, [Kernel.Image.Image, Kernel.Image.ImageView]> = {};
					for (let sprite_resource_property of resource_property.sprite) {
						let source_file = make_scope_child_path(source_directory, sprite_resource_property.source + '.png');
						let source = KernelX.Image.File.PNG.read_fs_of(source_file);
						let source_view = source.view();
						source_list[sprite_resource_property.source] = [source, source_view];
					}
					let sprite_list = source_list;
					let sprite_item_map = record_transform(sprite_list, (key, value) => ([key, { w: Number(value[0].size().value[0]), h: Number(value[0].size().value[1]) }]));
					let [atlas_box, sprite_rect_list] = Support.Atlas.PackAutomatic.pack_automatic_best(sprite_item_map, Support.Atlas.PackAutomatic.expander_exponent_of_2_generator(false));
					let atlas_size = PopCap.Texture.Encoding.compute_padded_image_size([BigInt(atlas_box.w), BigInt(atlas_box.h)], conversion_setting.format);
					let atlas = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(atlas_size));
					let atlas_view = atlas.view();
					for (let sprite_resource_property of resource_property.sprite) {
						let sprite_rect = sprite_rect_list[sprite_resource_property.source];
						let sprite_resource_manifest: TextureResourcePropertySpriteProperty = {
							id: sprite_resource_property.id,
							path: sprite_resource_property.path,
							position: [BigInt(sprite_rect.x), BigInt(sprite_rect.y)],
							size: [BigInt(sprite_rect.w), BigInt(sprite_rect.h)],
							offset: sprite_resource_property.offset,
							separate: sprite_resource_property.separate,
						};
						new_sprite_property_list.push(sprite_resource_manifest);
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
					KernelX.Storage.write_file(new_source_file, resource_data.stream_view());
					resource_setting.type = 'texture';
					resource_setting.property = {
						path: resource_property.path + '',
						format: conversion_setting.index,
						pitch: atlas_size[0] * PopCap.Texture.Encoding.get_bpp_for_pitch(conversion_setting.format) / 8n,
						additional_byte_count: conversion_setting.format !== 'rgb_etc1_a_palette' ? 0n : KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.compute_data_size_with_palette(atlas_size, texture_encode_option.rgb_etc1_a_palette!.palette.length),
						size: atlas_size,
						sprite: new_sprite_property_list,
					} as TextureResourceProperty;
				}
				break;
			}
			case 'special_pam': {
				let resource_property = resource_setting.property as SpecialWEMResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.json');
				if (option.generalize_pam) {
					Console.warning(`Transpiling by generalize_pam ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = find_conversion_setting_strict(conversion_setting_map, 'pam', resource_property.conversion);
					let new_source_file = make_scope_child_path(resource_directory, 'source.bin');
					KernelX.Tool.PopCap.Animation.encode_fs(new_source_file, source_file, conversion_setting.version, buffer.view());
					KernelX.Storage.remove(source_file);
					resource_setting.type = 'general';
					resource_setting.property = {
						path: resource_property.path + '.pam',
						type: 'PopAnim',
					} as GeneralResourceProperty;
				}
				break;
			}
			case 'special_wem': {
				let resource_property = resource_setting.property as SpecialWEMResourceProperty;
				let source_file = make_scope_child_path(resource_directory, 'source.wav');
				if (option.generalize_wem) {
					Console.warning(`Transpiling by generalize_wem ...`, [`/${part_name}/${group_name}/${resource_name}`]);
					let conversion_setting = find_conversion_setting_strict(conversion_setting_map, 'wem', resource_property.conversion);
					let new_source_file = make_scope_child_path(resource_directory, 'source.bin');
					Support.Wwise.Media.Encode.encode_fs(new_source_file, source_file, conversion_setting.format);
					KernelX.Storage.remove(source_file);
					resource_setting.type = 'general';
					resource_setting.property = {
						path: resource_property.path + '.wem',
						type: 'File',
					} as GeneralResourceProperty;
				}
				break;
			}
			default: {
				throw new Error('unknown resource type');
			}
		}
		if (resource_setting.type !== resource_type_old) {
			KernelX.JSON.write_fs_js(make_scope_setting_path(resource_directory), resource_setting);
		}
		return;
	}

	function transpile_group(
		project_directory: string,
		part_name: string,
		group_name: string,
		resource_scope: Scope0 | Scope1,
		conversion_setting_map: ConversionSetting,
		option: Option,
		buffer: Kernel.ByteArray,
	): void {
		let group_directory = make_scope_root_path(project_directory, part_name, group_name);
		for (let resource_name of list_scope_child_name(group_directory, resource_scope)) {
			transpile_resource(project_directory, part_name, group_name, resource_name, conversion_setting_map, option, buffer);
		}
		return;
	}

	function transpile_part(
		project_directory: string,
		part_name: string,
		group_scope: Scope0 | Scope1 | Scope2,
		conversion_setting_map: ConversionSetting,
		option: Option,
		buffer: Kernel.ByteArray,
	): void {
		let part_directory = make_scope_root_path(project_directory, part_name);
		for (let group_name of list_scope_child_name(part_directory, group_scope)) {
			transpile_group(project_directory, part_name, group_name, group_scope.slice(1) as any, conversion_setting_map, option, buffer);
		}
		return;
	}

	function transpile_project(
		project_directory: string,
		part_scope: Scope0 | Scope1 | Scope2 | Scope3,
		conversion_setting_map: ConversionSetting,
		option: Option,
		buffer: Kernel.ByteArray,
	): void {
		for (let part_name of list_scope_child_name(project_directory, part_scope)) {
			transpile_part(project_directory, part_name, part_scope.slice(1) as any, conversion_setting_map, option, buffer);
		}
		return;
	}

	// ------------------------------------------------

	export function transpile(
		project_directory: string,
		target_scope: null | Array<Scope>,
		target_package: string,
		option: Option,
		buffer_size: bigint,
	): void {
		check_version_file(project_directory);
		let buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
		let project_setting = KernelX.JSON.read_fs_js(make_scope_setting_path(project_directory)) as ProjectSetting;
		if (target_scope === null) {
			target_scope = [[]];
		}
		let package_setting_list = project_setting.package.filter((value) => (value.name === target_package));
		assert_test(package_setting_list.length === 1);
		let package_setting = package_setting_list[0];
		for (let scope of target_scope) {
			transpile_project(project_directory, scope, package_setting.conversion, option, buffer);
		}
		return;
	}

	// ------------------------------------------------

}