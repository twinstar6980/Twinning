/**
 * + popcap.rsb.pack PopCap-RSB 打包
 * + popcap.rsb.unpack PopCap-RSB 解包
 * + popcap.rsb.extract_resource PopCap-RSB 资源提取
 */
namespace TwinKleS.Entry.method.popcap.rsb {

	// ------------------------------------------------

	const RSBPackAndUnpackModeE = [
		'group',
		'subgroup',
		'resource',
	] as const;

	type RSBPackAndUnpackMode = typeof RSBPackAndUnpackModeE[number];

	function make_rsb_package_relative_path(
		mode: RSBPackAndUnpackMode,
	) {
		let result: {
			resource_directory: Argument.Require<string>;
			packet_file: Argument.Require<string>;
			packet_manifest_file: Argument.Require<string>;
		};
		switch (mode) {
			case 'group': {
				result = {
					resource_directory: 'group/{0}/{1}/resource',
					packet_file: 'group/{0}/{1}/packet.rsgp',
					packet_manifest_file: 'group/{0}/{1}/manifest.json',
				};
				break;
			}
			case 'subgroup': {
				result = {
					resource_directory: 'subgroup/{1}/resource',
					packet_file: 'subgroup/{1}/packet.rsgp',
					packet_manifest_file: 'subgroup/{1}/manifest.json',
				};
				break;
			}
			case 'resource': {
				result = {
					resource_directory: 'resource',
					packet_file: 'packet/{1}.rsgp',
					packet_manifest_file: 'packet/{1}.json',
				};
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

	type ResourceExtractOption = {
		json: Argument.Request<boolean, false>;
		json_crypt: Argument.Request<boolean, false>;
		json_crypt_key: Argument.Request<string, false>;
		image: Argument.Request<boolean, false>;
		image_texture_format_map_list: Record<string, Support.PvZ2.RSB.ResourceExtract.TextureFormatMap>;
		image_texture_format_map_name: Argument.Request<string, false>;
		image_atlas: Argument.Request<boolean, false>;
		image_atlas_resize: Argument.Request<boolean, false>;
		image_sprite: Argument.Request<boolean, false>;
		animation: Argument.Request<boolean, false>;
		animation_json: Argument.Request<boolean, false>;
		animation_flash: Argument.Request<boolean, false>;
		audio: Argument.Request<boolean, false>;
		audio_tool: {
			ffmpeg: string;
			ww2ogg: string;
			ww2ogg_pcb: string;
		};
	};

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: Argument.Request<string, false>;
		resource_extract_option: ResourceExtractOption;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.rsb.pack',
				description: 'PopCap-RSB 打包',
				worker(a: Entry.CFSA & {
					bundle_directory: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					mode: Argument.Request<string, false>;
					version_number: Argument.Request<bigint, false>;
					version_additional_texture_information_for_pvz_2_chinese_android: bigint | '?input';
					buffer_size: Argument.Request<string, false>;
					input_packet: Argument.Request<boolean, false>;
					output_new_packet: Argument.Request<boolean, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let mode: RSBPackAndUnpackMode;
					let version_number: [3n, 4n][number];
					let version_additional_texture_information_for_pvz_2_chinese_android: [0n, 1n, 2n][number];
					let buffer_size: bigint;
					let input_packet: boolean;
					let output_new_packet: boolean;
					{
						bundle_directory = Argument.require(
							'捆绑目录', '',
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Argument.request(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.rsb)(\.bundle))?$/i, '.rsb')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						mode = Argument.request(
							'数据导入模式', '',
							a.mode,
							(value) => (value),
							null,
							() => (Console.option([
								[RSBPackAndUnpackModeE[0], `群组：按群+子群树形结构导入，资源与子包均导入自group/<群名>/<子群名>目录`],
								[RSBPackAndUnpackModeE[1], `子群：按子群树形结构导入，资源与子包均导入自subgroup/<子群名>目录`],
								[RSBPackAndUnpackModeE[2], `资源：所有资源导入自resource目录，所有子包导入自packet目录`],
							], null)),
							(value) => (RSBPackAndUnpackModeE.includes(value as any) ? null : `选项非法`),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([3n, 4n].includes(value) ? null : `版本不受支持`),
						);
						version_additional_texture_information_for_pvz_2_chinese_android = Argument.request(
							'用于PvZ2中文安卓版RSB的纹理拓展数据编号', '',
							a.version_additional_texture_information_for_pvz_2_chinese_android,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([0n, 1n, 2n].includes(value) ? null : `版本不受支持`),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
						input_packet = Argument.request(
							'使用已有子包', '',
							a.input_packet,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
						output_new_packet = Argument.request(
							'导出新打包的子包（导出至对应模式下的子包文件路径，会覆盖原有子包文件）', '',
							a.output_new_packet,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					let relative_path = make_rsb_package_relative_path(mode);
					let manifest_file = `${bundle_directory}/manifest.json`;
					let description_file = `${bundle_directory}/description.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					CoreX.Tool.PopCap.RSB.pack_fs(data_file, manifest_file, description_file, resource_directory, packet_file, new_packet_file, { number: version_number, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android }, buffer_size);
					Console.notify('s', `执行成功`, [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					mode: '?input',
					version_number: '?input',
					version_additional_texture_information_for_pvz_2_chinese_android: '?input',
					buffer_size: config.pack_buffer_size,
					input_packet: '?input',
					output_new_packet: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsb)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.rsb.unpack',
				description: 'PopCap-RSB 解包',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					bundle_directory: Argument.Request<string, true>;
					mode: Argument.Request<string, false>;
					version_number: Argument.Request<bigint, false>;
					version_additional_texture_information_for_pvz_2_chinese_android: bigint | '?input';
					output_resource: Argument.Request<boolean, false>;
					output_packet: Argument.Request<boolean, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let mode: RSBPackAndUnpackMode;
					let version_number: [3n, 4n][number];
					let version_additional_texture_information_for_pvz_2_chinese_android: [0n, 1n, 2n][number];
					let output_resource: boolean;
					let output_packet: boolean;
					{
						data_file = Argument.require(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Argument.request(
							'捆绑目录', '',
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.rsb))?$/i, '.rsb.bundle')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						mode = Argument.request(
							'数据导出模式', '',
							a.mode,
							(value) => (value),
							null,
							() => (Console.option([
								[RSBPackAndUnpackModeE[0], `群组：按群+子群树形结构导出，资源与子包均导出至group/<群名>/<子群名>目录`],
								[RSBPackAndUnpackModeE[1], `子群：按子群树形结构导出，资源与子包均导出至subgroup/<子群名>目录`],
								[RSBPackAndUnpackModeE[2], `资源：所有资源导出至resource目录，所有子包导出至packet目录`],
							], null)),
							(value) => (RSBPackAndUnpackModeE.includes(value as any) ? null : `选项非法`),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([3n, 4n].includes(value) ? null : `版本不受支持`),
						);
						version_additional_texture_information_for_pvz_2_chinese_android = Argument.request(
							'用于PvZ2中文安卓版RSB的纹理拓展数据编号', '',
							a.version_additional_texture_information_for_pvz_2_chinese_android,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([0n, 1n, 2n].includes(value) ? null : `版本不受支持`),
						);
						output_resource = Argument.request(
							'导出资源', '',
							a.output_resource,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
						output_packet = Argument.request(
							'导出子包', '',
							a.output_packet,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					let relative_path = make_rsb_package_relative_path(mode);
					let manifest_file = `${bundle_directory}/manifest.json`;
					let description_file = `${bundle_directory}/description.json`;
					let resource_directory = !output_resource ? null : `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !output_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					CoreX.Tool.PopCap.RSB.unpack_fs(data_file, manifest_file, description_file, resource_directory, packet_file, { number: version_number, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android });
					Console.notify('s', `执行成功`, [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					mode: '?input',
					version_number: '?input',
					version_additional_texture_information_for_pvz_2_chinese_android: '?input',
					output_resource: '?input',
					output_packet: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.rsb.extract_resource',
				description: 'PopCap-RSB 资源提取',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					bundle_directory: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					version_additional_texture_information_for_pvz_2_chinese_android: bigint | '?input';
					option: ResourceExtractOption;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: [3n, 4n][number];
					let version_additional_texture_information_for_pvz_2_chinese_android: [0n, 1n, 2n][number];
					let option: Support.PvZ2.RSB.ResourceExtract.Option = {
						json: null,
						image: null,
						animation: null,
						audio: null,
					};
					{
						data_file = Argument.require(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Argument.request(
							'捆绑目录', '',
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.rsb))?$/i, '.rsb.bundle')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([3n, 4n].includes(value) ? null : `版本不受支持`),
						);
						version_additional_texture_information_for_pvz_2_chinese_android = Argument.request(
							'用于PvZ2中文安卓版RSB的纹理拓展数据编号', '',
							a.version_additional_texture_information_for_pvz_2_chinese_android,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([0n, 1n, 2n].includes(value) ? null : `版本不受支持`),
						);
						let extract_directory = `${bundle_directory}/extract`;
						{
							let json: boolean;
							json = Argument.request(
								'提取JSON', '',
								a.option.json,
								(value) => (value),
								null,
								() => (Console.confirm(null)),
								(value) => (null),
							);
							if (json) {
								option.json = {
									directory: extract_directory,
									crypt: null,
								};
								let crypt: boolean;
								crypt = Argument.request(
									'RTON已加密', '',
									a.option.json_crypt,
									(value) => (value),
									null,
									() => (Console.confirm(null)),
									(value) => (null),
								);
								if (crypt) {
									let key: string;
									key = Argument.request(
										'RTON密钥', '',
										a.option.json_crypt_key,
										(value) => (value),
										null,
										() => (Console.string(null)),
										(value) => (null),
									);
									option.json.crypt = {
										key: key,
									};
								}
							}
						}
						{
							let image: boolean;
							image = Argument.request(
								'提取图像', '',
								a.option.image,
								(value) => (value),
								null,
								() => (Console.confirm(null)),
								(value) => (null),
							);
							if (image) {
								option.image = {
									directory: extract_directory,
									texture_format_map: null!,
									atlas: null,
									sprite: null,
								};
								{
									let map_name_list = Object.keys(a.option.image_texture_format_map_list);
									if (map_name_list.length === 0) {
										throw new Error(`texture format map list is empty`);
									}
									if (a.option.image_texture_format_map_name === '?input') {
										Console.notify('i', `请选择所应用的纹理格式`, []);
										let map_name = Console.option(map_name_list.map((e) => ([e])), null);
										option.image.texture_format_map = a.option.image_texture_format_map_list[map_name];
									} else {
										if (a.option.image_texture_format_map_list[a.option.image_texture_format_map_name] === undefined) {
											throw new Error(`texture format map not found : ${a.option.image_texture_format_map_name}`);
										}
										option.image.texture_format_map = a.option.image_texture_format_map_list[a.option.image_texture_format_map_name];
									}
								}
								let atlas: boolean;
								atlas = Argument.request(
									'提取图像Atlas', '',
									a.option.image_atlas,
									(value) => (value),
									null,
									() => (Console.confirm(null)),
									(value) => (null),
								);
								let sprite: boolean;
								sprite = Argument.request(
									'提取图像Sprite（分解Atlas）', '',
									a.option.image_sprite,
									(value) => (value),
									null,
									() => (Console.confirm(null)),
									(value) => (null),
								);
								if (atlas) {
									let resize: boolean;
									resize = Argument.request(
										'调整Atlas尺寸（以资源清单内的定义为准）', '',
										a.option.image_atlas_resize,
										(value) => (value),
										null,
										() => (Console.confirm(null)),
										(value) => (null),
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
							animation = Argument.request(
								'提取动画', '',
								a.option.animation,
								(value) => (value),
								null,
								() => (Console.confirm(null)),
								(value) => (null),
							);
							if (animation) {
								option.animation = {
									directory: extract_directory,
									json: null,
									flash: null,
								};
								let json: boolean;
								json = Argument.request(
									'提取动画为JSON', '',
									a.option.animation_json,
									(value) => (value),
									null,
									() => (Console.confirm(null)),
									(value) => (null),
								);
								let flash: boolean;
								flash = Argument.request(
									'提取动画为Flash', '',
									a.option.animation_flash,
									(value) => (value),
									null,
									() => (Console.confirm(null)),
									(value) => (null),
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
							audio = Argument.request(
								'提取音频', '',
								a.option.audio,
								(value) => (value),
								null,
								() => (Console.confirm(null)),
								(value) => (null),
							);
							if (audio) {
								option.audio = {
									directory: extract_directory,
									tool: {
										ffmpeg: Main.path_at_home(a.option.audio_tool.ffmpeg),
										ww2ogg: Main.path_at_home(a.option.audio_tool.ww2ogg),
										ww2ogg_pcb: Main.path_at_home(a.option.audio_tool.ww2ogg_pcb),
									},
									temp_directory: `${bundle_directory}/audio_temp`,
								};
							}
						}
					}
					Support.PvZ2.RSB.ResourceExtract.extract_package(
						data_file,
						`${bundle_directory}/manifest.json`,
						`${bundle_directory}/resource_manifest.json`,
						`${bundle_directory}/resource`,
						option,
						version_number,
						version_additional_texture_information_for_pvz_2_chinese_android,
					);
					CoreX.FileSystem.remove(`${bundle_directory}/audio_temp`);
					Console.notify('s', `执行成功`, [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: '?input',
					version_additional_texture_information_for_pvz_2_chinese_android: '?input',
					option: config.resource_extract_option,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.rsb._injector,
});