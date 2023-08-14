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
		recase_path: Executor.Argument<boolean, false>;
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
		wem_tool_ffmpeg_program_file: Executor.Argument<string, false>;
		wem_tool_ww2ogg_program_file: Executor.Argument<string, false>;
		wem_tool_ww2ogg_code_book_file: Executor.Argument<string, false>;
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
					option_recase_path: Executor.Argument<boolean, false>;
					option_rton: Executor.Argument<boolean, false>;
					option_rton_version_number: Executor.Argument<bigint, false>;
					option_rton_version_native_string_encoding_use_utf8: Executor.Argument<boolean, false>;
					option_rton_crypt: Executor.Argument<boolean, false>;
					option_rton_crypt_key: Executor.Argument<string, false>;
					option_ptx: Executor.Argument<boolean, false>;
					option_ptx_texture_format_map_name: Executor.Argument<string, false>;
					option_ptx_atlas: Executor.Argument<boolean, false>;
					option_ptx_atlas_resize: Executor.Argument<boolean, false>;
					option_ptx_sprite: Executor.Argument<boolean, false>;
					option_pam: Executor.Argument<boolean, false>;
					option_pam_version_number: Executor.Argument<bigint, false>;
					option_pam_json: Executor.Argument<boolean, false>;
					option_pam_flash: Executor.Argument<boolean, false>;
					option_bnk: Executor.Argument<boolean, false>;
					option_bnk_version_number: Executor.Argument<bigint, false>;
					option_wem: Executor.Argument<boolean, false>;
					option_wem_tool_ffmpeg_program_file: Executor.Argument<string, false>;
					option_wem_tool_ww2ogg_program_file: Executor.Argument<string, false>;
					option_wem_tool_ww2ogg_code_book_file: Executor.Argument<string, false>;
				}) {
					let bundle_directory: string;
					let version_number: bigint;
					let version_extended_texture_information_for_pvz2_cn: bigint;
					let option_recase_path: boolean;
					let option_rton: boolean;
					let option_rton_version_number: bigint;
					let option_rton_version_native_string_encoding_use_utf8: boolean;
					let option_rton_crypt: boolean;
					let option_rton_crypt_key: string;
					let option_ptx: boolean;
					let option_ptx_texture_format_map_name: string;
					let option_ptx_atlas: boolean;
					let option_ptx_atlas_resize: boolean;
					let option_ptx_sprite: boolean;
					let option_pam: boolean;
					let option_pam_version_number: bigint;
					let option_pam_json: boolean;
					let option_pam_flash: boolean;
					let option_bnk: boolean;
					let option_bnk_version_number: bigint;
					let option_wem: boolean;
					let option_wem_tool_ffmpeg_program_file: string;
					let option_wem_tool_ww2ogg_program_file: string;
					let option_wem_tool_ww2ogg_code_book_file: string;
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
						option_recase_path = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_recase_path'),
							a.option_recase_path,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						option_rton = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_rton'),
							a.option_rton,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						option_rton_version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_rton_version_number'),
							a.option_rton_version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE), null, null, initial)),
							(option_rton) ? undefined : 0n,
						);
						option_rton_version_native_string_encoding_use_utf8 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_rton_version_native_string_encoding_use_utf8'),
							a.option_rton_version_native_string_encoding_use_utf8,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_rton) ? undefined : false,
						);
						option_rton_crypt = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_rton_crypt'),
							a.option_rton_crypt,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_rton) ? undefined : false,
						);
						option_rton_crypt_key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_rton_crypt_key'),
							a.option_rton_crypt_key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, null, initial)),
							(option_rton && option_rton_crypt) ? undefined : '',
						);
						option_ptx = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_ptx'),
							a.option_ptx,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						option_ptx_texture_format_map_name = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_ptx_texture_format_map_name'),
							a.option_ptx_texture_format_map_name,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(Object.keys(configuration.resource_convert_option.ptx_texture_format_map_list)), null, null, initial)),
							(option_ptx) ? undefined : '',
						);
						option_ptx_atlas = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_ptx_atlas'),
							a.option_ptx_atlas,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_ptx) ? undefined : false,
						);
						option_ptx_atlas_resize = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_ptx_atlas_resize'),
							a.option_ptx_atlas_resize,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_ptx && option_ptx_atlas) ? undefined : false,
						);
						option_ptx_sprite = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_ptx_sprite'),
							a.option_ptx_sprite,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_ptx) ? undefined : false,
						);
						option_pam = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_pam'),
							a.option_pam,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						option_pam_version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_pam_version_number'),
							a.option_pam_version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.Animation.VersionNumberE), null, null, initial)),
							(option_pam) ? undefined : 0n,
						);
						option_pam_json = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_pam_json'),
							a.option_pam_json,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_pam) ? undefined : false,
						);
						option_pam_flash = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_pam_flash'),
							a.option_pam_flash,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
							(option_pam) ? undefined : false,
						);
						option_bnk = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_bnk'),
							a.option_bnk,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						option_bnk_version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_bnk_version_number'),
							a.option_bnk_version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.Wwise.SoundBank.VersionNumberE), null, null, initial)),
							(option_bnk) ? undefined : 0n,
						);
						option_wem = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_wem'),
							a.option_wem,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						option_wem_tool_ffmpeg_program_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_wem_tool_ffmpeg_program_file'),
							a.option_wem_tool_ffmpeg_program_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
							(option_wem) ? undefined : '',
						);
						option_wem_tool_ww2ogg_program_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_wem_tool_ww2ogg_program_file'),
							a.option_wem_tool_ww2ogg_program_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
							(option_wem) ? undefined : '',
						);
						option_wem_tool_ww2ogg_code_book_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'option_wem_tool_ww2ogg_code_book_file'),
							a.option_wem_tool_ww2ogg_code_book_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
							(option_wem) ? undefined : '',
						);
					}
					let convert_directory = `${bundle_directory}/convert`;
					let option: Support.PvZ2.ResourceStreamBundle.ResourceConvert.Option = {
						recase_path: option_recase_path,
						rton: !option_rton ? null : {
							directory: convert_directory,
							version: {
								number: option_rton_version_number as any,
								native_string_encoding_use_utf8: option_rton_version_native_string_encoding_use_utf8,
							},
							crypt: !option_rton_crypt ? null : {
								key: option_rton_crypt_key,
							},
						},
						ptx: !option_ptx ? null : {
							directory: convert_directory,
							texture_format_map: configuration.resource_convert_option.ptx_texture_format_map_list[option_ptx_texture_format_map_name],
							atlas: !option_ptx_atlas ? null : {
								resize: option_ptx_atlas_resize,
							},
							sprite: !option_ptx_sprite ? null : {
							},
						},
						pam: !option_pam ? null : {
							directory: convert_directory,
							version: {
								number: option_pam_version_number as any,
							},
							json: !option_pam_json ? null : {
							},
							flash: !option_pam_flash ? null : {
							},
						},
						bnk: !option_bnk ? null : {
							directory: convert_directory,
							version: {
								number: option_bnk_version_number as any,
							},
						},
						wem: !option_wem ? null : {
							directory: convert_directory,
							tool: {
								ffmpeg_program: option_wem_tool_ffmpeg_program_file,
								ww2ogg_program: option_wem_tool_ww2ogg_program_file,
								ww2ogg_code_book: option_wem_tool_ww2ogg_code_book_file,
							},
						},
					};
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
					option_recase_path: configuration.resource_convert_option.recase_path,
					option_rton: configuration.resource_convert_option.rton,
					option_rton_version_number: configuration.resource_convert_option.rton_version_number,
					option_rton_version_native_string_encoding_use_utf8: configuration.resource_convert_option.rton_version_native_string_encoding_use_utf8,
					option_rton_crypt: configuration.resource_convert_option.rton_crypt,
					option_rton_crypt_key: configuration.resource_convert_option.rton_crypt_key,
					option_ptx: configuration.resource_convert_option.ptx,
					option_ptx_texture_format_map_name: configuration.resource_convert_option.ptx_texture_format_map_name,
					option_ptx_atlas: configuration.resource_convert_option.ptx_atlas,
					option_ptx_atlas_resize: configuration.resource_convert_option.ptx_atlas_resize,
					option_ptx_sprite: configuration.resource_convert_option.ptx_sprite,
					option_pam: configuration.resource_convert_option.pam,
					option_pam_version_number: configuration.resource_convert_option.pam_version_number,
					option_pam_json: configuration.resource_convert_option.pam_json,
					option_pam_flash: configuration.resource_convert_option.pam_flash,
					option_bnk: configuration.resource_convert_option.bnk,
					option_bnk_version_number: configuration.resource_convert_option.bnk_version_number,
					option_wem: configuration.resource_convert_option.wem,
					option_wem_tool_ffmpeg_program_file: configuration.resource_convert_option.wem_tool_ffmpeg_program_file,
					option_wem_tool_ww2ogg_program_file: configuration.resource_convert_option.wem_tool_ww2ogg_program_file,
					option_wem_tool_ww2ogg_code_book_file: configuration.resource_convert_option.wem_tool_ww2ogg_code_book_file,
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