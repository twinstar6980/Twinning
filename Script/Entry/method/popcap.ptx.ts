/**
 * + popcap.ptx.encode PopCap-PTX 编码
 * + popcap.ptx.decode PopCap-PTX 解码
 */
namespace TwinStar.Entry.method.popcap.ptx {

	// ------------------------------------------------

	function input_image_size(
		message: string,
	): CoreX.Image.ImageSize {
		Console.notify('i', message, []);
		let width = Console.integer((value) => (value > 0n ? null : los(`尺寸应大于零`)));
		let height = Console.integer((value) => (value > 0n ? null : los(`尺寸应大于零`)));
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
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					image_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					format: Executor.RequestArgument<string, false>;
				}) {
					let image_file: string;
					let data_file: string;
					let format: Support.PopCapTexture.Encode.Format;
					{
						image_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'image_file'),
							a.image_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (image_file.replace(/((\.png))?$/i, '.ptx')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						format = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							() => (Console.option(Support.PopCapTexture.Encode.FormatE.map((e) => ([e])), null)),
							(value) => (Support.PopCapTexture.Encode.FormatE.includes(value as any) ? null : los(`选项非法`)),
						);
					}
					Support.PopCapTexture.Encode.encode_fs(image_file, data_file, format);
					Console.notify('s', los(`执行成功`), [`${data_file}`]);
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
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					image_file: Executor.RequestArgument<string, true>;
					format: Executor.RequestArgument<string, false>;
					image_size: Executor.RequestArgument<[bigint, bigint], false>;
				}) {
					let data_file: string;
					let image_file: string;
					let format: Support.PopCapTexture.Encode.Format;
					let image_size: [bigint, bigint];
					{
						data_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						image_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'image_file'),
							a.image_file,
							(value) => (value),
							() => (data_file.replace(/((\.ptx))?$/i, '.png')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						format = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							() => (Console.option(Support.PopCapTexture.Encode.FormatE.map((e) => ([e])), null)),
							(value) => (Support.PopCapTexture.Encode.FormatE.includes(value as any) ? null : los(`选项非法`)),
						);
						image_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'image_size'),
							a.image_size,
							(value) => (value),
							null,
							() => (input_image_size(``)), // todo : fix empty message line
							(value) => (value.length === 2 ? null : los(`数组长度非法`)),
						);
					}
					Support.PopCapTexture.Encode.decode_fs(data_file, image_file, image_size, format);
					Console.notify('s', los(`执行成功`), [`${image_file}`]);
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
	injector: TwinStar.Entry.method.popcap.ptx._injector,
});