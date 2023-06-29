namespace TwinStar.Script.Entry.method.popcap.texture {

	// ------------------------------------------------

	// encode
	// decode

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.texture.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					image_file: Executor.Argument<string, false>;
					data_file: Executor.Argument<string, true>;
					format: Executor.Argument<string, false>;
				}) {
					let image_file: string;
					let data_file: string;
					let format: string;
					{
						image_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'image_file'),
							a.image_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (image_file.replace(/((\.png))?$/i, '.ptx')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						format = Executor.request_argument(
							Executor.query_argument_name(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(Support.PopCap.Texture.Encode.FormatE), null, null, initial)),
						);
					}
					Support.PopCap.Texture.Encode.encode_fs(image_file, data_file, format as any);
					return [`${data_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					image_file: undefined!,
					data_file: '?default',
					format: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'image_file',
			}),
			Executor.method_of({
				id: 'popcap.texture.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.Argument<string, false>;
					image_file: Executor.Argument<string, true>;
					format: Executor.Argument<string, false>;
					image_width: Executor.Argument<bigint, false>;
					image_height: Executor.Argument<bigint, false>;
				}) {
					let data_file: string;
					let image_file: string;
					let format: string;
					let image_width: bigint;
					let image_height: bigint;
					{
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						image_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'image_file'),
							a.image_file,
							(value) => (value),
							() => (data_file.replace(/((\.ptx))?$/i, '.png')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						format = Executor.request_argument(
							Executor.query_argument_name(this.id, 'format'),
							a.format,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(Support.PopCap.Texture.Encode.FormatE), null, null, initial)),
						);
						image_width = Executor.request_argument(
							Executor.query_argument_name(this.id, 'image_width'),
							a.image_width,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => ((0n < value) ? null : los(`尺寸应大于零`)), initial)),
						);
						image_height = Executor.request_argument(
							Executor.query_argument_name(this.id, 'image_height'),
							a.image_height,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => ((0n < value) ? null : los(`尺寸应大于零`)), initial)),
						);
					}
					Support.PopCap.Texture.Encode.decode_fs(data_file, image_file, [image_width, image_height], format as any);
					return [`${image_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file: undefined!,
					image_file: '?default',
					format: '?input',
					image_width: '?input',
					image_height: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.ptx)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.texture._injector,
});