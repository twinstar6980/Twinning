namespace TwinStar.Script.Entry.method.popcap.resource_stream_bundle {

	// ------------------------------------------------

	const ResourceStreamBundlePackAndUnpackModeX = [
		'group',
		'subgroup',
		'resource',
	] as const;

	type ResourceStreamBundlePackAndUnpackMode = typeof ResourceStreamBundlePackAndUnpackModeX[number];

	const ResourceStreamBundlePackAndUnpackModeE = ResourceStreamBundlePackAndUnpackModeX as unknown as ResourceStreamBundlePackAndUnpackMode[];

	function make_resource_stream_bundle_package_relative_path(
		mode: ResourceStreamBundlePackAndUnpackMode,
	) {
		let result: {
			resource_directory: Executor.Argument<string, false>;
			packet_file: Executor.Argument<string, false>;
			packet_definition_file: Executor.Argument<string, false>;
		};
		switch (mode) {
			case 'group': {
				result = {
					resource_directory: 'group/{0}/{1}/resource',
					packet_file: 'group/{0}/{1}/packet.rsg',
					packet_definition_file: 'group/{0}/{1}/definition.json',
				};
				break;
			}
			case 'subgroup': {
				result = {
					resource_directory: 'subgroup/{1}/resource',
					packet_file: 'subgroup/{1}/packet.rsg',
					packet_definition_file: 'subgroup/{1}/definition.json',
				};
				break;
			}
			case 'resource': {
				result = {
					resource_directory: 'resource',
					packet_file: 'packet/{1}.rsg',
					packet_definition_file: 'packet/{1}.json',
				};
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

	type ResourceConvertOption = {
		rton: Executor.Argument<boolean, false>;
		rton_version_number: Executor.Argument<bigint, false>;
		rton_version_native_string_encoding_use_utf8: Executor.Argument<boolean, false>;
		rton_crypt: Executor.Argument<boolean, false>;
		rton_crypt_key: Executor.Argument<string, false>;
		ptx: Executor.Argument<boolean, false>;
		ptx_texture_format_map_list: Record<string, Support.PvZ2.ResourceStreamBundle.ResourceConvert.TextureFormatMap>;
		ptx_texture_format_map_name: Executor.Argument<string, false>;
		ptx_atlas: Executor.Argument<boolean, false>;
		ptx_atlas_resize: Executor.Argument<boolean, false>;
		ptx_sprite: Executor.Argument<boolean, false>;
		pam: Executor.Argument<boolean, false>;
		pam_version_number: Executor.Argument<bigint, false>;
		pam_json: Executor.Argument<boolean, false>;
		pam_flash: Executor.Argument<boolean, false>;
		bnk: Executor.Argument<boolean, false>;
		bnk_version_number: Executor.Argument<bigint, false>;
		wem: Executor.Argument<boolean, false>;
		wem_tool: {
			ffmpeg_program: string;
			ww2ogg_program: string;
			ww2ogg_code_book: string;
		};
	};

	// ------------------------------------------------

	// pack
	// unpack
	// resource_convert
	// unpack_lenient

	type Configuration = {
		mode: Executor.Argument<string, false>;
		version_number: Executor.Argument<bigint, false>;
		version_extended_texture_information_for_pvz2_cn: Executor.Argument<bigint, false>;
		pack_buffer_size: Executor.Argument<string, false>;
		resource_convert_option: ResourceConvertOption;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.resource_stream_bundle.pack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					bundle_directory: Executor.Argument<string, false>;
					data_file: Executor.Argument<string, true>;
					mode: Executor.Argument<string, false>;
					version_number: Executor.Argument<bigint, false>;
					version_extended_texture_information_for_pvz2_cn: Executor.Argument<bigint, false>;
					buffer_size: Executor.Argument<string, false>;
					input_packet: Executor.Argument<boolean, false>;
					output_new_packet: Executor.Argument<boolean, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let mode: string;
					let version_number: bigint;
					let version_extended_texture_information_for_pvz2_cn: bigint;
					let buffer_size: bigint;
					let input_packet: boolean;
					let output_new_packet: boolean;
					{
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.rsb)(\.bundle))?$/i, '.rsb')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option([
								[ResourceStreamBundlePackAndUnpackModeE[0], '1', los(`群组：按群+子群树形结构导入，资源与子包均导入自group/<群名>/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[1], '2', los(`子群：按子群树形结构导入，资源与子包均导入自subgroup/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[2], '3', los(`资源：所有资源导入自resource目录，所有子包导入自packet目录`)],
							], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE), null, null, initial)),
						);
						version_extended_texture_information_for_pvz2_cn = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_extended_texture_information_for_pvz2_cn'),
							a.version_extended_texture_information_for_pvz2_cn,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
						input_packet = Executor.request_argument(
							Executor.query_argument_name(this.id, 'input_packet'),
							a.input_packet,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						output_new_packet = Executor.request_argument(
							Executor.query_argument_name(this.id, 'output_new_packet'),
							a.output_new_packet,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					let relative_path = make_resource_stream_bundle_package_relative_path(mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.PopCap.ResourceStreamBundle.pack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, new_packet_file, { number: version_number as any, extended_texture_information_for_pvz2_cn: version_extended_texture_information_for_pvz2_cn as any }, buffer_size);
					return [`${data_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					bundle_directory: '?input',
					data_file: '?default',
					mode: configuration.mode,
					version_number: configuration.version_number,
					version_extended_texture_information_for_pvz2_cn: configuration.version_extended_texture_information_for_pvz2_cn,
					buffer_size: configuration.pack_buffer_size,
					input_packet: '?input',
					output_new_packet: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsb)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_bundle.unpack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.Argument<string, false>;
					bundle_directory: Executor.Argument<string, true>;
					mode: Executor.Argument<string, false>;
					version_number: Executor.Argument<bigint, false>;
					version_extended_texture_information_for_pvz2_cn: Executor.Argument<bigint, false>;
					output_resource: Executor.Argument<boolean, false>;
					output_packet: Executor.Argument<boolean, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let mode: string;
					let version_number: bigint;
					let version_extended_texture_information_for_pvz2_cn: bigint;
					let output_resource: boolean;
					let output_packet: boolean;
					{
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.rsb))?$/i, '.rsb.bundle')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option([
								[ResourceStreamBundlePackAndUnpackModeE[0], '1', los(`群组：按群+子群树形结构导入，资源与子包均导入自group/<群名>/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[1], '2', los(`子群：按子群树形结构导入，资源与子包均导入自subgroup/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[2], '3', los(`资源：所有资源导入自resource目录，所有子包导入自packet目录`)],
							], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE), null, null, initial)),
						);
						version_extended_texture_information_for_pvz2_cn = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_extended_texture_information_for_pvz2_cn'),
							a.version_extended_texture_information_for_pvz2_cn,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE), null, null, initial)),
						);
						output_resource = Executor.request_argument(
							Executor.query_argument_name(this.id, 'output_resource'),
							a.output_resource,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						output_packet = Executor.request_argument(
							Executor.query_argument_name(this.id, 'output_packet'),
							a.output_packet,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					let relative_path = make_resource_stream_bundle_package_relative_path(mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = !output_resource ? null : `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !output_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.PopCap.ResourceStreamBundle.unpack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, { number: version_number as any, extended_texture_information_for_pvz2_cn: version_extended_texture_information_for_pvz2_cn as any });
					return [`${bundle_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file: '?input',
					bundle_directory: '?default',
					mode: configuration.mode,
					version_number: configuration.version_number,
					version_extended_texture_information_for_pvz2_cn: configuration.version_extended_texture_information_for_pvz2_cn,
					output_resource: '?input',
					output_packet: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_bundle.resource_convert',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					bundle_directory: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
					version_extended_texture_information_for_pvz2_cn: Executor.Argument<bigint, false>;
					option: ResourceConvertOption;
				}) {
					let bundle_directory: string;
					let version_number: bigint;
					let version_extended_texture_information_for_pvz2_cn: bigint;
					let option: Support.PvZ2.ResourceStreamBundle.ResourceConvert.Option = {
						rton: null,
						ptx: null,
						pam: null,
						bnk: null,
						wem: null,
					};
					{
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE), null, null, initial)),
						);
						version_extended_texture_information_for_pvz2_cn = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_extended_texture_information_for_pvz2_cn'),
							a.version_extended_texture_information_for_pvz2_cn,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE), null, null, initial)),
						);
						let convert_directory = `${bundle_directory}/convert`;
						{
							let rton: boolean;
							rton = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_rton'),
								a.option.rton,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (rton) {
								option.rton = {
									directory: convert_directory,
									version: {
										number: undefined!,
										native_string_encoding_use_utf8: undefined!,
									},
									crypt: null,
								};
								option.rton.version.number = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_rton_version_number'),
									a.option.rton_version_number,
									(value) => (value),
									null,
									(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE), null, null, initial)),
								) as any;
								option.rton.version.native_string_encoding_use_utf8 = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_rton_version_native_string_encoding_use_utf8'),
									a.option.rton_version_native_string_encoding_use_utf8,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								let crypt: boolean;
								crypt = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_rton_crypt'),
									a.option.rton_crypt,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								if (crypt) {
									let key: string;
									key = Executor.request_argument(
										Executor.query_argument_name(this.id, 'option_rton_crypt_key'),
										a.option.rton_crypt_key,
										(value) => (value),
										null,
										(initial) => (Console.string(null, null, initial)),
									);
									option.rton.crypt = {
										key: key,
									};
								}
							}
						}
						{
							let ptx: boolean;
							ptx = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_ptx'),
								a.option.ptx,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (ptx) {
								option.ptx = {
									directory: convert_directory,
									texture_format_map: null!,
									atlas: null,
									sprite: null,
								};
								{
									let map_name_list = Object.keys(a.option.ptx_texture_format_map_list);
									assert_test(map_name_list.length !== 0, `texture format map list is empty`);
									let texture_format_map_name: string;
									texture_format_map_name = Executor.request_argument(
										Executor.query_argument_name(this.id, 'option_ptx_texture_format_map_name'),
										a.option.ptx_texture_format_map_name,
										(value) => (value),
										null,
										(initial) => (Console.option(Console.option_string(map_name_list), null, null, initial)),
									);
									option.ptx.texture_format_map = a.option.ptx_texture_format_map_list[texture_format_map_name];
								}
								let atlas: boolean;
								atlas = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_ptx_atlas'),
									a.option.ptx_atlas,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								let sprite: boolean;
								sprite = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_ptx_sprite'),
									a.option.ptx_sprite,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								if (atlas) {
									let resize: boolean;
									resize = Executor.request_argument(
										Executor.query_argument_name(this.id, 'option_ptx_atlas_resize'),
										a.option.ptx_atlas_resize,
										(value) => (value),
										null,
										(initial) => (Console.confirmation(null, null, initial)),
									);
									option.ptx.atlas = {
										resize: resize,
									};
								}
								if (sprite) {
									option.ptx.sprite = {};
								}
							}
						}
						{
							let pam: boolean;
							pam = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_pam'),
								a.option.pam,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (pam) {
								option.pam = {
									directory: convert_directory,
									version: {
										number: undefined!,
									},
									json: null,
									flash: null,
								};
								option.pam.version.number = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_pam_version_number'),
									a.option.pam_version_number,
									(value) => (value),
									null,
									(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.Animation.VersionNumberE), null, null, initial)),
								) as any;
								let json: boolean;
								json = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_pam_json'),
									a.option.pam_json,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								let flash: boolean;
								flash = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_pam_flash'),
									a.option.pam_flash,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								if (json) {
									option.pam.json = {};
								}
								if (flash) {
									option.pam.flash = {};
								}
							}
						}
						{
							let bnk: boolean;
							bnk = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_bnk'),
								a.option.bnk,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (bnk) {
								option.bnk = {
									directory: convert_directory,
									version: {
										number: undefined!,
									},
								};
								option.bnk.version.number = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_bnk_version_number'),
									a.option.bnk_version_number,
									(value) => (value),
									null,
									(initial) => (Console.option(Console.option_integer(KernelX.Tool.Wwise.SoundBank.VersionNumberE), null, null, initial)),
								) as any;
							}
						}
						{
							let wem: boolean;
							wem = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_wem'),
								a.option.wem,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (wem) {
								option.wem = {
									directory: convert_directory,
									tool: {
										ffmpeg_program: Home.of(a.option.wem_tool.ffmpeg_program),
										ww2ogg_program: Home.of(a.option.wem_tool.ww2ogg_program),
										ww2ogg_code_book: Home.of(a.option.wem_tool.ww2ogg_code_book),
									},
								};
							}
						}
					}
					Support.PvZ2.ResourceStreamBundle.ResourceConvert.convert_fs(
						`${bundle_directory}/resource`,
						`${bundle_directory}/definition.json`,
						`${bundle_directory}/resource_manifest.json`,
						option,
						{ number: version_number as any, extended_texture_information_for_pvz2_cn: version_extended_texture_information_for_pvz2_cn as any },
					);
					return [`${bundle_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					bundle_directory: '?input',
					version_number: configuration.version_number,
					version_extended_texture_information_for_pvz2_cn: configuration.version_extended_texture_information_for_pvz2_cn,
					option: configuration.resource_convert_option,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsb)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_bundle.unpack_lenient',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.Argument<string, false>;
					bundle_directory: Executor.Argument<string, true>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					{
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.rsb))?$/i, '.rsb.bundle')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					Support.PopCap.ResourceStreamBundle.UnpackLenient.process_fs(data_file, definition_file, manifest_file, resource_directory);
					return [`${bundle_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file: '?input',
					bundle_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.resource_stream_bundle._injector,
});