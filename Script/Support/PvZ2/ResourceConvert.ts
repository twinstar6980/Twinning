namespace TwinStar.Script.Support.PvZ2.ResourceConvert {

	// ------------------------------------------------

	export type TextureFormatMap = Array<{
		index: bigint;
		format: Support.PopCap.Texture.Encoding.Format;
	}>;

	export type Option = {
		recase_path: boolean,
		rton: null | {
			directory: string;
			version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
			crypt: null | {
				key: string;
			};
		},
		ptx: null | {
			directory: string;
			texture_format_map: TextureFormatMap;
			atlas: null | {
				resize: boolean;
			};
			sprite: null | {
			};
		},
		pam: null | {
			directory: string;
			version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
			json: null | {
			};
			flash: null | {
			};
		},
		bnk: null | {
			directory: string;
			version: typeof Kernel.Tool.Wwise.SoundBank.Version.Value,
		},
		wem: null | {
			directory: string;
			tool: {
				ffmpeg_program: string;
				ww2ogg_program: string;
				ww2ogg_code_book: string;
			};
		},
	};

	export function convert(
		resource_directory: string,
		package_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package,
		resource_manifest: RegularResourceManifest.Package,
		option: Option,
	): void {
		let find_key_ignore_case = <T>(map: Record<string, T>, key: string): string | null => {
			let key_lower = key.toLowerCase();
			for (let key_in_map in map) {
				if (key_in_map.toLowerCase() === key_lower) {
					return key_in_map;
				}
			}
			return null;
		};
		let iterate_resource = (show_group_progress: boolean) => (worker: (
			group: [string, RegularResourceManifest.Group, Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Group],
			subgroup: [string, RegularResourceManifest.Subgroup, Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Subgroup],
			resource: [string, RegularResourceManifest.Resource, Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Resource],
		) => void): void => {
			let group_progress = new TextGenerator.Progress('fraction', false, 40, Object.keys(package_definition.group).length);
			for (let package_group_id in package_definition.group) {
				group_progress.increase();
				if (show_group_progress) {
					Console.information(`${group_progress} - ${package_group_id}`, []);
				}
				if (/__MANIFESTGROUP__(.+)?/.test(package_group_id)) {
					continue;
				}
				let package_group = package_definition.group[package_group_id];
				let group_id = find_key_ignore_case(resource_manifest.group, package_group_id);
				if (group_id === null) {
					Console.warning(`group not found in resource manifest : ${package_group_id}`, []);
					continue;
				}
				let group = resource_manifest.group[group_id];
				for (let package_subgroup_id in package_group.subgroup) {
					let package_subgroup = package_group.subgroup[package_subgroup_id];
					let subgroup_id = find_key_ignore_case(group.subgroup, package_subgroup_id);
					if (subgroup_id === null) {
						Console.warning(`subgroup not found in resource manifest : ${package_subgroup_id}`, []);
						continue;
					}
					let subgroup = group.subgroup[subgroup_id];
					for (let package_resource_path in package_subgroup.resource) {
						let package_resource = package_subgroup.resource[package_resource_path];
						let resource: null | RegularResourceManifest.Resource = null;
						let resource_id = '';
						for (let k in subgroup.resource) {
							let resource_path = package_resource_path.toLowerCase();
							if (resource_path.endsWith('.ptx')) {
								resource_path = resource_path.slice(0, -4);
							}
							if (subgroup.resource[k].path.toLowerCase() === resource_path) {
								resource_id = k;
								resource = subgroup.resource[k];
								break;
							}
						}
						if (resource === null) {
							Console.warning(`resource not found in resource manifest : ${package_resource_path}`, []);
							continue;
						}
						worker(
							[group_id, group, package_group],
							[subgroup_id, subgroup, package_subgroup],
							[resource_id, resource, package_resource],
						);
					}
				}
			}
			return;
		};
		if (option.recase_path) {
			Console.information(los('support.pvz2.resource_convert:recase_resource_path'), [
			]);
			let resource_path_list: Array<string> = [];
			iterate_resource(false)((group, subgroup, resource) => {
				resource_path_list.push(`${resource[1].path}${(resource[1].additional.type === 'atlas' ? '.ptx' : '')}`);
			});
			let rename_tree = (
				parent: string,
				tree: PathUtility.Tree,
			) => {
				for (let name in tree) {
					try {
						//KernelX.FileSystem.rename(`${parent}/${name.toUpperCase()}`, `${parent}/${name}`);
						PathUtility.safe_rename(`${parent}/${name.toUpperCase()}`, `${parent}/${name}`);
					} catch (e) {
						Console.error_of(e);
					}
					if (tree[name] !== null) {
						rename_tree(`${parent}/${name}`, tree[name]!);
					}
				}
			};
			let resource_path_tree = PathUtility.tree(resource_path_list);
			rename_tree(resource_directory, resource_path_tree);
		}
		Console.information(los('support.pvz2.resource_convert:convert_resource'), [
		]);
		let audio_temporary_directory = Home.new_temporary();
		iterate_resource(true)((group, subgroup, resource) => {
			let path = resource[1].path;
			if (option.rton !== null && path.endsWith('.rton')) {
				Console.verbosity(`  ${path}`, []);
				try {
					if (option.rton.crypt !== null) {
						KernelX.Tool.PopCap.ReflectionObjectNotation.decrypt_then_decode_fs(
							`${resource_directory}/${path}`,
							`${option.rton.directory}/${path.slice(0, -4)}json`,
							option.rton.version,
							option.rton.crypt.key,
						);
					} else {
						KernelX.Tool.PopCap.ReflectionObjectNotation.decode_fs(
							`${resource_directory}/${path}`,
							`${option.rton.directory}/${path.slice(0, -4)}json`,
							option.rton.version,
						);
					}
				} catch (e) {
					Console.error_of(e);
				}
			}
			if (option.ptx !== null && resource[1].additional.type === 'atlas') {
				Console.verbosity(`  ${path}`, []);
				try {
					assert_test(resource[2].additional.type === 'texture', `invalid image resource`);
					let atlas_image_additional = resource[1].additional.value;
					let texture_additional_source = resource[2].additional.value;
					let size = atlas_image_additional.size;
					let actual_size = texture_additional_source.size;
					let texture_format = option.ptx.texture_format_map.find((e) => (e.index === texture_additional_source.format));
					assert_test(texture_format !== undefined, `unknown texture format : ${texture_additional_source.format}`);
					Console.verbosity(`    size : [ ${make_prefix_padded_string(size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(size[1].toString(), ' ', 4)} ] of [ ${make_prefix_padded_string(actual_size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(actual_size[1].toString(), ' ', 4)} ] , format : ${texture_format.format}`, []);
					let data = KernelX.FileSystem.read_file(`${resource_directory}/${path}.ptx`);
					let stream = Kernel.ByteStreamView.watch(data.view());
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(actual_size));
					let image_view = image.view();
					Support.PopCap.Texture.Encoding.decode(stream, image_view, texture_format.format);
					if (option.ptx.atlas !== null) {
						let atlas_view = image_view;
						if (option.ptx.atlas.resize) {
							atlas_view = atlas_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(size));
						}
						KernelX.Image.File.PNG.write_fs(`${option.ptx.directory}/${path}.png`, atlas_view);
					}
					if (option.ptx.sprite !== null) {
						Support.Atlas.Pack.unpack_fsh({
							size: atlas_image_additional.size,
							sprite: record_transform(atlas_image_additional.sprite, (key, value) => ([value.path, { position: value.position, size: value.size }])),
						}, image_view, option.ptx.directory);
					}
				} catch (e) {
					Console.error_of(e);
				}
			}
			if (option.pam !== null && path.endsWith('.pam')) {
				Console.verbosity(`  ${path}`, []);
				try {
					let raw_file = `${path}.json`;
					let flash_directory = `${path}.xfl`;
					let data = KernelX.FileSystem.read_file(`${resource_directory}/${path}`);
					let stream = Kernel.ByteStreamView.watch(data.view());
					let version_c = Kernel.Tool.PopCap.Animation.Version.value(option.pam.version);
					let definition = Kernel.Tool.PopCap.Animation.Definition.Animation.default();
					Kernel.Tool.PopCap.Animation.Decode.process(stream, definition, version_c);
					let definition_json = definition.get_json(version_c);
					let definition_js = definition_json.value;
					if (option.pam.json !== null) {
						KernelX.JSON.write_fs(`${option.pam.directory}/${raw_file}`, definition_json);
					}
					if (option.pam.flash !== null) {
						let flash_package = Support.PopCap.Animation.Convert.Flash.From.from(definition_js as any);
						Support.PopCap.Animation.Convert.Flash.save_flash_package(`${option.pam.directory}/${flash_directory}`, flash_package);
						Support.PopCap.Animation.Convert.Flash.SourceManager.create_fsh(`${option.pam.directory}/${flash_directory}`, definition_js as any);
						Support.PopCap.Animation.Convert.Flash.create_xfl_content_file(`${option.pam.directory}/${flash_directory}`);
					}
				} catch (e) {
					Console.error_of(e);
				}
			}
			if (option.bnk !== null && path.endsWith('.bnk')) {
				Console.verbosity(`  ${path}`, []);
				try {
					KernelX.Tool.Wwise.SoundBank.decode_fs(
						`${resource_directory}/${path}`,
						`${option.bnk.directory}/${path}.bundle/definition.json`,
						`${option.bnk.directory}/${path}.bundle/embedded_media`,
						option.bnk.version,
					);
				} catch (e) {
					Console.error_of(e);
				}
			}
			if (option.wem !== null && path.endsWith('.wem')) {
				Console.verbosity(`  ${path}`, []);
				try {
					KernelX.Tool.Wwise.Media.decode_fs(
						`${resource_directory}/${path}`,
						`${option.wem.directory}/${path.slice(0, -3)}wav`,
						option.wem.tool.ffmpeg_program,
						option.wem.tool.ww2ogg_program,
						option.wem.tool.ww2ogg_code_book,
						audio_temporary_directory,
						{},
					);
				} catch (e) {
					Console.error_of(e);
				}
			}
		});
		return;
	}

	export function convert_fs(
		resource_directory: string,
		package_definition_file: string,
		resource_manifest_file: string,
		option: Option,
	): void {
		let package_definition = KernelX.JSON.read_fs_js<Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package>(package_definition_file);
		Console.information(los('support.pvz2.resource_convert:extract_resource_manifest'), []);
		let resource_manifest: ResourceManifest.Package;
		{
			let group_id_list = Object.keys(package_definition.group).filter((value) => (/^__MANIFESTGROUP__(.+)?$/i.test(value)));
			assert_test(group_id_list.length === 1, `package must has only one MANIFEST group`);
			let group_id = group_id_list[0];
			let group = package_definition.group[group_id];
			assert_test(!group.composite, `MANIFEST should not be a composite group`);
			let subgroup_id_list = Object.keys(group.subgroup);
			assert_test(subgroup_id_list.length === 1, `MANIFEST subgroup must has only one subgroup`);
			let subgroup_id = subgroup_id_list[0];
			assert_test(subgroup_id === group_id, `MANIFEST subgroup id must equal group id`);
			let subgroup = group.subgroup[subgroup_id];
			let resource_path_list = Object.keys(subgroup.resource).filter((value) => (/^properties\/resources(.+)?\.(rton|newton)$/i.test(value)));
			assert_test(resource_path_list.length !== 0, `MANIFEST subgroup must contains manifest file`);
			let resource_path: string;
			if (resource_path_list.length === 1) {
				Console.information(los('support.pvz2.resource_convert:resource_manifest_found_single'), [resource_path_list[0]]);
				resource_path = resource_path_list[0];
			} else {
				Console.information(los('support.pvz2.resource_convert:resource_manifest_found_multi'), []);
				resource_path = Console.option(Console.option_string(resource_path_list), null, null);
			}
			Console.information(los('support.pvz2.resource_convert:parse_resource_manifest'), []);
			if (resource_path.toLowerCase().endsWith('rton')) {
				let data = KernelX.FileSystem.read_file(`${resource_directory}/${resource_path}`);
				let stream = Kernel.ByteStreamView.watch(data.view());
				let result = Kernel.JSON.Value.default<ResourceManifest.Package>();
				Kernel.Tool.PopCap.ReflectionObjectNotation.Decode.process(
					stream,
					result as any,
					Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value({ number: 1n, native_string_encoding_use_utf8: true }),
				);
				resource_manifest = result.value;
			}
			if (resource_path.toLowerCase().endsWith('newton')) {
				let data = KernelX.FileSystem.read_file(`${resource_directory}/${resource_path}`);
				let stream = new ByteStreamView(data.view().value);
				let result = ResourceManifest.NewTypeObjectNotation.Decode.process(
					stream,
				);
				resource_manifest = result;
			}
			resource_manifest = resource_manifest!;
		}
		let regular_resource_manifest = RegularResourceManifest.Convert.from_official(resource_manifest);
		KernelX.JSON.write_fs(resource_manifest_file, Kernel.JSON.Value.value(regular_resource_manifest));
		convert(
			resource_directory,
			package_definition,
			regular_resource_manifest,
			option,
		);
		return;
	}

	// ------------------------------------------------

}