namespace TwinStar.Script.Entry.method.popcap.resource_stream_bundle {

	// ------------------------------------------------

	const ResourceStreamBundlePackAndUnpackModeE = [
		'group',
		'subgroup',
		'resource',
	] as const;

	type ResourceStreamBundlePackAndUnpackMode = typeof ResourceStreamBundlePackAndUnpackModeE[number];

	function make_resource_stream_bundle_package_relative_path(
		mode: ResourceStreamBundlePackAndUnpackMode,
	) {
		let result: {
			resource_directory: Executor.RequireArgument<string>;
			packet_file: Executor.RequireArgument<string>;
			packet_manifest_file: Executor.RequireArgument<string>;
		};
		switch (mode) {
			case 'group': {
				result = {
					resource_directory: 'group/{0}/{1}/resource',
					packet_file: 'group/{0}/{1}/packet.rsg',
					packet_manifest_file: 'group/{0}/{1}/manifest.json',
				};
				break;
			}
			case 'subgroup': {
				result = {
					resource_directory: 'subgroup/{1}/resource',
					packet_file: 'subgroup/{1}/packet.rsg',
					packet_manifest_file: 'subgroup/{1}/manifest.json',
				};
				break;
			}
			case 'resource': {
				result = {
					resource_directory: 'resource',
					packet_file: 'packet/{1}.rsg',
					packet_manifest_file: 'packet/{1}.json',
				};
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

	type ResourceConvertOption = {
		json: Executor.RequestArgument<boolean, false>;
		json_crypt: Executor.RequestArgument<boolean, false>;
		json_crypt_key: Executor.RequestArgument<string, false>;
		image: Executor.RequestArgument<boolean, false>;
		image_texture_format_map_list: Record<string, Support.PvZ2.ResourceStreamBundle.ResourceConvert.TextureFormatMap>;
		image_texture_format_map_name: Executor.RequestArgument<string, false>;
		image_atlas: Executor.RequestArgument<boolean, false>;
		image_atlas_resize: Executor.RequestArgument<boolean, false>;
		image_sprite: Executor.RequestArgument<boolean, false>;
		animation: Executor.RequestArgument<boolean, false>;
		animation_json: Executor.RequestArgument<boolean, false>;
		animation_flash: Executor.RequestArgument<boolean, false>;
		audio: Executor.RequestArgument<boolean, false>;
		audio_tool: {
			ffmpeg_program: string;
			ww2ogg_program: string;
			ww2ogg_code_book: string;
		};
	};

	// ------------------------------------------------

	// pack
	// unpack
	// resource_convert
	// repair

	type Config = {
		mode: Executor.RequestArgument<string, false>;
		version_number: Executor.RequestArgument<bigint, false>;
		version_additional_texture_information_for_pvz_2_chinese_android: Executor.RequestArgument<bigint, false>;
		pack_buffer_size: Executor.RequestArgument<string, false>;
		resource_convert_option: ResourceConvertOption;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.resource_stream_bundle.pack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					bundle_directory: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					mode: Executor.RequestArgument<string, false>;
					version_number: Executor.RequestArgument<bigint, false>;
					version_additional_texture_information_for_pvz_2_chinese_android: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
					input_packet: Executor.RequestArgument<boolean, false>;
					output_new_packet: Executor.RequestArgument<boolean, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let mode: string;
					let version_number: bigint;
					let version_additional_texture_information_for_pvz_2_chinese_android: bigint;
					let buffer_size: bigint;
					let input_packet: boolean;
					let output_new_packet: boolean;
					{
						bundle_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.rsb)(\.bundle))?$/i, '.rsb')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option([
								[ResourceStreamBundlePackAndUnpackModeE[0], los(`群组：按群+子群树形结构导入，资源与子包均导入自group/<群名>/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[1], los(`子群：按子群树形结构导入，资源与子包均导入自subgroup/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[2], los(`资源：所有资源导入自resource目录，所有子包导入自packet目录`)],
							], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundle.VersionNumberE), null, null, initial)),
						);
						version_additional_texture_information_for_pvz_2_chinese_android = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_additional_texture_information_for_pvz_2_chinese_android'),
							a.version_additional_texture_information_for_pvz_2_chinese_android,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundle.VersionaAditionalTextureInformationForPVZ2ChineseAndroidE), null, null, initial)),
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
					let manifest_file = `${bundle_directory}/manifest.json`;
					let description_file = `${bundle_directory}/description.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					CoreX.Tool.PopCap.ResourceStreamBundle.pack_fs(data_file, manifest_file, description_file, resource_directory, packet_file, new_packet_file, { number: version_number as any, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android as any }, buffer_size);
					Console.message('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					mode: config.mode,
					version_number: config.version_number,
					version_additional_texture_information_for_pvz_2_chinese_android: config.version_additional_texture_information_for_pvz_2_chinese_android,
					buffer_size: config.pack_buffer_size,
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
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					bundle_directory: Executor.RequestArgument<string, true>;
					mode: Executor.RequestArgument<string, false>;
					version_number: Executor.RequestArgument<bigint, false>;
					version_additional_texture_information_for_pvz_2_chinese_android: Executor.RequestArgument<bigint, false>;
					output_resource: Executor.RequestArgument<boolean, false>;
					output_packet: Executor.RequestArgument<boolean, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let mode: string;
					let version_number: bigint;
					let version_additional_texture_information_for_pvz_2_chinese_android: bigint;
					let output_resource: boolean;
					let output_packet: boolean;
					{
						data_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.rsb))?$/i, '.rsb.bundle')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option([
								[ResourceStreamBundlePackAndUnpackModeE[0], los(`群组：按群+子群树形结构导出，资源与子包均导出至group/<群名>/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[1], los(`子群：按子群树形结构导出，资源与子包均导出至subgroup/<子群名>目录`)],
								[ResourceStreamBundlePackAndUnpackModeE[2], los(`资源：所有资源导出至resource目录，所有子包导出至packet目录`)],
							], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundle.VersionNumberE), null, null, initial)),
						);
						version_additional_texture_information_for_pvz_2_chinese_android = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_additional_texture_information_for_pvz_2_chinese_android'),
							a.version_additional_texture_information_for_pvz_2_chinese_android,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundle.VersionaAditionalTextureInformationForPVZ2ChineseAndroidE), null, null, initial)),
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
					let manifest_file = `${bundle_directory}/manifest.json`;
					let description_file = `${bundle_directory}/description.json`;
					let resource_directory = !output_resource ? null : `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !output_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					CoreX.Tool.PopCap.ResourceStreamBundle.unpack_fs(data_file, manifest_file, description_file, resource_directory, packet_file, { number: version_number as any, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android as any });
					Console.message('s', los(`执行成功`), [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					mode: config.mode,
					version_number: config.version_number,
					version_additional_texture_information_for_pvz_2_chinese_android: config.version_additional_texture_information_for_pvz_2_chinese_android,
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
				worker(a: Entry.CFSA & {
					bundle_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					version_additional_texture_information_for_pvz_2_chinese_android: Executor.RequestArgument<bigint, false>;
					option: ResourceConvertOption;
				}) {
					let bundle_directory: string;
					let version_number: bigint;
					let version_additional_texture_information_for_pvz_2_chinese_android: bigint;
					let option: Support.PvZ2.ResourceStreamBundle.ResourceConvert.Option = {
						json: null,
						image: null,
						animation: null,
						audio: null,
					};
					{
						bundle_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundle.VersionNumberE), null, null, initial)),
						);
						version_additional_texture_information_for_pvz_2_chinese_android = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_additional_texture_information_for_pvz_2_chinese_android'),
							a.version_additional_texture_information_for_pvz_2_chinese_android,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundle.VersionaAditionalTextureInformationForPVZ2ChineseAndroidE), null, null, initial)),
						);
						let convert_directory = `${bundle_directory}/convert`;
						{
							let json: boolean;
							json = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_json'),
								a.option.json,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (json) {
								option.json = {
									directory: convert_directory,
									crypt: null,
								};
								let crypt: boolean;
								crypt = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_json_crypt'),
									a.option.json_crypt,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								if (crypt) {
									let key: string;
									key = Executor.request_argument(
										Executor.query_argument_name(this.id, 'option_json_crypt_key'),
										a.option.json_crypt_key,
										(value) => (value),
										null,
										(initial) => (Console.string(null, null, initial)),
									);
									option.json.crypt = {
										key: key,
									};
								}
							}
						}
						{
							let image: boolean;
							image = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_image'),
								a.option.image,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (image) {
								option.image = {
									directory: convert_directory,
									texture_format_map: null!,
									atlas: null,
									sprite: null,
								};
								{
									let map_name_list = Object.keys(a.option.image_texture_format_map_list);
									assert_test(map_name_list.length !== 0, `texture format map list is empty`);
									let texture_format_map_name: string;
									texture_format_map_name = Executor.request_argument(
										Executor.query_argument_name(this.id, 'option_image_texture_format_map_name'),
										a.option.image_texture_format_map_name,
										(value) => (value),
										null,
										(initial) => (Console.option(map_name_list.map((e) => ([e])), null, null, initial)),
									);
									option.image.texture_format_map = a.option.image_texture_format_map_list[texture_format_map_name];
								}
								let atlas: boolean;
								atlas = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_image_atlas'),
									a.option.image_atlas,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								let sprite: boolean;
								sprite = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_image_sprite'),
									a.option.image_sprite,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								if (atlas) {
									let resize: boolean;
									resize = Executor.request_argument(
										Executor.query_argument_name(this.id, 'option_image_atlas_resize'),
										a.option.image_atlas_resize,
										(value) => (value),
										null,
										(initial) => (Console.confirmation(null, null, initial)),
									);
									option.image.atlas = {
										resize: resize,
									};
								}
								if (sprite) {
									option.image.sprite = {};
								}
							}
						}
						{
							let animation: boolean;
							animation = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_animation'),
								a.option.animation,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (animation) {
								option.animation = {
									directory: convert_directory,
									json: null,
									flash: null,
								};
								let json: boolean;
								json = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_animation_json'),
									a.option.animation_json,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								let flash: boolean;
								flash = Executor.request_argument(
									Executor.query_argument_name(this.id, 'option_animation_flash'),
									a.option.animation_flash,
									(value) => (value),
									null,
									(initial) => (Console.confirmation(null, null, initial)),
								);
								if (json) {
									option.animation.json = {};
								}
								if (flash) {
									option.animation.flash = {};
								}
							}
						}
						{
							let audio: boolean;
							audio = Executor.request_argument(
								Executor.query_argument_name(this.id, 'option_audio'),
								a.option.audio,
								(value) => (value),
								null,
								(initial) => (Console.confirmation(null, null, initial)),
							);
							if (audio) {
								option.audio = {
									directory: convert_directory,
									tool: {
										ffmpeg_program: HomeDirectory.of(a.option.audio_tool.ffmpeg_program),
										ww2ogg_program: HomeDirectory.of(a.option.audio_tool.ww2ogg_program),
										ww2ogg_code_book: HomeDirectory.of(a.option.audio_tool.ww2ogg_code_book),
									},
								};
							}
						}
					}
					Support.PvZ2.ResourceStreamBundle.ResourceConvert.convert_fs(
						`${bundle_directory}/resource`,
						`${bundle_directory}/manifest.json`,
						`${bundle_directory}/resource_manifest.json`,
						option,
						{ number: version_number as any, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android as any },
					);
					Console.message('s', los(`执行成功`), [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					version_number: config.version_number,
					version_additional_texture_information_for_pvz_2_chinese_android: config.version_additional_texture_information_for_pvz_2_chinese_android,
					option: config.resource_convert_option,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsb)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_bundle.repair',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/((\.rsb))?$/i, '.repair.rsb')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					Support.PopCap.ResourceStreamBundle.Repair.repair_package_fs(raw_file, ripe_file);
					Console.message('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'raw_file',
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