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
			resource_directory: string;
			packet_file: string;
			packet_manifest_file: string;
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
		json: boolean | '?input';
		json_crypt: boolean | '?input';
		json_crypt_key: string | '?input';
		image: boolean | '?input';
		image_texture_format_map_list: Record<string, Support.PvZ2.RSB.ResourceExtract.TextureFormatMap>;
		image_texture_format_map_name: string | '?input';
		image_atlas: boolean | '?input';
		image_atlas_resize: boolean | '?input';
		image_sprite: boolean | '?input';
		animation: boolean | '?input';
		animation_json: boolean | '?input';
		animation_flash: boolean | '?input';
		audio: boolean | '?input';
		audio_tool: {
			ffmpeg: string;
			ww2ogg: string;
			ww2ogg_pcb: string;
		};
	};

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: string | '?input';
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
					bundle_directory: string;
					data_file: string | '?default' | '?input';
					mode: RSBPackAndUnpackMode | '?input';
					version_number: bigint | '?input';
					version_additional_texture_information_for_pvz_2_chinese_android: bigint | '?input';
					data_buffer_size: string | '?input';
					input_packet: boolean | '?input';
					output_new_packet: boolean | '?input';
				}) {
					let bundle_directory: string;
					let data_file: string;
					let mode: RSBPackAndUnpackMode;
					let version_number: bigint;
					let version_additional_texture_information_for_pvz_2_chinese_android: bigint;
					let data_buffer_size: bigint;
					let input_packet: boolean;
					let output_new_packet: boolean;
					{
						bundle_directory = a.bundle_directory;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: bundle_directory.replace(/((\.rsb)(\.bundle))?$/i, '.rsb'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.mode === '?input') {
							Console.notify(
								'i',
								`请选择数据导入模式`,
								[
									`1. 群组：按群+子群树形结构导入，资源与子包均导入自group/<群名>/<子群名>目录`,
									`2. 子群：按子群树形结构导入，资源与子包均导入自subgroup/<子群名>目录`,
									`3. 资源：所有资源导入自resource目录，所有子包导入自packet目录`,
								],
							);
							mode = RSBPackAndUnpackModeE[
								Console.number(Check.enum_checkerx([1, 2, 3]))! as 1 | 2 | 3
								- 1
							];
						} else {
							mode = a.mode;
						}
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（3~4）`, []);
							version_number = Console.integer(Check.enum_checkerx([3n, 4n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.version_additional_texture_information_for_pvz_2_chinese_android === '?input') {
							Console.notify('i', `请输入用于PvZ2中文安卓版RSB的纹理拓展数据编号（0~2）`, []);
							version_additional_texture_information_for_pvz_2_chinese_android = Console.integer(Check.enum_checkerx([0n, 1n, 2n]))!;
						} else {
							version_additional_texture_information_for_pvz_2_chinese_android = BigInt(a.version_additional_texture_information_for_pvz_2_chinese_android);
						}
						if (a.data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存包数据输出的内存空间大小`, []);
							data_buffer_size = Console.size()!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
						if (a.input_packet === '?input') {
							Console.notify('i', `是否使用已有子包`, []);
							input_packet = Console.yon()!;
						} else {
							input_packet = a.input_packet;
						}
						if (a.output_new_packet === '?input') {
							Console.notify('i', `是否导出新打包的子包（导出至对应模式下的子包文件路径，会覆盖原有子包文件）`, []);
							output_new_packet = Console.yon()!;
						} else {
							output_new_packet = a.output_new_packet;
						}
					}
					let relative_path = make_rsb_package_relative_path(mode);
					let manifest_file = `${bundle_directory}/manifest.json`;
					let description_file = `${bundle_directory}/description.json`;
					let resource_directory = `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !input_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					let new_packet_file = !output_new_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					CoreX.Tool.PopCap.RSB.pack_fs(data_file, manifest_file, description_file, resource_directory, packet_file, new_packet_file, { number: version_number as any, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android as any }, data_buffer_size);
					Console.notify('s', `输出路径：${data_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					mode: '?input',
					version_number: '?input',
					version_additional_texture_information_for_pvz_2_chinese_android: '?input',
					data_buffer_size: config.pack_buffer_size,
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
					data_file: string;
					bundle_directory: string | '?default' | '?input';
					mode: RSBPackAndUnpackMode | '?input';
					version_number: bigint | '?input';
					version_additional_texture_information_for_pvz_2_chinese_android: bigint | '?input';
					output_resource: boolean | '?input';
					output_packet: boolean | '?input';
				}) {
					let data_file: string;
					let bundle_directory: string;
					let mode: RSBPackAndUnpackMode;
					let version_number: bigint;
					let version_additional_texture_information_for_pvz_2_chinese_android: bigint;
					let output_resource: boolean;
					let output_packet: boolean;
					{
						data_file = a.data_file;
						bundle_directory = ArgumentParser.path(a.bundle_directory, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.rsb))?$/i, '.rsb.bundle'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.mode === '?input') {
							Console.notify(
								'i',
								`请选择数据导出模式`,
								[
									`1. 群组：按群+子群树形结构导出，资源与子包均导出至group/<群名>/<子群名>目录`,
									`2. 子群：按子群树形结构导出，资源与子包均导出至subgroup/<子群名>目录`,
									`3. 资源：所有资源导出至resource目录，所有子包导出至packet目录`,
								],
							);
							mode = RSBPackAndUnpackModeE[
								Console.number(Check.enum_checkerx([1, 2, 3]))! as 1 | 2 | 3
								- 1
							];
						} else {
							mode = a.mode;
						}
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（3~4）`, []);
							version_number = Console.integer(Check.enum_checkerx([3n, 4n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.version_additional_texture_information_for_pvz_2_chinese_android === '?input') {
							Console.notify('i', `请输入用于PvZ2中文安卓版RSB的纹理拓展数据编号（0~2）`, []);
							version_additional_texture_information_for_pvz_2_chinese_android = Console.integer(Check.enum_checkerx([0n, 1n, 2n]))!;
						} else {
							version_additional_texture_information_for_pvz_2_chinese_android = BigInt(a.version_additional_texture_information_for_pvz_2_chinese_android);
						}
						if (a.output_resource === '?input') {
							Console.notify('i', `是否导出资源`, []);
							output_resource = Console.yon()!;
						} else {
							output_resource = a.output_resource;
						}
						if (a.output_packet === '?input') {
							Console.notify('i', `是否导出子包`, []);
							output_packet = Console.yon()!;
						} else {
							output_packet = a.output_packet;
						}
					}
					let relative_path = make_rsb_package_relative_path(mode);
					let manifest_file = `${bundle_directory}/manifest.json`;
					let description_file = `${bundle_directory}/description.json`;
					let resource_directory = !output_resource ? null : `${bundle_directory}/${relative_path.resource_directory}`;
					let packet_file = !output_packet ? null : `${bundle_directory}/${relative_path.packet_file}`;
					CoreX.Tool.PopCap.RSB.unpack_fs(data_file, manifest_file, description_file, resource_directory, packet_file, { number: version_number as any, additional_texture_information_for_pvz_2_chinese_android: version_additional_texture_information_for_pvz_2_chinese_android as any });
					Console.notify('s', `输出路径：${bundle_directory}`, []);
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
					data_file: string;
					bundle_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
					version_additional_texture_information_for_pvz_2_chinese_android: bigint | '?input';
					option: ResourceExtractOption;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: bigint;
					let version_additional_texture_information_for_pvz_2_chinese_android: bigint;
					let option: Support.PvZ2.RSB.ResourceExtract.Option = {
						json: null,
						image: null,
						animation: null,
						audio: null,
					};
					{
						data_file = a.data_file;
						bundle_directory = ArgumentParser.path(a.bundle_directory, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.rsb))?$/i, '.rsb.bundle'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（3~4）`, []);
							version_number = Console.integer(Check.enum_checkerx([3n, 4n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.version_additional_texture_information_for_pvz_2_chinese_android === '?input') {
							Console.notify('i', `请输入用于PvZ2中文安卓版RSB的纹理拓展数据编号（0~2）`, []);
							version_additional_texture_information_for_pvz_2_chinese_android = Console.integer(Check.enum_checkerx([0n, 1n, 2n]))!;
						} else {
							version_additional_texture_information_for_pvz_2_chinese_android = BigInt(a.version_additional_texture_information_for_pvz_2_chinese_android);
						}
						let extract_directory = `${bundle_directory}/extract`;
						{
							let json: boolean;
							if (a.option.json === '?input') {
								Console.notify('i', `是否提取JSON`, []);
								json = Console.yon()!;
							} else {
								json = a.option.json;
							}
							if (json) {
								option.json = {
									directory: extract_directory,
									crypt: null,
								};
								let crypt: boolean;
								if (a.option.json_crypt === '?input') {
									Console.notify('i', `RTON是否已被加密`, []);
									crypt = Console.yon()!;
								} else {
									crypt = a.option.json_crypt;
								}
								if (crypt) {
									let key: string;
									if (a.option.json_crypt_key === '?input') {
										Console.notify('i', `请输入RTON密钥`, []);
										key = Console.string()!;
									} else {
										key = a.option.json_crypt_key;
									}
									option.json.crypt = {
										key: key,
									};
								}
							}
						}
						{
							let image: boolean;
							if (a.option.image === '?input') {
								Console.notify('i', `是否提取图像`, []);
								image = Console.yon()!;
							} else {
								image = a.option.image;
							}
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
										throw new MyError(`texture format map list is empty`);
									}
									if (a.option.image_texture_format_map_name === '?input') {
										Console.notify('i', `请选择所应用的纹理格式`, map_name_list.map((e, i) => (`${i + 1}. ${e}`)));
										let map_name_index = Console.number(Check.enum_checkerx(map_name_list.map((e, i) => (i + 1))))!;
										option.image.texture_format_map = a.option.image_texture_format_map_list[map_name_list[map_name_index - 1]];
									} else {
										if (a.option.image_texture_format_map_list[a.option.image_texture_format_map_name] === undefined) {
											throw new MyError(`texture format map not found : ${a.option.image_texture_format_map_name}`);
										}
										option.image.texture_format_map = a.option.image_texture_format_map_list[a.option.image_texture_format_map_name];
									}
								}
								let atlas: boolean;
								if (a.option.image_atlas === '?input') {
									Console.notify('i', `是否提取图像Atlas`, []);
									atlas = Console.yon()!;
								} else {
									atlas = a.option.image_atlas;
								}
								let sprite: boolean;
								if (a.option.image_sprite === '?input') {
									Console.notify('i', `是否提取图像Sprite（分解Atlas）`, []);
									sprite = Console.yon()!;
								} else {
									sprite = a.option.image_sprite;
								}
								if (atlas) {
									let resize: boolean;
									if (a.option.image_atlas_resize === '?input') {
										Console.notify('i', `是否调整Atlas尺寸（以资源清单内的定义为准）`, []);
										resize = Console.yon()!;
									} else {
										resize = a.option.image_atlas_resize;
									}
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
							if (a.option.animation === '?input') {
								Console.notify('i', `是否提取动画`, []);
								animation = Console.yon()!;
							} else {
								animation = a.option.animation;
							}
							if (animation) {
								option.animation = {
									directory: extract_directory,
									json: null,
									flash: null,
								};
								let json: boolean;
								if (a.option.animation_json === '?input') {
									Console.notify('i', `是否提取动画为JSON`, []);
									json = Console.yon()!;
								} else {
									json = a.option.animation_json;
								}
								let flash: boolean;
								if (a.option.animation_flash === '?input') {
									Console.notify('i', `是否提取动画为Flash`, []);
									flash = Console.yon()!;
								} else {
									flash = a.option.animation_flash;
								}
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
							if (a.option.audio === '?input') {
								Console.notify('i', `是否提取音频`, []);
								audio = Console.yon()!;
							} else {
								audio = a.option.audio;
							}
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
					Console.notify('s', `输出路径：${bundle_directory}`, []);
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