namespace TwinStar.Script.Entry.method.popcap.sexy_texture {

	// ------------------------------------------------

	// encode
	// decode

	type Config = {
		version_number: Executor.RequestArgument<bigint, false>;
		encode_compress_texture_data: Executor.RequestArgument<boolean, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.sexy_texture.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					image_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					format: Executor.RequestArgument<string, false>;
					compress_texture_data: Executor.RequestArgument<boolean, false>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let image_file: string;
					let data_file: string;
					let format: string;
					let compress_texture_data: boolean;
					let version_number: bigint;
					{
						image_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'image_file'),
							a.image_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (image_file.replace(/((\.png))?$/i, '.tex')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						format = Executor.request_argument(
							Executor.query_argument_name(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							(initial) => (Console.option(CoreX.Tool.PopCap.SexyTexture.FormatE.map((e) => ([e])), null, null, initial)),
						);
						compress_texture_data = Executor.request_argument(
							Executor.query_argument_name(this.id, 'compress_texture_data'),
							a.compress_texture_data,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.SexyTexture.VersionNumberE), null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.SexyTexture.encode_fs(data_file, image_file, format as any, compress_texture_data, { number: version_number as any });
					Console.message('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					image_file: undefined!,
					data_file: '?default',
					format: '?input',
					compress_texture_data: config.encode_compress_texture_data,
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'image_file',
			}),
			Executor.method_of({
				id: 'popcap.sexy_texture.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					image_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file: string;
					let image_file: string;
					let version_number: bigint;
					{
						data_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						image_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'image_file'),
							a.image_file,
							(value) => (value),
							() => (data_file.replace(/((\.tex))?$/i, '.png')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.SexyTexture.VersionNumberE), null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.SexyTexture.decode_fs(data_file, image_file, { number: version_number as any });
					Console.message('s', los(`执行成功`), [`${image_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					image_file: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.tex)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.sexy_texture.encode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					image_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					format: Executor.RequestArgument<string, false>;
					compress_texture_data: Executor.RequestArgument<boolean, false>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let image_file_directory: string;
					let data_file_directory: string;
					let format: string;
					let compress_texture_data: boolean;
					let version_number: bigint;
					{
						image_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'image_file_directory'),
							a.image_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (image_file_directory.replace(/$/i, '.encode')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						format = Executor.request_argument(
							Executor.query_argument_name(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							(initial) => (Console.option(CoreX.Tool.PopCap.SexyTexture.FormatE.map((e) => ([e])), null, null, initial)),
						);
						compress_texture_data = Executor.request_argument(
							Executor.query_argument_name(this.id, 'compress_texture_data'),
							a.compress_texture_data,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.SexyTexture.VersionNumberE), null, null, initial)),
						);
					}
					simple_batch_execute(
						image_file_directory,
						['file', /.+(\.png)$/i],
						(item) => {
							let image_file = `${image_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -4)}.tex`;
							CoreX.Tool.PopCap.SexyTexture.encode_fs(data_file, image_file, format as any, compress_texture_data, { number: version_number as any });
						},
					);
					Console.message('s', los(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					image_file_directory: undefined!,
					data_file_directory: '?default',
					format: '?input',
					compress_texture_data: config.encode_compress_texture_data,
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'image_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.sexy_texture.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					image_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file_directory: string;
					let image_file_directory: string;
					let version_number: bigint;
					{
						data_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						image_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'image_file_directory'),
							a.image_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.SexyTexture.VersionNumberE), null, null, initial)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.tex)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let image_file = `${image_file_directory}/${item.slice(0, -4)}.png`;
							CoreX.Tool.PopCap.SexyTexture.decode_fs(data_file, image_file, { number: version_number as any });
						},
					);
					Console.message('s', los(`执行成功`), [`${image_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					image_file_directory: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.sexy_texture._injector,
});