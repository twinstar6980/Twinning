namespace Twinning.Script.Executor.Implementation.Popcap.ResourceStreamBundle {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.resource_stream_bundle', [
			typical_method({
				identifier: 'pack',
				filter: ['directory', /(\.rsb\.bundle)$/i],
				argument: [
					typical_argument_path({
						identifier: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory.replace(/(\.rsb\.bundle)?$/i, '.rsb')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ResourceStreamBundle.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_extended_texture_information_for_pvz2cn',
						option: KernelX.Tool.Popcap.ResourceStreamBundle.VersionaExtendedTextureInformationForPvz2cnE,
						checker: null,
						automatic: null,
						condition: (argument: { version_number: bigint; }) => ([4n].includes(argument.version_number) ? null : 0n),
					}),
					typical_argument_string({
						identifier: 'layout_mode',
						option: Support.Popcap.ResourceStreamBundle.LayoutModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'input_packet',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'output_new_packet',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory + '.pack'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rsb\.bundle)?$/i, '.rsb')),
					}),
				],
				worker: ({ bundle_directory, data_file, version_number, version_extended_texture_information_for_pvz2cn, layout_mode, input_packet, output_new_packet, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					let relative_path = Support.Popcap.ResourceStreamBundle.make_package_relative_path(layout_mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.Popcap.ResourceStreamBundle.pack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, new_packet_file, { number: version_number as any, extended_texture_information_for_pvz2cn: version_extended_texture_information_for_pvz2cn as any }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'unpack',
				filter: ['file', /(\.rsb)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rsb)?$/i, '.rsb.bundle')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ResourceStreamBundle.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_extended_texture_information_for_pvz2cn',
						option: KernelX.Tool.Popcap.ResourceStreamBundle.VersionaExtendedTextureInformationForPvz2cnE,
						checker: null,
						automatic: null,
						condition: (argument: { version_number: bigint; }) => ([4n].includes(argument.version_number) ? null : 0n),
					}),
					typical_argument_string({
						identifier: 'layout_mode',
						option: Support.Popcap.ResourceStreamBundle.LayoutModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'output_resource',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'output_packet',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.unpack'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rsb)?$/i, '.rsb.bundle')),
					}),
				],
				worker: ({ data_file, bundle_directory, version_number, version_extended_texture_information_for_pvz2cn, layout_mode, output_resource, output_packet }, temporary: {}) => {
					let relative_path = Support.Popcap.ResourceStreamBundle.make_package_relative_path(layout_mode as any);
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = !output_resource ? null : `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !output_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					KernelX.Tool.Popcap.ResourceStreamBundle.unpack_fs(data_file, definition_file, manifest_file, resource_directory, packet_file, { number: version_number as any, extended_texture_information_for_pvz2cn: version_extended_texture_information_for_pvz2cn as any });
					return;
				},
			}),
			typical_method({
				identifier: 'unpack_lenient',
				filter: ['file', /(\.rsb)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rsb)?$/i, '.rsb.bundle')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.unpack_lenient'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rsb)?$/i, '.rsb.bundle')),
					}),
				],
				worker: ({ data_file, bundle_directory }, temporary: {}) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					Support.Popcap.ResourceStreamBundle.UnpackLenient.process_fs(data_file, definition_file, manifest_file, resource_directory);
					return;
				},
			}),
			typical_method({
				identifier: 'resource_convert',
				filter: ['directory', /(\.rsb\.bundle)$/i],
				argument: [
					typical_argument_path({
						identifier: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'option_recase_path',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'option_rton',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'option_rton_version_number',
						option: KernelX.Tool.Popcap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; }) => (argument.option_rton ? null : 0n),
					}),
					typical_argument_boolean({
						identifier: 'option_rton_version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; }) => (argument.option_rton ? null : false),
					}),
					typical_argument_boolean({
						identifier: 'option_rton_crypt',
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; }) => (argument.option_rton ? null : false),
					}),
					typical_argument_string({
						identifier: 'option_rton_crypt_key',
						option: null,
						checker: null,
						automatic: null,
						condition: (argument: { option_rton: boolean; option_rton_crypt: boolean; }) => (argument.option_rton && argument.option_rton_crypt ? null : ''),
					}),
					typical_argument_boolean({
						identifier: 'option_ptx',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'option_ptx_format',
						option: Object.keys(MainScript.g_setting.executor_pvz2_resource_convert_ptx_format_map_list),
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; }) => (argument.option_ptx ? null : ''),
					}),
					typical_argument_boolean({
						identifier: 'option_ptx_atlas',
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; }) => (argument.option_ptx ? null : false),
					}),
					typical_argument_boolean({
						identifier: 'option_ptx_atlas_resize',
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; option_ptx_atlas: boolean; }) => (argument.option_ptx && argument.option_ptx_atlas ? null : false),
					}),
					typical_argument_boolean({
						identifier: 'option_ptx_sprite',
						checker: null,
						automatic: null,
						condition: (argument: { option_ptx: boolean; }) => (argument.option_ptx ? null : false),
					}),
					typical_argument_boolean({
						identifier: 'option_pam',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'option_pam_version_number',
						option: KernelX.Tool.Popcap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { option_pam: boolean; }) => (argument.option_pam ? null : 0n),
					}),
					typical_argument_boolean({
						identifier: 'option_pam_json',
						checker: null,
						automatic: null,
						condition: (argument: { option_pam: boolean; }) => (argument.option_pam ? null : false),
					}),
					typical_argument_boolean({
						identifier: 'option_pam_flash',
						checker: null,
						automatic: null,
						condition: (argument: { option_pam: boolean; }) => (argument.option_pam ? null : false),
					}),
					typical_argument_boolean({
						identifier: 'option_bnk',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'option_bnk_version_number',
						option: KernelX.Tool.Wwise.SoundBank.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { option_bnk: boolean; }) => (argument.option_bnk ? null : 0n),
					}),
					typical_argument_boolean({
						identifier: 'option_wem',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
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
					option_ptx_format,
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
				}) => {
					let convert_directory = `${bundle_directory}/convert`;
					let option: Support.Pvz2.ResourceConvert.Option = {
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
							format: MainScript.g_setting.executor_pvz2_resource_convert_ptx_format_map_list[option_ptx_format],
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
						},
					};
					Support.Pvz2.ResourceConvert.convert_fs(
						`${bundle_directory}/resource`,
						`${bundle_directory}/definition.json`,
						`${bundle_directory}/resource_manifest.json`,
						option,
					);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
