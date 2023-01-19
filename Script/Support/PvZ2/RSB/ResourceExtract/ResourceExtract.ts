/** PvZ2-RSB资源提取 */
namespace TwinStar.Support.PvZ2.RSB.ResourceExtract {

	// ------------------------------------------------

	/** 纹理索引表 */
	export type TextureFormatMap = Array<{
		index: bigint;
		format: Support.PopCapTexture.Encode.Format;
	}>;

	/** 提取选项 */
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

	/**
	 * 从资源目录中提取资源
	 * @param package_manifest 包清单
	 * @param resource_manifest 资源清单
	 * @param resource_directory 资源目录路径
	 * @param option 提取选项
	 */
	export function extract(
		package_manifest: Core.Tool.PopCap.RSB.Manifest.JS_N.Package,
		resource_manifest: ResourceManifest.Package,
		resource_directory: string,
		option: Option,
	): void {
		let iterate_manifest = (show_group_progress: boolean) => (work: (
			group: [string, ResourceManifest.Group, Core.Tool.PopCap.RSB.Manifest.JS_N.Group],
			subgroup: [string, ResourceManifest.Subgroup, Core.Tool.PopCap.RSB.Manifest.JS_N.Subgroup],
			resource: [string, ResourceManifest.Resource, Core.Tool.PopCap.RSB.Manifest.JS_N.Resource],
		) => void): void => {
			let group_progress = new TextGenerator.Progress('fraction', false, 40, Object.keys(package_manifest.group).length);
			for (let group_id in package_manifest.group) {
				group_progress.increase();
				if (show_group_progress) {
					Console.notify('i', `${group_progress} - ${group_id}`, []);
				}
				if (group_id.startsWith('__MANIFESTGROUP__')) {
					continue;
				}
				let package_group = package_manifest.group[group_id];
				let group = resource_manifest.group[group_id];
				if (group === undefined) {
					throw new Error(`group not found in resource manifest : ${group_id}`);
				}
				for (let subgroup_id in package_group.subgroup) {
					let package_subgroup = package_group.subgroup[subgroup_id];
					let subgroup = group.subgroup[subgroup_id];
					if (subgroup === undefined) {
						throw new Error(`subgroup not found in resource manifest : ${subgroup_id}`);
					}
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
							throw new Error(`resource not found in resource manifest : ${package_resource_path}`);
						}
						work(
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
					CoreX.FileSystem.rename(`${parent}/${name.toUpperCase()}`, `${parent}/${name}`, Shell.is_android);
					if (tree[name] !== null) {
						rename_tree(`${parent}/${name}`, tree[name]!);
					}
				}
			};
			let resource_path_tree = PathUtility.to_tree(resource_path_list);
			rename_tree(resource_directory, resource_path_tree);
		}
		Console.notify('i', los(`提取资源 ...`), []);
		let audio_temporary_directory = HomeDirectory.new_temporary();
		iterate_manifest(true)((group, subgroup, resource) => {
			let path = resource[1].path;
			if (option.json !== null && path.endsWith('.rton')) {
				Console.notify('v', `  ${path}`, []);
				try {
					if (option.json.crypt !== null) {
						CoreX.Tool.PopCap.RTON.decrypt_then_decode_fs(
							`${resource_directory}/${path}`,
							`${option.json.directory}/${path.slice(0, -4)}json`,
							option.json.crypt.key,
						);
					} else {
						CoreX.Tool.PopCap.RTON.decode_fs(
							`${resource_directory}/${path}`,
							`${option.json.directory}/${path.slice(0, -4)}json`,
						);
					}
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
			if (option.image !== null && resource[1].expand[0] === 'atlas') {
				Console.notify('v', `  ${path}`, []);
				try {
					if (!(option.image !== null && resource[1].expand[0] === 'atlas')) {
						throw new Error(`NEVER`);
					}
					if (resource[2].additional.type !== 'texture') {
						throw new Error(`invalid image resource`);
					}
					let atlas_image_information = resource[1].expand[1];
					let texture_information_source = resource[2].additional.value;
					let size = atlas_image_information.size;
					let actual_size = texture_information_source.size;
					let texture_format = option.image.texture_format_map.find((e) => (e.index === texture_information_source.format));
					if (texture_format === undefined) {
						throw new Error(`unknown texture format : ${texture_information_source.format}`);
					}
					Console.notify('v', `    size : [ ${make_prefix_padded_string(size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(size[1].toString(), ' ', 4)} ] , actual_size : [ ${make_prefix_padded_string(actual_size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(actual_size[1].toString(), ' ', 4)} ] , format : ${texture_format.format}`, []);
					let data = CoreX.FileSystem.read_file(`${resource_directory}/${path}.ptx`);
					let stream = Core.ByteStreamView.look(data.view());
					let image = Core.Image.Bitmap.allocate(Core.Image.ImageSize.value(actual_size));
					let image_view = image.view();
					Support.PopCapTexture.Encode.decode(stream, image_view, texture_format.format);
					if (option.image.atlas !== null) {
						let atlas_view = image_view;
						if (option.image.atlas.resize) {
							atlas_view = atlas_view.sub(Core.Image.ImagePosition.value([0n, 0n]), Core.Image.ImageSize.value(size));
						}
						CoreX.Image.File.png_write_file(`${option.image.directory}/${path}.png`, atlas_view);
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
					let stream = Core.ByteStreamView.look(data.view());
					let version_c = Core.Tool.PopCap.PAM.Version.value({ number: 6n });
					let information = Core.Tool.PopCap.PAM.Manifest.Animation.default();
					Core.Tool.PopCap.PAM.Decode.process_animation(stream, information, version_c);
					let information_json = information.get_json(version_c);
					let information_js = information_json.value;
					if (option.animation.json !== null) {
						CoreX.JSON.write_fs(`${option.animation.directory}/${raw_file}`, information_json);
					}
					if (option.animation.flash !== null) {
						let flash_package = Support.PopCapAnimation.Convert.Flash.From.from(information_js as any);
						Support.PopCapAnimation.Convert.Flash.save_flash_package(flash_package, `${option.animation.directory}/${flash_directory}`);
						Support.PopCapAnimation.Convert.Flash.SourceManager.create_fsh(`${option.animation.directory}/${flash_directory}`, information_js as any);
						Support.PopCapAnimation.Convert.Flash.create_xfl_content_file(`${option.animation.directory}/${flash_directory}`);
					}
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
			if (option.audio !== null && path.endsWith('.wem')) {
				Console.notify('v', `  ${path}`, []);
				try {
					CoreX.Tool.Wwise.EncodedMedia.decode_fs(
						`${resource_directory}/${path}`,
						`${option.audio.directory}/${path.slice(0, -3)}wav`,
						option.audio.tool.ffmpeg_program,
						option.audio.tool.ww2ogg_program,
						option.audio.tool.ww2ogg_code_book,
						audio_temporary_directory,
					);
				} catch (e: any) {
					Console.notify_error(e);
				}
			}
		});
		return;
	}

	/**
	 * 从数据包中直接提取资源与清单
	 * @param data_file 包文件路径
	 * @param manifest_file 包清单文件路径
	 * @param resource_manifest_file 资源清单文件路径
	 * @param resource_directory 资源目录路径
	 * @param option 提取选项
	 */
	export function extract_package(
		data_file: string,
		manifest_file: string,
		resource_manifest_file: string,
		resource_directory: string,
		option: Option,
		version_number: [3n, 4n][number],
		version_additional_texture_information_for_pvz_2_chinese_android: [0n, 1n, 2n][number],
	): void {
		Console.notify('i', los(`解包 ...`), []);
		let package_manifest: Core.Tool.PopCap.RSB.Manifest.JS_N.Package;
		{
			let version_c = Core.Tool.PopCap.RSB.Version.value({ number: version_number, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android });
			let data = CoreX.FileSystem.read_file(data_file);
			let stream = Core.ByteStreamView.look(data.view());
			let manifest = Core.Tool.PopCap.RSB.Manifest.Package.default();
			let description = Core.Tool.PopCap.RSB.Description.PackageOptional.default();
			Core.Tool.PopCap.RSB.Unpack.process_package(
				stream,
				manifest,
				description,
				Core.PathOptional.value(resource_directory),
				Core.PathOptional.value(null),
				version_c,
			);
			data.reset();
			let manifest_json = manifest.get_json(version_c);
			package_manifest = manifest_json.value as any;
			CoreX.JSON.write_fs(manifest_file, manifest_json);
		}
		Console.notify('i', los(`提取资源清单文件 ...`), []);
		let official_resource_manifest: OfficialResourceManifest.Package;
		{
			let group_id = Object.keys(package_manifest.group).filter((e) => (/__MANIFESTGROUP__(.+)?/.test(e)));
			if (group_id.length === 0) {
				throw new Error(`can not found manifest group`);
			}
			if (group_id.length > 1) {
				throw new Error(`too many manifest group`);
			}
			let group = package_manifest.group[group_id[0]];
			if (group.composite) {
				throw new Error(`manifest should not be a composite group`);
			}
			let subgroup_id = Object.keys(group.subgroup);
			if (subgroup_id.length !== 1) {
				throw new Error(`manifest subgroup must has one only subgroup`);
			}
			if (subgroup_id[0] !== group_id[0]) {
				throw new Error(`manifest subgroup id must equal group id`);
			}
			let subgroup = group.subgroup[subgroup_id[0]];
			let resource_path_list = Object.keys(subgroup.resource);
			if (resource_path_list.length !== 1) {
				throw new Error(`manifest subgroup must has one only resource`);
			}
			let resource_path = resource_path_list[0];
			let resource = subgroup.resource[resource_path];
			if (/properties\/resources(_.+)?\.rton/.test(resource_path)) {
				throw new Error(`manifest resource path invalid`);
			}
			let rton = CoreX.FileSystem.read_file(resource_directory + '/' + resource_path);
			let rton_stream = Core.ByteStreamView.look(rton.view());
			let json = Core.JSON.Value.default<OfficialResourceManifest.Package>();
			Core.Tool.PopCap.RTON.Decode.process_whole(
				rton_stream,
				json,
				Core.Tool.PopCap.RTON.Version.value({ number: 1n }),
			);
			official_resource_manifest = json.value;
		}
		Console.notify('i', los(`解析资源清单 ...`), []);
		let resource_manifest = ResourceManifest.Convert.from_official(official_resource_manifest);
		CoreX.JSON.write_fs(resource_manifest_file, Core.JSON.Value.value(resource_manifest));
		extract(
			package_manifest,
			resource_manifest,
			resource_directory,
			option,
		);
		return;
	}

	// ------------------------------------------------

}