namespace Twinning.Script.Support.PvZ2.ResourceConvert {

	// ------------------------------------------------

	export type PTXFormatMap = Array<{
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
			format: PTXFormatMap;
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
		},
	};

	export function convert(
		resource_directory: string,
		package_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package,
		resource_manifest: RegularResourceManifest.Package,
		option: Option,
	): void {
		let find_item_by_id_ignore_case = <T extends { id: string }>(list: Array<T>, value: string): null | T => {
			let value_lower = value.toLowerCase();
			for (let item of list) {
				let item_value = item.id;
				if (item_value.toLowerCase() === value_lower) {
					return item;
				}
			}
			return null;
		};
		let find_item_by_path_ignore_case = <T extends RegularResourceManifest.Resource>(list: Array<T>, value: string): null | T => {
			let value_lower = value.toLowerCase();
			for (let item of list) {
				if (item.additional.type === 'dummy') {
					continue;
				}
				let item_value = item.additional.value.path;
				if (item_value.toLowerCase() === value_lower) {
					return item;
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
			for (let package_group of package_definition.group) {
				group_progress.increase();
				if (show_group_progress) {
					Console.information(`${group_progress} - ${package_group.id}`, []);
				}
				if (/__MANIFESTGROUP__(.+)?/.test(package_group.id)) {
					continue;
				}
				let group = find_item_by_id_ignore_case(resource_manifest.group, package_group.id);
				if (group === null) {
					Console.warning(`group not found in resource manifest : ${package_group.id}`, []);
					continue;
				}
				for (let package_subgroup of package_group.subgroup) {
					let subgroup = find_item_by_id_ignore_case(group.subgroup, package_subgroup.id);
					if (subgroup === null) {
						Console.warning(`subgroup not found in resource manifest : ${package_subgroup.id}`, []);
						continue;
					}
					for (let package_resource of package_subgroup.resource) {
						let resource_path = package_resource.path.toLowerCase();
						if (resource_path.endsWith('.ptx')) {
							resource_path = resource_path.slice(0, -4);
						}
						let resource = find_item_by_path_ignore_case(subgroup.resource, resource_path);
						if (resource === null) {
							Console.warning(`resource not found in resource manifest : ${resource_path}`, []);
							continue;
						}
						worker(
							[group.id, group, package_group],
							[subgroup.id, subgroup, package_subgroup],
							[resource.id, resource, package_resource],
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
				assert_test(resource[1].additional.type !== 'dummy');
				resource_path_list.push(`${resource[1].additional.value.path}${(resource[1].additional.type === 'texture' ? '.ptx' : '')}`);
			});
			let rename_tree = (
				parent: string,
				tree: PathUtility.Tree,
			) => {
				for (let name in tree) {
					try {
						PathUtility.rename_secure(`${parent}/${name.toUpperCase()}`, `${parent}/${name}`);
					}
					catch (e) {
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
		iterate_resource(true)((group, subgroup, resource) => {
			assert_test(resource[1].additional.type !== 'dummy');
			let path = resource[1].additional.value.path;
			if (option.rton !== null && path.endsWith('.rton')) {
				Console.verbosity(`  ${path}`, []);
				try {
					KernelX.Tool.PopCap.ReflectionObjectNotation.decode_cipher_fs(
						`${resource_directory}/${path}`,
						`${option.rton.directory}/${path.slice(0, -4)}json`,
						option.rton.version,
						option.rton.crypt === null ? null : option.rton.crypt.key,
					);
				}
				catch (e) {
					Console.error_of(e);
				}
			}
			if (option.ptx !== null && resource[1].additional.type === 'texture') {
				Console.verbosity(`  ${path}`, []);
				try {
					assert_test(resource[2].additional.type === 'texture', `invalid image resource`);
					let atlas_image_additional = resource[1].additional.value;
					let texture_additional_source = resource[2].additional.value;
					let size = atlas_image_additional.size;
					let actual_size = texture_additional_source.size;
					let format = option.ptx.format.find((value) => (value.index === texture_additional_source.format))?.format;
					assert_test(format !== undefined, `unknown texture format : ${texture_additional_source.format}`);
					Console.verbosity(`    size : [ ${make_prefix_padded_string(size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(size[1].toString(), ' ', 4)} ] of [ ${make_prefix_padded_string(actual_size[0].toString(), ' ', 4)}, ${make_prefix_padded_string(actual_size[1].toString(), ' ', 4)} ] , format : ${format}`, []);
					let data = KernelX.Storage.read_file(`${resource_directory}/${path}.ptx`);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(actual_size));
					let image_view = image.view();
					Support.PopCap.Texture.Encoding.decode(data_stream, image_view, format);
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
							sprite: atlas_image_additional.sprite.map((value) => ({
								name: value.path,
								position: value.position,
								size: value.size,
							})),
						}, image_view, option.ptx.directory);
					}
				}
				catch (e) {
					Console.error_of(e);
				}
			}
			if (option.pam !== null && path.endsWith('.pam')) {
				Console.verbosity(`  ${path}`, []);
				try {
					let data = KernelX.Storage.read_file(`${resource_directory}/${path}`);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let version_c = Kernel.Tool.PopCap.Animation.Version.value(option.pam.version);
					let definition = Kernel.Tool.PopCap.Animation.Definition.Animation.default();
					Kernel.Tool.PopCap.Animation.Decode.process(data_stream, definition, version_c);
					let definition_json = definition.get_json(version_c);
					let definition_js = definition_json.value;
					if (option.pam.json !== null) {
						KernelX.JSON.write_fs(`${option.pam.directory}/${path}.json`, definition_json);
					}
					if (option.pam.flash !== null) {
						let flash_package = Support.PopCap.Animation.Convert.Flash.From.from(definition_js, option.pam.version);
						Support.PopCap.Animation.Convert.Flash.save_flash_package(`${option.pam.directory}/${path}.xfl`, flash_package);
						Support.PopCap.Animation.Convert.Flash.SourceManager.create_fsh(`${option.pam.directory}/${path}.xfl`, definition_js);
						Support.PopCap.Animation.Convert.Flash.create_xfl_content_file(`${option.pam.directory}/${path}.xfl`);
					}
				}
				catch (e) {
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
				}
				catch (e) {
					Console.error_of(e);
				}
			}
			if (option.wem !== null && path.endsWith('.wem')) {
				Console.verbosity(`  ${path}`, []);
				try {
					Support.Wwise.Media.Decode.decode_fs(
						`${resource_directory}/${path}`,
						`${option.wem.directory}/${path.slice(0, -3)}wav`,
					);
				}
				catch (e) {
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
			let group_list = package_definition.group.filter((value) => (/^__MANIFESTGROUP__(.+)?$/i.test(value.id)));
			assert_test(group_list.length === 1, `package must contain MANIFEST group`);
			let group = group_list[0];
			assert_test(group.subgroup.length === 1, `MANIFEST group must contain one subgroup`);
			let subgroup = group.subgroup[0];
			assert_test(subgroup.id === group.id, `MANIFEST subgroup id must equal group id`);
			let resource_list = subgroup.resource.filter((value) => (/^properties\/resources(.+)?\.(rton|newton)$/i.test(value.path)));
			assert_test(resource_list.length !== 0, `MANIFEST subgroup must contains manifest file`);
			let resource_path_list = resource_list.map((value) => (value.path));
			let resource_path: string;
			if (resource_list.length === 1) {
				Console.information(los('support.pvz2.resource_convert:resource_manifest_found_single'), [resource_path_list[0]]);
				resource_path = resource_path_list[0];
			}
			else {
				Console.information(los('support.pvz2.resource_convert:resource_manifest_found_multi'), []);
				resource_path = Console.enumeration(Console.option_string(resource_path_list), null, null);
			}
			Console.information(los('support.pvz2.resource_convert:parse_resource_manifest'), []);
			if (resource_path.toLowerCase().endsWith('rton')) {
				let data = KernelX.Storage.read_file(`${resource_directory}/${resource_path}`);
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
				let data = KernelX.Storage.read_file(`${resource_directory}/${resource_path}`);
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