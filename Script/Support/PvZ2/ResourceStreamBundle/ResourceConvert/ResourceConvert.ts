namespace TwinStar.Script.Support.PvZ2.ResourceStreamBundle.ResourceConvert {

	// ------------------------------------------------

	export type TextureFormatMap = Array<{
		index: bigint;
		format: Support.PopCap.Texture.Encode.Format;
	}>;

	export type Option = {
		json: null | {
			directory: string;
			crypt: null | {
				key: string;
			};
		},
		image: null | {
			directory: string;
			texture_format_map: TextureFormatMap;
			atlas: null | {
				resize: boolean;
			};
			sprite: null | {};
		},
		animation: null | {
			directory: string;
			json: null | {};
			flash: null | {};
		},
		audio: null | {
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
		package_manifest: Core.Tool.PopCap.ResourceStreamBundle.Manifest.JS_N.Package,
		resource_manifest: ResourceManifest.Package,
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
		let iterate_manifest = (show_group_progress: boolean) => (worker: (
			group: [string, ResourceManifest.Group, Core.Tool.PopCap.ResourceStreamBundle.Manifest.JS_N.Group],
			subgroup: [string, ResourceManifest.Subgroup, Core.Tool.PopCap.ResourceStreamBundle.Manifest.JS_N.Subgroup],
			resource: [string, ResourceManifest.Resource, Core.Tool.PopCap.ResourceStreamBundle.Manifest.JS_N.Resource],
		) => void): void => {
			let group_progress = new TextGenerator.Progress('fraction', false, 40, Object.keys(package_manifest.group).length);
			for (let package_group_id in package_manifest.group) {
				group_progress.increase();
				if (show_group_progress) {
					Console.notify('i', `${group_progress} - ${package_group_id}`, []);
				}
				if (/__MANIFESTGROUP__(.+)?/.test(package_group_id)) {
					continue;
				}
				let package_group = package_manifest.group[package_group_id];
				let group_id = find_key_ignore_case(resource_manifest.group, package_group_id);
				if (group_id === null) {
					Console.notify('w', `group not found in resource manifest : ${package_group_id}`, []);
					continue;
				}
				let group = resource_manifest.group[group_id];
				for (let package_subgroup_id in package_group.subgroup) {
					let package_subgroup = package_group.subgroup[package_subgroup_id];
					let subgroup_id = find_key_ignore_case(group.subgroup, package_subgroup_id);
					if (subgroup_id === null) {
						Console.notify('w', `subgroup not found in resource manifest : ${package_subgroup_id}`, []);
						continue;
					}
					let subgroup = group.subgroup[subgroup_id];
					for (let package_resource_path in package_subgroup.resource) {
						let package_resource = package_subgroup.resource[package_resource_path];
						let resource: null | ResourceManifest.Resource = null;
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
							Console.notify('w', `resource not found in resource manifest : ${package_resource_path}`, []);
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
		{
			Console.notify('i', los(`恢复文件路径大小写 ...`), []);
			let resource_path_list: Array<string> = [];
			iterate_manifest(false)((group, subgroup, resource) => {
				resource_path_list.push(`${resource[1].path}${(resource[1].expand[0] === 'atlas' ? '.ptx' : '')}`);
			});
			let rename_tree = (
				parent: string,
				tree: PathUtility.Tree,
			) => {
				for (let name in tree) {
					//CoreX.FileSystem.rename(`${parent}/${name.toUpperCase()}`, `${parent}/${name}`);
					PathUtility.safe_rename(`${parent}/${name.toUpperCase()}`, `${parent}/${name}`);
					if (tree[name] !== null) {
						rename_tree(`${parent}/${name}`, tree[name]!);
					}
				}
			};
			let resource_path_tree = PathUtility.tree(resource_path_list);
			rename_tree(resource_directory, resource_path_tree);
		}
		Console.notify('i', los(`转换资源 ...`), []);
		let audio_temporary_directory = HomeDirectory.new_temporary();
		iterate_manifest(true)((group, subgroup, resource) => {
			let path = resource[1].path;
			if (option.json !== null && path.endsWith('.rton')) {
				Console.notify('v', `  ${path}`, []);
				try {
					if (option.json.crypt !== null) {
						CoreX.Tool.PopCap.ReflectionObjectNotation.decrypt_then_decode_fs(
							`${resource_directory}/${path}`,
							`${option.json.directory}/${path.slice(0, -4)}json`,
							{ number: 1n, native_string_encoding_use_utf8: true },
							option.json.crypt.key,
						);
					} else {
						CoreX.Tool.PopCap.ReflectionObjectNotation.decode_fs(
							`${resource_directory}/${path}`,
							`${option.json.directory}/${path.slice(0, -4)}json`,
							{ number: 1n, native_string_encoding_use_utf8: true },
						);
					}
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
			if (option.image !== null && resource[1].expand[0] === 'atlas') {
				Console.notify('v', `  ${path}`, []);
				try {
					assert_test(resource[2].additional.type === 'texture', `invalid image resource`);
					let atlas_image_information = resource[1].expand[1];
					let texture_information_source = resource[2].additional.value;
					let size = atlas_image_information.size;
					let actual_size = texture_information_source.size;
					let texture_format = option.image.texture_format_map.find((e) => (e.index === texture_information_source.format));
					assert_test(texture_format !== undefined, `unknown texture format : ${texture_information_source.format}`);
					Console.notify('v', `    size : [ ${make_prefix_padded_string(size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(size[1].toString(), ' ', 4)} ] of [ ${make_prefix_padded_string(actual_size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(actual_size[1].toString(), ' ', 4)} ] , format : ${texture_format.format}`, []);
					let data = CoreX.FileSystem.read_file(`${resource_directory}/${path}.ptx`);
					let stream = Core.ByteStreamView.watch(data.view());
					let image = Core.Image.Image.allocate(Core.Image.ImageSize.value(actual_size));
					let image_view = image.view();
					Support.PopCap.Texture.Encode.decode(stream, image_view, texture_format.format);
					if (option.image.atlas !== null) {
						let atlas_view = image_view;
						if (option.image.atlas.resize) {
							atlas_view = atlas_view.sub(Core.Image.ImagePosition.value([0n, 0n]), Core.Image.ImageSize.value(size));
						}
						CoreX.Image.File.PNG.write_fs(`${option.image.directory}/${path}.png`, atlas_view);
					}
					if (option.image.sprite !== null) {
						Support.Atlas.Pack.unpack_fsh({
							size: atlas_image_information.size,
							sprite: record_transform(atlas_image_information.sprite, (k, v) => ([v.path, { position: v.position, size: v.size }])),
						}, image_view, option.image.directory);
					}
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
			if (option.animation !== null && path.endsWith('.pam')) {
				Console.notify('v', `  ${path}`, []);
				try {
					let raw_file = `${path}.json`;
					let flash_directory = `${path}.xfl`;
					let data = CoreX.FileSystem.read_file(`${resource_directory}/${path}`);
					let stream = Core.ByteStreamView.watch(data.view());
					let version_c = Core.Tool.PopCap.Animation.Version.value({ number: 6n });
					let information = Core.Tool.PopCap.Animation.Manifest.Animation.default();
					Core.Tool.PopCap.Animation.Decode.process_animation(stream, information, version_c);
					let information_json = information.get_json(version_c);
					let information_js = information_json.value;
					if (option.animation.json !== null) {
						CoreX.JSON.write_fs(`${option.animation.directory}/${raw_file}`, information_json);
					}
					if (option.animation.flash !== null) {
						let flash_package = Support.PopCap.Animation.Convert.Flash.From.from(information_js as any);
						Support.PopCap.Animation.Convert.Flash.save_flash_package(`${option.animation.directory}/${flash_directory}`, flash_package);
						Support.PopCap.Animation.Convert.Flash.SourceManager.create_fsh(`${option.animation.directory}/${flash_directory}`, information_js as any);
						Support.PopCap.Animation.Convert.Flash.create_xfl_content_file(`${option.animation.directory}/${flash_directory}`);
					}
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
			if (option.audio !== null && path.endsWith('.wem')) {
				Console.notify('v', `  ${path}`, []);
				try {
					CoreX.Tool.Wwise.Media.decode_fs(
						`${resource_directory}/${path}`,
						`${option.audio.directory}/${path.slice(0, -3)}wav`,
						option.audio.tool.ffmpeg_program,
						option.audio.tool.ww2ogg_program,
						option.audio.tool.ww2ogg_code_book,
						audio_temporary_directory,
						{},
					);
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
		});
		return;
	}

	export function convert_fs(
		resource_directory: string,
		package_manifest_file: string,
		resource_manifest_file: string,
		option: Option,
		version: typeof Core.Tool.PopCap.ResourceStreamBundle.Version.Value,
	): void {
		let version_c = Core.Tool.PopCap.ResourceStreamBundle.Version.value(version);
		let package_manifest = CoreX.JSON.read_fs_js<Core.Tool.PopCap.ResourceStreamBundle.Manifest.JS_N.Package>(package_manifest_file);
		Console.notify('i', los(`提取资源清单 ...`), []);
		let official_resource_manifest: OfficialResourceManifest.Package;
		{
			let group_id = Object.keys(package_manifest.group).filter((e) => (/__MANIFESTGROUP__(.+)?/.test(e)));
			assert_test(group_id.length === 1, `package must has only one manifest group`);
			let group = package_manifest.group[group_id[0]];
			assert_test(!group.composite, `manifest should not be a composite group`);
			let subgroup_id = Object.keys(group.subgroup);
			assert_test(subgroup_id.length === 1, `manifest subgroup must has only one subgroup`);
			assert_test(subgroup_id[0] === group_id[0], `manifest subgroup id must equal group id`);
			let subgroup = group.subgroup[subgroup_id[0]];
			let resource_path_list = Object.keys(subgroup.resource);
			assert_test(resource_path_list.length === 1, `manifest subgroup must has one only resource`);
			let resource_path = resource_path_list[0];
			let resource = subgroup.resource[resource_path];
			assert_test(/properties\/resources(_.+)?\.rton/i.test(resource_path), `manifest resource path invalid`);
			let rton = CoreX.FileSystem.read_file(resource_directory + '/' + resource_path);
			let rton_stream = Core.ByteStreamView.watch(rton.view());
			let json = Core.JSON.Value.default<OfficialResourceManifest.Package>();
			Core.Tool.PopCap.ReflectionObjectNotation.Decode.process_whole(
				rton_stream,
				json,
				Core.Tool.PopCap.ReflectionObjectNotation.Version.value({ number: 1n, native_string_encoding_use_utf8: true }),
			);
			official_resource_manifest = json.value;
		}
		Console.notify('i', los(`解析资源清单 ...`), []);
		let resource_manifest = ResourceManifest.Convert.from_official(official_resource_manifest);
		CoreX.JSON.write_fs(resource_manifest_file, Core.JSON.Value.value(resource_manifest));
		convert(
			resource_directory,
			package_manifest,
			resource_manifest,
			option,
		);
		return;
	}

	// ------------------------------------------------

}