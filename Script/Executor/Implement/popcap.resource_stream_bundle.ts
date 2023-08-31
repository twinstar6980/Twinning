namespace TwinStar.Script.Executor.Implement.popcap.resource_stream_bundle {

	// ------------------------------------------------

	const ResourceStreamBundleLayoutModeX = [
		'group',
		'subgroup',
		'resource',
	] as const;

	type ResourceStreamBundleLayoutMode = typeof ResourceStreamBundleLayoutModeX[number];

	const ResourceStreamBundleLayoutModeE = ResourceStreamBundleLayoutModeX as unknown as ResourceStreamBundleLayoutMode[];

	function make_resource_stream_bundle_package_relative_path(
		layout_mode: ResourceStreamBundleLayoutMode,
	) {
		let result: {
			resource_directory: TypicalArgumentExpression<string>;
			packet_file: TypicalArgumentExpression<string>;
			packet_definition_file: TypicalArgumentExpression<string>;
		};
		switch (layout_mode) {
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
		recase_path: TypicalArgumentExpression<boolean>;
		rton: TypicalArgumentExpression<boolean>;
		rton_version_number: TypicalArgumentExpression<bigint>;
		rton_version_native_string_encoding_use_utf8: TypicalArgumentExpression<boolean>;
		rton_crypt: TypicalArgumentExpression<boolean>;
		rton_crypt_key: TypicalArgumentExpression<string>;
		ptx: TypicalArgumentExpression<boolean>;
		ptx_texture_format_map_list: Record<string, Support.PvZ2.ResourceStreamBundle.ResourceConvert.TextureFormatMap>;
		ptx_texture_format_map_name: TypicalArgumentExpression<string>;
		ptx_atlas: TypicalArgumentExpression<boolean>;
		ptx_atlas_resize: TypicalArgumentExpression<boolean>;
		ptx_sprite: TypicalArgumentExpression<boolean>;
		pam: TypicalArgumentExpression<boolean>;
		pam_version_number: TypicalArgumentExpression<bigint>;
		pam_json: TypicalArgumentExpression<boolean>;
		pam_flash: TypicalArgumentExpression<boolean>;
		bnk: TypicalArgumentExpression<boolean>;
		bnk_version_number: TypicalArgumentExpression<bigint>;
		wem: TypicalArgumentExpression<boolean>;
		wem_tool_ffmpeg_program_file: TypicalArgumentExpression<string>;
		wem_tool_ww2ogg_program_file: TypicalArgumentExpression<string>;
		wem_tool_ww2ogg_code_book_file: TypicalArgumentExpression<string>;
	};

	// ------------------------------------------------

	// pack *
	// unpack *
	// resource_convert *
	// unpack_lenient *

	export type Configuration = {
		version_number: TypicalArgumentExpression<bigint>;
		version_extended_texture_information_for_pvz2_cn: TypicalArgumentExpression<bigint>;
		layout_mode: TypicalArgumentExpression<string>;
		pack_buffer_size: TypicalArgumentExpression<string>;
		resource_convert_option: ResourceConvertOption;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.resource_stream_bundle', [
			typical_method({
				id: 'pack',
				filter: ['directory', /(\.rsb\.bundle)$/i],
				argument: [
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory.replace(/(\.rsb\.bundle)?$/i, '.rsb')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_integer({
						id: 'version_extended_texture_information_for_pvz2_cn',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE,
						checker: null,
						automatic: null,
						condition: (argument: { version_number: bigint; }) => (argument.version_number === 4n ? null : 0n),
						default: configuration.version_extended_texture_information_for_pvz2_cn,
					}),
					typical_argument_string({
						id: 'layout_mode',
						option: ResourceStreamBundleLayoutModeE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.layout_mode,
					}),
					typical_argument_confirmation({
						id: 'input_packet',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_confirmation({
						id: 'output_new_packet',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.pack_buffer_size,
					}),
				],
				worker: ({ bundle_directory, data_file, version_number, version_extended_texture_information_for_pvz2_cn, layout_mode, input_packet, output_new_packet, buffer_size }) => {
					let relative_path = make_resource_stream_bundle_package_relative_path(layout_mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.PopCap.ResourceStreamBundle.pack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, new_packet_file, { number: version_number as any, extended_texture_information_for_pvz2_cn: version_extended_texture_information_for_pvz2_cn as any }, buffer_size);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory + '.pack'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rsb\.bundle)?$/i, '.rsb')),
					}),
				],
				batch_worker: ({ bundle_directory, data_file, version_number, version_extended_texture_information_for_pvz2_cn, layout_mode, input_packet, output_new_packet, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					let relative_path = make_resource_stream_bundle_package_relative_path(layout_mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.PopCap.ResourceStreamBundle.pack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, new_packet_file, { number: version_number as any, extended_texture_information_for_pvz2_cn: version_extended_texture_information_for_pvz2_cn as any }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'unpack',
				filter: ['file', /(\.rsb)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rsb)?$/i, '.rsb.bundle')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_integer({
						id: 'version_extended_texture_information_for_pvz2_cn',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE,
						checker: null,
						automatic: null,
						condition: (argument: { version_number: bigint; }) => (argument.version_number === 4n ? null : 0n),
						default: configuration.version_extended_texture_information_for_pvz2_cn,
					}),
					typical_argument_string({
						id: 'layout_mode',
						option: ResourceStreamBundleLayoutModeE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.layout_mode,
					}),
					typical_argument_confirmation({
						id: 'output_resource',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_confirmation({
						id: 'output_packet',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ data_file, bundle_directory, version_number, version_extended_texture_information_for_pvz2_cn, layout_mode, output_resource, output_packet }) => {
					let relative_path = make_resource_stream_bundle_package_relative_path(layout_mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = !output_resource ? null : `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !output_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.PopCap.ResourceStreamBundle.unpack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, { number: version_number as any, extended_texture_information_for_pvz2_cn: version_extended_texture_information_for_pvz2_cn as any });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.unpack'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rsb)?$/i, '.rsb.bundle')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'resource_convert',
				filter: ['directory', /(\.rsb\.bundle)$/i],
				argument: [
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_confirmation({
						id: 'option_recase_path',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.resource_convert_option.recase_path,
					}),
					typical_argument_confirmation({
						id: 'option_rton',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.resource_convert_option.rton,
					}),
					typical_argument_integer({
						id: 'option_rton_version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; }) => (argument.option_rton ? null : 0n),
						default: configuration.resource_convert_option.rton_version_number,
					}),
					typical_argument_confirmation({
						id: 'option_rton_version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; }) => (argument.option_rton ? null : false),
						default: configuration.resource_convert_option.rton_version_native_string_encoding_use_utf8,
					}),
					typical_argument_confirmation({
						id: 'option_rton_crypt',
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; }) => (argument.option_rton ? null : false),
						default: configuration.resource_convert_option.rton_crypt,
					}),
					typical_argument_string({
						id: 'option_rton_crypt_key',
						option: null,
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; option_rton_crypt: boolean; }) => (argument.option_rton && argument.option_rton_crypt ? null : ''),
						default: configuration.resource_convert_option.rton_crypt_key,
					}),
					typical_argument_confirmation({
						id: 'option_ptx',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.resource_convert_option.ptx,
					}),
					typical_argument_string({
						id: 'option_ptx_texture_format_map_name',
						option: Object.keys(configuration.resource_convert_option.ptx_texture_format_map_list),
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; }) => (argument.option_ptx ? null : ''),
						default: configuration.resource_convert_option.ptx_texture_format_map_name,
					}),
					typical_argument_confirmation({
						id: 'option_ptx_atlas',
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; }) => (argument.option_ptx ? null : false),
						default: configuration.resource_convert_option.ptx_atlas,
					}),
					typical_argument_confirmation({
						id: 'option_ptx_atlas_resize',
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; option_ptx_atlas: boolean; }) => (argument.option_ptx && argument.option_ptx_atlas ? null : false),
						default: configuration.resource_convert_option.ptx_atlas_resize,
					}),
					typical_argument_confirmation({
						id: 'option_ptx_sprite',
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; }) => (argument.option_ptx ? null : false),
						default: configuration.resource_convert_option.ptx_sprite,
					}),
					typical_argument_confirmation({
						id: 'option_pam',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.resource_convert_option.pam,
					}),
					typical_argument_integer({
						id: 'option_pam_version_number',
						option: KernelX.Tool.PopCap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { option_pam: boolean; }) => (argument.option_pam ? null : 0n),
						default: configuration.resource_convert_option.pam_version_number,
					}),
					typical_argument_confirmation({
						id: 'option_pam_json',
						checker: null,
						automatic: null,
						condition: (argument: { option_pam: boolean; }) => (argument.option_pam ? null : false),
						default: configuration.resource_convert_option.pam_json,
					}),
					typical_argument_confirmation({
						id: 'option_pam_flash',
						checker: null,
						automatic: null,
						condition: (argument: { option_pam: boolean; }) => (argument.option_pam ? null : false),
						default: configuration.resource_convert_option.pam_flash,
					}),
					typical_argument_confirmation({
						id: 'option_bnk',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.resource_convert_option.bnk,
					}),
					typical_argument_integer({
						id: 'option_bnk_version_number',
						option: KernelX.Tool.Wwise.SoundBank.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { option_bnk: boolean; }) => (argument.option_bnk ? null : 0n),
						default: configuration.resource_convert_option.bnk_version_number,
					}),
					typical_argument_confirmation({
						id: 'option_wem',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.resource_convert_option.wem,
					}),
					typical_argument_path({
						id: 'option_wem_tool_ffmpeg_program_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: (argument: { option_wem: boolean; }) => (argument.option_wem ? null : ''),
						default: configuration.resource_convert_option.wem_tool_ffmpeg_program_file,
					}),
					typical_argument_path({
						id: 'option_wem_tool_ww2ogg_program_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: (argument: { option_wem: boolean; }) => (argument.option_wem ? null : ''),
						default: configuration.resource_convert_option.wem_tool_ww2ogg_program_file,
					}),
					typical_argument_path({
						id: 'option_wem_tool_ww2ogg_code_book_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: (argument: { option_wem: boolean; }) => (argument.option_wem ? null : ''),
						default: configuration.resource_convert_option.wem_tool_ww2ogg_code_book_file,
					}),
				],
				worker: ({
					bundle_directory,
					option_recase_path,
					option_rton,
					option_rton_version_number,
					option_rton_version_native_string_encoding_use_utf8,
					option_rton_crypt,
					option_rton_crypt_key,
					option_ptx,
					option_ptx_texture_format_map_name,
					option_ptx_atlas,
					option_ptx_atlas_resize,
					option_ptx_sprite,
					option_pam,
					option_pam_version_number,
					option_pam_json,
					option_pam_flash,
					option_bnk,
					option_bnk_version_number,
					option_wem,
					option_wem_tool_ffmpeg_program_file,
					option_wem_tool_ww2ogg_program_file,
					option_wem_tool_ww2ogg_code_book_file,
				}) => {
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
					);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'unpack_lenient',
				filter: ['file', /(\.rsb)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rsb)?$/i, '.rsb.bundle')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: ({ data_file, bundle_directory }) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					Support.PopCap.ResourceStreamBundle.UnpackLenient.process_fs(data_file, definition_file, manifest_file, resource_directory);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.unpack_lenient'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rsb)?$/i, '.rsb.bundle')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.resource_stream_bundle.injector;