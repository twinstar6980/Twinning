namespace TwinStar.Script.Entry.method.popcap.u_texture {

	// ------------------------------------------------

	// encode
	// decode

	type Config = {
		version_compress_texture_data: Executor.RequestArgument<boolean, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.u_texture.encode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					image_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					format: Executor.RequestArgument<string, false>;
					version_compress_texture_data: Executor.RequestArgument<boolean, false>;
				}) {
					let image_file: string;
					let data_file: string;
					let format: CoreX.Tool.PopCap.UTexture.Format;
					let version_compress_texture_data: boolean;
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
							() => (image_file.replace(/((\.png))?$/i, '.tex')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						format = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							() => (Console.option(CoreX.Tool.PopCap.UTexture.FormatE.map((e) => ([e])), null)),
							(value) => (CoreX.Tool.PopCap.UTexture.FormatE.includes(value as any) ? null : los(`选项非法`)),
						);
						version_compress_texture_data = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_compress_texture_data'),
							a.version_compress_texture_data,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.UTexture.encode_fs(data_file, image_file, format, { compress_texture_data: version_compress_texture_data });
					Console.notify('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					image_file: undefined!,
					data_file: '?default',
					format: '?input',
					version_compress_texture_data: config.version_compress_texture_data,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'image_file',
			}),
			Executor.method_of({
				id: 'popcap.u_texture.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					image_file: Executor.RequestArgument<string, true>;
					version_compress_texture_data: Executor.RequestArgument<boolean, false>;
				}) {
					let data_file: string;
					let image_file: string;
					let version_compress_texture_data: boolean;
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
							() => (data_file.replace(/((\.tex))?$/i, '.png')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_compress_texture_data = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_compress_texture_data'),
							a.version_compress_texture_data,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.UTexture.decode_fs(data_file, image_file, { compress_texture_data: version_compress_texture_data });
					Console.notify('s', los(`执行成功`), [`${image_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					image_file: '?default',
					version_compress_texture_data: config.version_compress_texture_data,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.tex)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.u_texture._injector,
});