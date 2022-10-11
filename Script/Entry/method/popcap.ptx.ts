/**
 * + popcap.ptx.encode PopCap-PTX 编码
 * + popcap.ptx.decode PopCap-PTX 解码
 */
namespace TwinKleS.Entry.method.popcap.ptx {

	// ------------------------------------------------

	function input_image_size(
		message: string,
	): CoreX.Image.ImageSize {
		Console.notify('i', message, []);
		let width = Console.integer((value) => (value > 0n ? null : `尺寸不可为0`))!;
		let height = Console.integer((value) => (value > 0n ? null : `尺寸不可为0`))!;
		return [width, height];
	}

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.ptx.encode',
				description: 'PopCap-PTX 编码',
				worker(a: Entry.CFSA & {
					image_file: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					format: Argument.Request<string, false>;
				}) {
					let image_file: string;
					let data_file: string;
					let format: Support.PopCapTexture.Encode.Format;
					{
						image_file = Argument.require(
							'图像文件', '',
							a.image_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Argument.request(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							() => (image_file.replace(/((\.png))?$/i, '.ptx')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						format = Argument.request(
							'纹理格式', '',
							a.format,
							(value) => (value),
							null,
							() => (Console.option(Support.PopCapTexture.Encode.FormatE.map((e) => ([e])), null)),
							(value) => (Support.PopCapTexture.Encode.FormatE.includes(value as any) ? null : `选项非法`),
						);
					}
					Support.PopCapTexture.Encode.encode_fs(image_file, data_file, format);
					Console.notify('s', `执行成功`, [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					image_file: undefined!,
					data_file: '?default',
					format: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'image_file',
			}),
			Executor.method_of({
				id: 'popcap.ptx.decode',
				description: 'PopCap-PTX 解码',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					image_file: Argument.Request<string, true>;
					format: Argument.Request<string, false>;
					image_size: [bigint, bigint] | '?input';
				}) {
					let data_file: string;
					let image_file: string;
					let format: Support.PopCapTexture.Encode.Format;
					let image_size: [bigint, bigint];
					{
						data_file = Argument.require(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						image_file = Argument.request(
							'图像文件', '',
							a.image_file,
							(value) => (value),
							() => (data_file.replace(/((\.ptx))?$/i, '.png')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						format = Argument.request(
							'纹理格式', '',
							a.format,
							(value) => (value),
							null,
							() => (Console.option(Support.PopCapTexture.Encode.FormatE.map((e) => ([e])), null)),
							(value) => (Support.PopCapTexture.Encode.FormatE.includes(value as any) ? null : `选项非法`),
						);
						image_size = Argument.request(
							'图像尺寸', '',
							a.image_size,
							(value) => (value),
							null,
							() => (input_image_size(``)),
							(value) => (value.length === 2 ? null : `数组长度非法`),
						);
					}
					Support.PopCapTexture.Encode.decode_fs(data_file, image_file, image_size, format);
					Console.notify('s', `执行成功`, [`${image_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					image_file: '?default',
					format: '?input',
					image_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.ptx)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.ptx._injector,
});